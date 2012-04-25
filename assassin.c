#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include "assassin.h"
#include "shuffle.h"

#define READ 0
#define WRITE 1


int fromClient, live, dead, numplayers, check_auth;

int main(){
 
  printf("Content-type: text/plain\n\n");
 
  // printf("%d\n", already_playing());
  fflush(stdout);

   umask(0000);
 
   
   //    fromClient = server_init();
     //   printf("after init\n");
    
 
     
   char* buffer, *ass, *victim, *next;
   ass = (char*)malloc(128);
   victim = (char*)malloc(128);
   next = (char*)malloc(128);
   memset(ass, 0, 128);
   memset(victim, 0, 128);
   memset(next, 0, 128);
   //  buffer = (char*)malloc(256);
   // memset(buffer, 0, 256);
  
   //attempt to open new file, fail if exists
   live = open("live.txt", O_RDWR | O_CREAT | O_EXCL, 0664);
   
  //if failed, file exists. open.
  if(live==-1){
    live = open("live.txt", O_RDWR, 0664);
    lseek(live, 0, SEEK_END);
  }
  
  
  dead = open("dead.txt", O_RDWR | O_CREAT | O_EXCL, 0664);
  if(dead==-1){
    dead = open("dead.txt", O_RDWR, 0664);
    lseek(dead, 0, SEEK_END);
  }

    buffer = getenv("QUERY_STRING");
    printf("buffer: %s\n", buffer);
    fflush(stdout);

    
    ass = strsep(&buffer, "&");
    strsep(&ass, "=");
    strsep(&victim, "=");
    
  
       strcpy(victim, buffer);
     strcat(ass, "\n");
     strcat(victim, "\n");
     
      printf("**ass: %s\nvictim: %s\n", ass, victim);
    fflush(stdout);
  
 printf("...");
    fflush(stdout);
  if(!already_playing()){
    pre_game(buffer, ass, victim, live);
  }
  else{//playing
    //0 = auth success; -1 = auth failure
    check_auth =  auth(live, dead, ass, victim, next);
    if(check_auth==0){

    }
  }
  return 0;
 
  
 
 
  /*   
  while(1){
    memset(buffer, 0, 100);
 
    if(read(fromClient, buffer, 100)){
        printf("Message: %d'%s'\n", strlen(buffer), buffer);
      
      if(strchr(buffer, '.')){
	printf("Invalid string.\n");
      }
      else{//buffer contains two things separated by '.'

	//name of user
	char user[50];
	
	//buffer now holds only name of the input target
        strcpy(user, strsep(&buffer, "."));
	
	//will hold appropriate target
	char tar[50];
	memset(tar, 0, 50);

	int check;
	check = auth(live, dead, user, tar);
	if(check==-1){
	  printf("Invalid String.\n");
	}
	if(check==1){
	  printf("No match.\n");
	}
	if(check==-2){
	  printf("You're dead.\n");
	}
	if(check==0){//match was found, now check tar against input tar
	  if(strcmp(tar, buffer)==0){//match
	    record(dead, tar);
	    next_target(live, dead, tar);
	    check_win();
	    printf("Kill successfully recorded. Your next target is %s.\n", tar);
	  }
	}
        
	fflush(stdout);
	//	int check;
	check = check_key(live, dead, buffer, KILL);
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
}	*/   
}   


void pre_game(char* buffer, char* ass, char* victim, int live){
 
    //add players
  //   printf("Waiting for players or start command...\n");
  //    fflush(stdout);
  


      if(strcmp(ass, "start+admin")==0){
	printf("Game starting!\n\n");
	printf("shuffling before play\n");
	shuffle(live);
	numplayers = count_lines(live);
	open("playing", O_CREAT | O_EXCL);
      }
     
      else{
	write(live, ass, strlen(ass));
	printf("You have been added to the game!\n");
      }
   
}

//if str is in live, put appropriate target in tar
//handles improperly formatted string
int auth(int live, int dead, char* ass, char* victim, char* next ){
  lseek(live, 0, SEEK_SET);

  char* line;
  line = (char*)malloc(100);

  if(is_dead(dead, ass)){
    printf("You are dead.\n");
    return -2;
  }
  while(readline(live, line)){
    if(strcmp(ass, line)==0){
      next_target(live, dead, next);
      return 0;
    }
  }
  return -1;
}

/*******
  //returns 0 upon success, -1 (or any non-zero number) upon failure
  //success = properly formatted string
int check_key(int live, int dead, char* str, int mode){
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

  if(mode==KILL){
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
	
	check_win(str);
	printf("Kill successfully recorded. Your next target is %s.\n", line);
	//    check_win(str);
	return 0; 
      }
    }
    return 1;
  }//if KILL
  if(mode==AUTH){
    while(readline(live, line)){
      if(strcmp(str, line)==0){
	next_target(live, dead, line);
      }
    }
    return 1;
  }
}
*****/
int already_playing(){
  if(open("playing", O_CREAT | O_EXCL, 0664)==-1){
    return 1;
  }
  remove("playing");
  return 0;
}

void check_win(){
  int numdead;
  numdead = count_lines(dead);
  // printf("check win: %d %d\n", numplayers, numdead);
  if(numplayers-1==numdead){
    char name[50];
 
    //winner is the only target remaining
      next_target(live, dead, name); 
  
    printf("GAME OVER!!! WINNER: %s\n", name);
    remove("dead");
    remove("playing");
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
      (*strchr(line, '+')) = '\0';
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
