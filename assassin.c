#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <assert.h>
#include "assassin.h"

#define READ 0
#define WRITE 1

int fromClient, live, dead, numplayers;

int main(){
  
  printf("Waiting for players or start command...");
  fflush(stdout);

   umask(0000);
   signal(SIGINT, sighandler);
   
     fromClient = server_init();
   
     char buffer[50];
   memset(buffer, 0, 50);
   
   live = open("live.txt", O_RDWR, 0664);
   dead = open("dead.txt", O_RDWR, 0664);
   
   //add players
   while(1){
     memset(buffer, 0, 50);
     if(read(fromClient, buffer, 50)){
       if(strchr(buffer, ' ')){
	 write(live, buffer, strlen(buffer));
       }
       else{
	 if(strcmp(buffer, "start\n")==0){
	   break;
	 }
	 else{
	   printf("Invalid name/password pair.\n");
	 }
       }
     }
   }
   
   //start play
   printf("Instructions:\nEnter first and last name of your victim in all lower case, with no space, and THEN a space, then the password you received from him/her.\nExample: johndoe password\n");
   numplayers = count_lines(live);
  
  while(1){
    memset(buffer, 0, 50);
    if(read(fromClient, buffer, 50)){
      //  printf("Message: %d'%s'\n", strlen(buffer), buffer);
      
      int check;
      check = check_key(live, dead, buffer);
      if(check==-1){
	printf("Invalid string.\n");
      }
      if(check==1){
	 printf("No match.\n");
      }
      if(check==-2){
	printf("Already dead.\n");
      }
      fflush(stdout);
    }
  }
}  	      

static void sighandler(int signo){
  if(signo == SIGINT){
    remove("upstream");
    close(live);
    close(dead);
    exit(0);
  }
} 
  
int server_init(){
  int fd;
  assert(mkfifo("upstream", 0644)==0);
  // printf("about to open upstream\n");
  fd = open("upstream", O_RDONLY);
  assert(fd!=-1);
  return fd;
}

  //returns 0 upon success, -1 (or any non-zero number) upon failure
  //success = properly formatted string
int check_key(int live, int dead, char* str){
  lseek(live, 0, SEEK_SET);
  lseek(dead, 0, SEEK_SET);
  //  printf("checking\n");
  if(!strchr(str, ' ')){
    //    printf("no space\n");
    return -1;
    }
  if(str[0]==' '){
    // printf("No name.\n");
    return -1;
  }
  
  if(is_dead(dead, str)){
    return -2;
  }

  char line[50];
  //check every line in live.txt against the input name and pw.
  //  if there is no match, return success.
  //if there is a match, write the name/pw in dead.txt.
  //then continue through live.txt and send back the next not dead name
  // printf("about to check line by line\n");
  while(readline(live, line)){
    //   printf("line: %d'%s'", strlen(line), line);
     //  printf("str: %d'%s'", strlen(str), str);
    if(strcmp(str, line)==0){
      //  printf("match\n");
      record(dead, str);
      //   printf("recorded\n");
      next_target(live, dead, line);
    
      printf("Kill successfully recorded. Your next target is %s.\n", line);
      check_win(str);
      return 0;
    }
  }
 
    return 1;
}

void check_win(char* name){
  int numdead;
  numdead = count_lines(dead);
  // printf("check win: %d %d\n", numplayers, numdead);
  if(numplayers-1==numdead){
    char name[50];
    //winner is the only target remaining
     next_target(live, dead, name);
  
  
    printf("GAME OVER!!! WINNER: %s\n", name);
    remove("upstream");
       exit(0);
  }
}

int count_lines(int file){
  int c;
  c = 0;
  lseek(file, 0, SEEK_SET);
  char dummy[50];
  while(readline(file, dummy))
    c++;
  lseek(file, 0, SEEK_SET);
  return c;
}

//read through live.txt from current position, find next not-dead person
//place his name in line
void next_target(int live, int dead, char* line){
  // printf("find target\n");
  
   lseek(dead, 0, SEEK_SET);
  while(readline(live, line)){
    //  printf("'%s'", line);
    if(!is_dead(dead, line)){
      (*strchr(line, ' ')) = '\0';
      return;
    }
  }
  
  lseek(live, 0, SEEK_SET);
  lseek(dead, 0, SEEK_SET);
  while(readline(live, line)){
    //  printf("'%s'", line);
    if(!is_dead(dead, line)){
      (*strchr(line, ' ')) = '\0';
      return;
    }
  }
}

//return true if str is in dead
int is_dead(int dead, char* str){
  char line[50];
  while(readline(dead, line)){
    if(strcmp(str, line)==0)
      return 1;
  }
  return 0;
}

void record(int dead, char* str){
   lseek(dead, 0, SEEK_END);
   write(dead, str, strlen(str));
}

int readline(int file, char* line){
  char* s;
  s = line;
  while(1){
    if(!read(file, s, 1))
      return 0;
    if(*s=='\n'){
      *(s+1)='\0';
      break;
    }
    s++;
  }
  return 1;
}
