#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/times.h>
#include "Process/Process.h"

// Declaring Globaly for signal handling
PList* waiting_processes;
PList* done_processes;
PList* failed;
int m, processes_amount;
struct tms start, end;
clock_t s, e;

void stats(
  int total_processes, int max_process_amount,
  struct tms start, struct tms end, clock_t elapsed_time,
  PList* done, PList* failed
  )
{
  Process* p;
  char args[1024];
  printf("Running time for %i commands with %i different concurrent processes available:\n", total_processes, max_process_amount);
  printf("Real CPU time: %li\n", elapsed_time);
  printf("Total concurrent CPU time in User mode: %li\n", (end.tms_cutime + end.tms_utime - start.tms_cutime - start.tms_utime));
  printf("Total concurrent CPU time in Sys mode: %li\n", (end.tms_cstime + end.tms_stime - start.tms_cstime - start.tms_stime));
  printf("Succesful Commands:\n");
  if (done->count == 0) {
    printf("There are no Succesful Commands\n");
  }
  for (size_t i = 0; i < done->count; i++) {
    p = p_list_get(done, i);
    read(p->output_buffer, p->output, BUFSIZ);
    strcpy(args, "");
    if (strcmp(p->output, "") == 0) {
      strcpy(p->output, "NO OUTPUT");
    }
    // Aggregating all args into a single string.
    for (size_t j = 1; j < p->arg_count-1; j++) {
      strcat(args, p->args[j]);
      if (j < p->arg_count - 2)
        strcat(args, ", ");
    }

    printf("Command: %s | Arguments: %s\nExit Code: %i\nOutput:\n%s\n\n", p->cmd, args, p->code, p->output);
  }
  printf("Failed Commands:\n");
  if (failed->count == 0) {
    printf("There are no Failed Commands\n");
  }
  for (size_t i = 0; i < failed->count; i++) {
    p = p_list_get(failed, i);
    // Aggregating all args into a single string.
    strcpy(args, "");
    for (size_t j = 1; j < p->arg_count; j++) {
      printf("%s\n", args);
      strcat(args, p->args[j]);
      if (j < p->arg_count - 2)
        strcat(args, ", ");
    }

    printf("Command: %s | Arguments: %s\nExit Code: %i\nOutput: %s\n\n", p->cmd, args, p->code, p->output);
  }
}

void handler(int sig)
{
  // Checking if we have already read file and started the process
  if (waiting_processes != NULL) {
    // Cheking if m has been assigned. if m != 0, start and s are already assigned
    if (m != 0) {
      e = clock();
      times(&end);
      stats(m, processes_amount, start, end, e-s, done_processes, failed);
    } else {
      printf("INTERRUPTED BEFORE ANYTHING INTERESTING HAPPENED\n");
    }
  } else {
    printf("INTERRUPTED BEFORE ANYTHING INTERESTING HAPPENED\n");
  }
  exit(0);
}

PList* readFile(const char *filename)
{
  PList* waiting_processes = p_list_init();
  // File Reading
  int i;
  char space = ' ';
  char quote = '"';
  char *space_pos, *quote_pos;
  char line[1024], *linePTR, cmd[32], *temp;
  char **args;
  args = malloc(32*sizeof(char*));
  FILE* fp = fopen(filename, "r");
  while (fgets(line, 512, fp)) {
    linePTR = &line;
    // obtaining cmd
    strcpy(cmd, strsep(&linePTR, " "));
    // Unpacking args
    args[0] = cmd;
    i = 1;
    while (1) {
      // Checking for ""
      if (linePTR) {
        space_pos = strchr(linePTR, space);
        quote_pos = strchr(linePTR, quote);
      } else {
        space_pos = NULL;
        quote_pos = NULL;
      }

      if (space_pos && quote_pos) {
        // If the space is before the quote
        if (quote_pos > space_pos) {
          temp = strsep(&linePTR, &space);
          if (temp != NULL && strcmp(temp, "\n") != 0) {
           strtok(temp, "\n");
           args[i] = temp;
           i++;
          } else {
           args[i] = NULL;
           break;
          }
        } else {
          // removing first quote and taking everything until the second quote
          strsep(&linePTR, &quote);
          temp = strsep(&linePTR, &quote);
          if (temp != NULL && strcmp(temp, "\n") != 0) {
           strtok(temp, "\n");
           args[i] = temp;
           i++;
          } else {
           args[i] = NULL;
           break;
          }
          // the next character after a quoted argument is always a space, we skip it.
          linePTR++;
        }
      } else {
        temp = strsep(&linePTR, &space);
        if (temp != NULL && strcmp(temp, "\n") != 0) {
          strtok(temp, "\n");
          // Special case when we reach end of line, only add the arg if it has something in it.
          if (strcmp(temp, "") != 0) {
            args[i] = temp;
            i++;
          }
        } else {
          args[i] = NULL;
          break;
        }
      }
    }
    Process* p = p_init(0, cmd, args, ++i);
    p_list_append(waiting_processes, p);
  }
  free(args);
  free(linePTR);
  fclose(fp);
  return waiting_processes;
}

int main(int argc, char const *argv[]) {
  signal(SIGINT, handler);
  times(&start);
  s = clock();
  if (argc != 3) {
    printf("Incorrect amount of parameters for execution\n Please run as ./doer <file> <n>");
    return 1;
  }
  // Amount of different processes
  processes_amount = atoi(argv[2]);
  int running_processes = 0;

  PList* processes = p_list_init();
  waiting_processes = readFile(argv[1]);
  done_processes = p_list_init();
  failed = p_list_init();

  // m is the total amount of processes to be run.
  m = waiting_processes->count;

  // Running processes
  int child;
  siginfo_t infop;

  // runs while there are waiting processes
  do {
    Process* p = p_list_pop(waiting_processes);

    // Setting up pipe for receiving the output of child processes
    int fd[2];
    pipe(fd);

    child = fork();
    if (child == 0) {
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      execvp(p->cmd, p->args);
    } else if(child > 0) {
      close(fd[1]);
      p->output_buffer = fd[0];
      p->PID = child;
      p_list_append(processes, p);
      running_processes++;
    }

    // Managing max amount of processes
    if (running_processes == processes_amount) {

      // docs on waitid http://www.tutorialspoint.com/unix_system_calls/waitid.htm
      waitid(P_ALL, -1, &infop, WEXITED);

      // Checking which process exited
      for (size_t i = 0; i < processes->count; i++) {

        if (p_list_get(processes, i)->PID == infop.si_pid) {
          // if succesful we put it on done. Otherwise check how many times it has run and put it back on ready/failed.
          if (infop.si_status == 0) { // SUCCESS
            p_list_append(done_processes, p_list_remove(processes, i));
          } else { // Failure, we check and rerun if needed.
            Process* p = p_list_remove(processes, i);
            if (p->repeated == 0) {
              p->repeated++;
              p_list_append(waiting_processes, p);
            } else {
              p->code = infop.si_status;
              p_list_append(failed, p);
            }
          }
          break;
        }
      }
      running_processes--;
    }
  } while(!is_empty(waiting_processes));

  // Waiting on all current running processes
  while (!is_empty(processes)) {
    if (waitid(P_ALL, -1, &infop, WEXITED) == -1) {
      break;
    };
    // Checking which process exited
    for (size_t i = 0; i < processes->count; i++) {
      if (p_list_get(processes, i)->PID == infop.si_pid) {
        // if succesful we put it on done. Otherwise check how many times it has run and put it back on ready/failed.
        if (infop.si_status == 0) {
          p_list_append(done_processes, p_list_remove(processes, i));
        } else {
          Process* p = p_list_remove(processes, i);
          if (p->repeated == 0) {
            p->repeated++;
            // we rerun the process if it fails.
            child = fork();
            if (child == 0) {
              execvp(p->cmd, p->args);
            } else if(child > 0) {
              p->PID = child;
              p_list_append(processes, p);
              running_processes++;
            }
          } else {
            p->code = infop.si_status;
            p_list_append(failed, p);
          }
        }
        break;
      }
    }
    running_processes--;
  }
  e = clock();
  times(&end);
  stats(m, processes_amount, start, end, (e-s), done_processes, failed);
  p_list_destroy(processes);
  p_list_destroy(waiting_processes);
  p_list_destroy(failed);
  p_list_destroy(done_processes);
  // At this point all processes have run and been completed or rerun twice and failed.
  return 0;
}
