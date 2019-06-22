#ifndef _HELPERS_H
#define _HELPERS_H

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN 20
#endif

struct symdat{
	char name[MAX_NAME_LEN];
	int address;
	int size;
};
typedef struct symdat symboldata;

struct blodat{
	char name[MAX_NAME_LEN];
	int address;
};
typedef struct blodat labeldata;

struct inter{
	int no;
	int opcode;
	int parameters[4];
};
typedef inter intermediatedata;

void purify(char *s);
symboldata *fill_symbol_table(char *s, int no_symbols, int *memory_index);
int count_lines(char *file);
int check_for_register(char *s);
void update_labeltable(char *command, int address, labeldata *labeltable, int *label_index);
void init_outdata(intermediatedata *out_data, int lines);

#endif