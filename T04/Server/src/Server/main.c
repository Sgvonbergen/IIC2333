#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int MAX_WAITING_CONNECTIONS = 5;

void signal_handler(int sig){
  if (sig == SIGINT){
    // close all connections before exiting
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  signal(SIGINT,signal_handler);
  if (argc != 5) {
    printf("Incorrect parameters, to run the program do:\n$ ./server -i <ip_address> -p <tcp-port>\n");
    return 1;
  }

  int port;
  char *ip;
  for (size_t i = 0; i < 5; i++) {
    if (strcmp(argv[i], "-i")) {
      ip = argv[i+1];
    }
    if (strcmp(argv[i], "-p")) {
      port = atoi(argv[i+1]);
    }
  }

  // Creating Listener Socket for receiving new connection attempts
  struct sockaddr_in server;
  int listener = -1;
  do {
    listener = socket(AF_INET, SOCK_STREAM, 0);
  } while(listener != -1);

  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_family = AF_INET;
  server.sin_port= htons(port);

  // Attempting to bind the listener to the designated port
  int bind_success = -1;
  do {
    bind_success = bind(listener, (struct sockaddr*)&server, sizeof(server));
  } while(bind_success != 0);
  listen(listener, MAX_WAITING_CONNECTIONS);

  return 0;
}
