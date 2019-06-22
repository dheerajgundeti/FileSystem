#include<stdio.h>
#include<malloc.h>
#include<string.h>

#include "disk_io.h"

#define MAGIC_NUMBER 1234321

#ifndef HEAD_LENGTH
#define HEAD_LENGTH 20
#endif

static int FILE_SIZE;
static int BLOCK_SIZE;
char HARDDISK[FILE_NAME_MAX];

int get_filesize(char filename[]){
	FILE *fp;
	fp = fopen(filename, "rb+");
	fseek(fp, 0, SEEK_END);
	return ftell(fp);
	fclose(fp);
}


void freebuffer(char *buffer){
	int i = 0;
	for (i = 0; i < BLOCK_SIZE; i++){
		buffer[i] = NULL;
	}
}


void fill_initial_metadata(char *buffer){
	header *head;
	head = (header *)malloc(sizeof(header));
	head->magic_number = MAGIC_NUMBER;
	head->block_size = BLOCK_SIZE;
	head->no_blocks = FILE_SIZE/BLOCK_SIZE;
	head->available_blocks = head->no_blocks;
	head->no_files = 0;
	memcpy(buffer, head, HEAD_LENGTH);
	buffer[HEAD_LENGTH] = 'Y';
	int i;
	for (i = 1; i < head->no_blocks; i++) buffer[HEAD_LENGTH+i] = 'N';
}

//initializing existing file
void init(char harddisk_name[],int blocksize){

	FILE_SIZE = get_filesize(harddisk_name);
	BLOCK_SIZE = blocksize;
	strcpy(HARDDISK, harddisk_name);
	char *buffer;
	buffer = (char *)calloc(BLOCK_SIZE, sizeof(char));
	int i = 0;
	for (i = 0; i < BLOCK_SIZE; i++)
		buffer[i] = NULL;
	fill_initial_metadata(buffer);
	writeblock(0, buffer);
}

//loading existing file
void mount(char harddisk_name[], int blocksize){
	FILE_SIZE = get_filesize(harddisk_name);
	BLOCK_SIZE = blocksize;
	strcpy(HARDDISK, harddisk_name);
}


//read nth block from harddisk and store in bufer
char *readblock(int n){
	char *a;
	a = (char *)calloc(BLOCK_SIZE, sizeof(char));
	FILE *fp_disk;
	fp_disk = fopen(HARDDISK, "rb+");
	if (fp_disk == NULL){
		printf("CANNOT OPEN FILE %s\n", "hard");
		return a;
	}
	fseek(fp_disk, BLOCK_SIZE*n, SEEK_SET);
	int i;
	for (i = 0; i < BLOCK_SIZE; i++){
		fread(&a[i], 1, 1, fp_disk);
	}
	fclose(fp_disk);
	return a;
}

//take buffer and store at n th block in harddisk
void writeblock(int n, char *a){
	FILE *fp_disk;
	fp_disk = fopen("hardDisk.hdd", "rb+");
	if (fp_disk == NULL){
		printf("CANNOT OPEN FILE %s\n", "write hard");
		return;
	}
	fseek(fp_disk, BLOCK_SIZE*n, SEEK_SET);
	int i;
	for (i = 0; i < BLOCK_SIZE; i++){
		fwrite(&a[i], 1, 1, fp_disk);
	}
	fclose(fp_disk);
}