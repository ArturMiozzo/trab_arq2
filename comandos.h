#include "arquivos.h"
#include <string.h>
#include <stdint.h>

//declaração dos comandos

int DIR(OBJETO caminho[255], int cur);

int mkfile(char *name, OBJETO caminho[255], int cur);

int mkdir(char *name, OBJETO caminho[255], int cur);

int MOVE(char *file, char *path, OBJETO caminho[255], int cur);

int RENAME(char *oldname, char *newname, OBJETO caminho[255], int cur);

int edit(char *name, char* newdata, OBJETO caminho[255], int cur);

int CD(char *args, OBJETO caminho[255], int cur);

int remove_(char *path, int force, OBJETO caminho[255], int cur);

int disk(char *path, OBJETO caminho[255], int cur);

int defrag(char *path, OBJETO caminho[255], int cur);
