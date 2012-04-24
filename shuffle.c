#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "shuffle.h"

//readliner for readline return
char* readliner(int fd){
  char* line = (char*)malloc(256);
  char* c = (char*)malloc(1);
  do{
    read(fd, c, 1);
    strcat(line, c);
    if(*c == '\n')
      break;
  }while(*c);
  return line;
}

void remove_name(char** names, int i, int len){
  int j;
  for(j = i; j < len-1; j++)
    strcpy(names[j], names[j+1]);
}

void shuffle(int fd){
  printf("shuffle\n");
  int i, len, r;
  char* name = (char*)malloc(256);
  char* final = (char*)malloc(1024*256);
  char** names = (char**)malloc(1024*sizeof(char*));//list of names
  for(i = 0; i < 1024; i++)
    names[i] = (char*)malloc(256);

  //populating list of names
  name = readliner(fd);
  for(len = 0; *name; len++){
    strcpy(names[len], name);
    name = readliner(fd);
  }
  lseek(fd, 0, SEEK_SET);

  srand(time(0));
  // printf("before for\n");
  printf("len: %d\n", len);
  for(; len > 0; len--){//clear file, now open for writing
    r = rand()%len;
    printf("names[r]: %s", names[r]);
    strcat(final, names[r]);
    remove_name(names, r, len);
  }
  printf("final: %s\n", final);
  write(fd, final, strlen(final));
}
