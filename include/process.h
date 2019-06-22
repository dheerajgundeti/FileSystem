#ifndef _PROCESS_H
#define _PROCESS_H

#ifndef FILE_NAME_MAX
#define FILE_NAME_MAX 20
#endif

void execute1(char *command);
void copytofs(char from[], char to[]);
void copyfromfs(char from[], char to[]);

#endif