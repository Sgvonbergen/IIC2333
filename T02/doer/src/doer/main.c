#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Process/Process.h"

int main(int argc, char const *argv[]) {
  if (argc != 3) {
    printf("Incorrect amount of parameters for execution\n Please run as ./doer <file> <n>");
    return 1;
  }
  // Amount of different processes
  int processes_amount = atoi(argv[2]);
  int running_processes = 0;
  siginfo_t *infop;

  int i, res;
  long int file_pos;
  char line[1024], *linePTR, cmd[32], *temp;
  char **args;
  args = malloc(sizeof(char*));
  FILE* fp = fopen(argv[1], "r");
  while (fgets(line, 512, fp)) {
    linePTR = &line;
    // obtaining cmd
    strcpy(cmd, strsep(&linePTR, " "));
    // Unpacking args
    args[0] = cmd;
    i = 1;
    while (1) {
       temp = strsep(&linePTR, " ");
       if (temp != NULL) {
         args = realloc(args, (i+1)*sizeof(char*));
         args[i] = temp;
         i++;
       } else {
         break;
       }
    }
    args = realloc(args, (i+1)*sizeof(char*));
    args[++i] = NULL;
    Process* p = p_init(0, cmd, args);
    /*
    // if process creation fails, tries again until it doesn't.
    int child;
    do {
      child = fork();
      if (child == 0) {
        execvp(cmd, args);
      } else if(child > 0) {
        running_processes++;
      }
    } while(child < 0);

    if (running_processes == processes_amount) {
      // we save current pos and close the file.
      file_pos = ftell(fp);
      fclose(fp);


      // docs on waitid http://www.tutorialspoint.com/unix_system_calls/waitid.htm
      waitid(P_ALL, -1, infop, WEXITED);

      running_processes--;
      // when we can create another process we open the file and go back to reading.
      fp = fopen(argv[1], "r");
      fseek(fp, file_pos, SEEK_SET);
    }
  */
  }
  return 0;
}
