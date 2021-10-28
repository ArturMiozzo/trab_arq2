#include <string.h>

//declaração dos comandos

int DIR(int folder);

int mkfile(char *name, int folder);

int mkdir(char *name, int folder);

int MOVE(char *file, char *path, int folder);

int RENAME(char *oldname, char *newname, int folder);

int edit(char *name, char* newdata,int folder);

int CD(char *args, int folder);
