#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>

int main(){
  char* s;
  s = getenv("QUERY_STRING");

    printf("Content-type: text/plain\n\n");
    printf("%s", s);
  return 0;
}
