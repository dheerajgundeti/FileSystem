#include<malloc.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<stdio.h>


#include "helpers.h"

#ifndef MAX_LINE_COUNT
#define MAX_LINE_COUNT 100
#endif

void purify(char *s){
	int i = 0, ind = 0;
	while (s[i] == ' '){
		i++;
	}
	for (; s[i] != '\n'; i++){
		if (s[i] != ' '){
			s[ind] = s[i];
			ind++;
		}
		else if (s[i] == ' ' && s[i + 1] != ' '){
			s[ind] = s[i];
			ind++;
		}
	}
	s[ind] = '\0';
}

void fill_nth_symboldata(char *str, symboldata *symboltable, int ind, int *memory_index){

	char temp[MAX_NAME_LEN];
	int i = 0;
	while (str[i] != ' '){
		i++;
	}
	i++;
	int j = 0;
	while (str[i] != EOF && str[i] != '\0' && str[i] != '[' && str[i] != ' '){
		temp[j] = str[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	j = 0;
	symboltable[ind].size = 1;
	strcpy(symboltable[ind].name, temp);
	if (str[i] != EOF && str[i] != '\0' && str[i] == '['){
		i++;
		while (str[i] != '\0' && str[i] != ']'){
			temp[j] = str[i];
			i++;
			j++;
		}
		temp[j] = '\0';
		symboltable[ind].size = atoi(temp);
	}
	symboltable[ind].address = memory_index[0];
	memory_index[0] = memory_index[0] + symboltable[ind].size;
}

symboldata *fill_symbol_table(char *s, int no_symbols, int *memory_index){

	symboldata *symboltable;
	symboltable = (symboldata *)calloc(no_symbols, sizeof(symboldata));

	char* token = strtok(s, "\n");
	int i = 0;

	while (i<no_symbols && token != NULL) {
		fill_nth_symboldata(token, symboltable, i, memory_index);
		token = strtok(NULL, "\n");
		i++;
	}
	return symboltable;
}
int count_lines(char *file){
	FILE *fp;
	char * line;
	line = (char *)calloc(MAX_LINE_COUNT, sizeof(char));
	fp = fopen(file, "r");
	if (fp == NULL){
		printf("CANNOT OPEN FILE\n");
		return 0;
	}
	int lines = 0, ok = 0;
	while (fgets(line, MAX_LINE_COUNT, fp) != NULL){
		lines++;
		ok++;
		if (strcmp(line, "START:\n") == 0){
			lines = lines - ok;
		}
	}
	fclose(fp);
	return lines - 1;
}

int check_for_register(char *s){
	if (strcmp(s, "AX") == 0 || strcmp(s, "BX") == 0 || strcmp(s, "CX") == 0 || strcmp(s, "DX") == 0
		|| strcmp(s, "EX") == 0 || strcmp(s, "FX") == 0 || strcmp(s, "GX") == 0 || strcmp(s, "HX") == 0)
		return 1;
	return 0;
}

void update_labeltable(char *command, int address, labeldata *labeltable, int *label_index){
	strcpy(labeltable[label_index[0]].name, command);
	labeltable[label_index[0]].address = address;
	label_index[0]++;
}

void init_outdata(intermediatedata *out_data, int lines){
	int i, j;
	for (i = 0; i < lines; i++){
		out_data[i].no = -1;
		out_data[i].opcode = -1;
		for (j = 0; j < 4; j++)
			out_data[i].parameters[j] = -1;
	}
}