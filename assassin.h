static void sighandler(int signo);
int server_init();
int check_key(int live, int dead, char* str);
void next_target(int live, int dead, char* line);
int readline(int live, char* line);
int is_dead(int dead, char* str);
void record(int dead, char* str);
void check_win(char* name);
int count_lines(int file);
