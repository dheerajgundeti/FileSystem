#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>


#include "process.h"
#include "disk_io.h" 
#include "processCompiler.h"

#ifndef HEAD_LENGTH
#define HEAD_LENGTH 20
#endif

#ifndef FILEDAT_LENGTH
#define FILEDAT_LENGTH 32
#endif



void copytofs(char from[],char to[]){
	char *buffer;
	buffer = readblock(0);
	header *head;
	head = (header *)malloc(sizeof(header));
	memcpy(head, buffer, HEAD_LENGTH);
	filedata *filedat;
	filedat = (filedata *)malloc(sizeof(filedata));
	filedat->size = get_filesize(from);
	filedat->blocks = filedat->size / head->block_size;
	if (filedat->size % head->block_size != 0) filedat->blocks++;
	strcpy(filedat->name, to);

	int *blocks_for_file;
	blocks_for_file = (int *)calloc(filedat->blocks+1, sizeof(int));
	int count = 0,i,j;
	for (i = 0; i < head->no_blocks; i++){
		if (count>filedat->blocks)
			break;
		if (buffer[HEAD_LENGTH + i] == 'N') {
			buffer[HEAD_LENGTH + i] = 'Y';
			blocks_for_file[count] = i;
			count++;
		}
	}
	filedat->pointer = blocks_for_file[0];
	head->no_files++;
	head->available_blocks = head->available_blocks - filedat->blocks - 1;
	memcpy(buffer, head, HEAD_LENGTH);
	memcpy(buffer + (HEAD_LENGTH + head->no_blocks+(head->no_files - 1)*FILEDAT_LENGTH), filedat, FILEDAT_LENGTH);
	writeblock(0, buffer);

	freebuffer(buffer);
	memcpy(buffer,blocks_for_file+1,filedat->blocks*4);
	writeblock(filedat->pointer, buffer);

	FILE *fp;
	fp = fopen(from, "r");
	if (fp == NULL){
		printf("FILE NOT FOUND\n");
		return;
	}
	freebuffer(buffer);
	count = 0;
	for (i = 1; i < filedat->blocks; i++){
		for (j = 0; j < head->block_size; j++){
			char c;
			fread(&c, 1, 1, fp);
			buffer[j] = c;
		}
		writeblock(blocks_for_file[i],buffer);
		freebuffer(buffer);
	}
	int size = filedat->size - head->block_size*(filedat->blocks - 1);
	i = 0;
	char c;
	fread(&c, 1, 1, fp);
	while (!feof(fp) && size > 0){
		buffer[i] = c;
		size--;
		i++;
		fread(&c, 1, 1, fp);
	}
	writeblock(blocks_for_file[filedat->blocks], buffer);
}
void copyfromfs(char from[],char to[]){
	FILE *fp;
	fp = fopen(to, "w");
	if (fp == NULL){
		printf("FILE NOT FOUND\n");
		return;
	}
	char *buffer;
	buffer = readblock(0);
	header *head;
	head = (header *)malloc(sizeof(header));
	memcpy(head, buffer, HEAD_LENGTH);
	int start;
	filedata *filedat;
	filedat = (filedata *)malloc(sizeof(filedata));
	int j = 1;
	for (start = HEAD_LENGTH + head->no_blocks; j <=head->no_files ; start = start + FILEDAT_LENGTH, j++){
		memcpy(filedat, buffer + start, FILEDAT_LENGTH);
		if (strcmp(from, filedat->name)==0){
			break;
		}
	}
	int *blocks_for_file;
	blocks_for_file = (int *)calloc(filedat->blocks, sizeof(int));
	freebuffer(buffer);
	buffer = readblock(filedat->pointer);
	memcpy(blocks_for_file, buffer, (filedat->blocks+1) * 4);
	printf("%s,%d,%d\n", filedat->name, filedat->pointer, filedat->size);
	int count = 0,i;
	for (i = 1; i < filedat->blocks; i++){
		buffer = readblock(blocks_for_file[i]);
		for (j = 0; j < head->block_size; j++){
			char c;
			c=buffer[j];
			fwrite(&c, 1, 1, fp);
		}
		freebuffer(buffer);
	}
	int size = filedat->size - head->block_size*(filedat->blocks - 1);
	i = 0;
	char c;
	buffer = readblock(blocks_for_file[filedat->blocks-1]);
	j = 0;
	while (size > 0){
		c = buffer[j];
		size--;
		j++;
		fwrite(&c, 1, 1, fp);
	}
	fclose(fp);
}
void ls(){
	char *buffer;
	buffer = readblock(0);
	header *head;
	head = (header *)malloc(sizeof(header));
	memcpy(head, buffer, HEAD_LENGTH);
	int start;
	filedata *filedat;
	filedat = (filedata *)malloc(sizeof(filedata));
	int j = 1;
	for (start = HEAD_LENGTH + head->no_blocks; j <= head->no_files; start = start + FILEDAT_LENGTH, j++){
		memcpy(filedat, buffer + start, FILEDAT_LENGTH);
		printf("%s\n", filedat->name);
	}
	free(buffer);
	free(head);
	free(filedat);
}
void execute1(char *s){
	char command[101];
	strcpy(command, "");
	char argument[101];
	strcpy(argument, "");
	char argument1[100];
	strcpy(argument1, "");
	char file[100];
	char* token = strtok(s, " ");
	int i = 0;
	char present_hardisk[FILE_NAME_MAX];
	while (token != NULL) {
		if (i == 0) strcpy(command, token);
		if (i == 1) strcpy(argument, token);
		if (i == 2) strcpy(argument1, token);
		token = strtok(NULL, " ");
		i++;
	}
	if (strcmp(command, "copytofs") == 0){
		copytofs(argument,argument1);
	}
	if (strcmp(command, "ls") == 0){
		ls();
	}
	if (strcmp(command, "copyfromfs") == 0){
		copyfromfs(argument, argument1);
	}
	if (strcmp(command, "mount") == 0){
		char c;
		printf("NEW FILE(N) OR EXISTING(E) : ");
		scanf("%c", &c);
		if (c == 'N'){
			init(argument, atoi(argument1));
		}
		else{
			mount(argument, atoi(argument1));
		}
		strcpy(present_hardisk,argument);
	}
	if (strcmp(command, "format") == 0){
		mount(present_hardisk, atoi(argument));
	}
	if (strcmp(command, "compile") == 0){
		compilefile(argument, argument1);
	}
}