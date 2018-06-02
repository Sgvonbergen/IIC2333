#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Array.h"

int MAX_WAITING_CONNECTIONS = 5;
Array* clients;

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

  // All new clients will be saved on the clients array.
  clients = Array_init();

  // TODO: create a handler thread that does matchmaking between clients.
  // This thread should spawn 1 thread for every match that exists.

  // Accpting new clients, adding them to the available clients.
  int c = sizeof(struct sockaddr_in);
  struct sockaddr_in client;
  int new_client;
  do {
    new_client = accept(listener, (struct sockaddr*)&client, (socklen_t*)&c);
    // TODO: new_client should be a struct that holds relevant data for the matches aswell as socket identifier.
    Array_append(clients, new_client);
    /* TODO: on thread create execute the following communication. after that allow the match handler thread manage everything else.
    1. Start Connection: Cliente envia este paquete al Servidor.
    2. Connection Established: Servidor responde con este paquete luego de recibir el Start Connection del cliente.
    3. Ask Nickname: Servidor env´ıa a cliente este paquete para preguntarle el nickname (nombre) del cliente que se
    acaba de conectar.
    4. Return Nickname: Cliente responde a servidor este paquete con el nickname del cliente
    */
  } while(new_client >= 0);

  return 0;
}
