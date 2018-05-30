#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

void signal_handler(int sig){
  if (sig == SIGINT){
    // do stuff
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  signal(SIGINT,signal_handler);
  if (argc != 5) {
    printf("Incorrect parameters, to run the program do:\n$ ./client -i <ip_address> -p <tcp-port>\n");
    return 1;
  }

  int ip, port;
  for (size_t i = 0; i < 5; i++) {
    if (strcmp(argv[i], "-i")) {
      ip = atoi(argv[i+1]);
    }
    if (strcmp(argv[i], "-p")) {
      port = atoi(argv[i+1]);
    }
  }

  // Attempts to create a socket until it is succesful
  int client_socket = -1;
  do {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
  } while(client_socket != -1);

  printf("%s\n", "hihi");
  return 0;
}
