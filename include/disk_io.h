#ifndef _DISK_IO_H
#define _DISK_IO_H

#ifndef FILE_NAME_MAX
#define FILE_NAME_MAX 20
#endif

struct file{
	char name[FILE_NAME_MAX];
	int size;
	int blocks;
	int pointer;
};
typedef file filedata;

struct head{
	int magic_number;
	int block_size;
	int no_blocks;
	int available_blocks;
	int no_files;
};
typedef head header;

char *readblock(int n);
void writeblock(int n, char *a);
void init(char harddisk_name[], int blocksize);
void mount(char harddisk_name[], int blocksize);
int get_filesize(char filename[]);
void freebuffer(char *buffer);

#endif