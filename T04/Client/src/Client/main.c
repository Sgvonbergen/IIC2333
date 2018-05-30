#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void signal_handler(int sig){
  if (sig == SIGINT){
    // do stuff
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  signal(SIGINT,signal_handler);
  if (argc != 2) {
    printf("Incorrect parameters, to run the program do:\n/czfx <disk>\nwhere <disk> is the path to the file that simulates the virtual disk.\n");
    return 1;
  }
  printf("%s\n", "hihi");
  return 0;
}
