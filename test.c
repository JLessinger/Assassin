#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>

int main(){
  /*  
      char* s;
         s = getenv("QUERY_STRING");
    // if(s!=NULL)
    //  printf("'%s'\n", s);
  
     printf("<html>\n
<head>\n
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n
<title>Untitled Document</title>\n
</head>\n
<body>\n
%s\n
<form name=\"input\" action=\"http://localhost:8888/Volumes/Macintosh%20HD/Users/jonathan/Desktop/assassin/cgi-bin/test.cgi\" method=\"get\">\n
Username: <input type=\"text\" name=\"user\" />\n
<input type=\"submit\" value=\"Submit\" />\n
</form>\n
</body>\n
</html>\n", s);
    */

    printf("Content-type: text/plain\n\n");
    printf("ran\n");
  return 0;
}
