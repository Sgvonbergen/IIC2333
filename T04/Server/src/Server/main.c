#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Array.h"

int MAX_WAITING_CONNECTIONS = 3;
Array* clients;

void signal_handler(int sig){
  if (sig == SIGINT){
    game_end(Array_get(clients, 0));
    game_end(Array_get(clients, 1));
    Array_destroy(clients);
    exit(0);
  }
}

int rrand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
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
    if (strcmp(argv[i], "-i") == 0) {
      ip = argv[i+1];
    }
    if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i+1]);
    }
  }

  // Creating Listener Socket for receiving new connection attempts
  int listener;
  struct sockaddr_in server_address;

  if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("Failed Listener Socket Creation\n");
    exit(1);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port);

  if (bind(listener, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    printf("Failed binding of Listener socket to port %d\n", port);
    exit(1);
  }

  if (listen(listener, 5) < 0) {
    printf("Failed Setting the Listener Socket to listen for new connections\n");
    exit(1);
  }

  // Accepting Client
  int new_client;
  struct sockaddr_in client_address;
  int addrlen = sizeof(client_address);
  clients = Array_init();

  while ((new_client = accept(listener, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) != -1) {
    Client c = client_init(new_client, client_address);
    Array_append(clients, c);
    receive_start_connection(c);
    send_connection_established(c);
    ask_for_nickname(c);
    receive_nickname(c);
    // Start match
    if (clients->count == 2) {
      break;
    }
  }

  Client c1 = Array_get(clients, 0);
  Client c2 = Array_get(clients, 1);

  opponent_found(c1, c2);
  opponent_found(c2, c1);
  send_initial_pot(c1);
  send_initial_pot(c2);
  game_start(c1);
  game_start(c2);

  while (c1.pot > 10 && c2.pot > 10)
  {
    // if first == 0, c1 is first. if first == 1, c2 is first.
    int first = rrand(0, 1);
    c1.cards = malloc(5*sizeof(int));
    c2.cards = malloc(5*sizeof(int));
    c1.types = malloc(5*sizeof(int));
    c2.types = malloc(5*sizeof(int));
    for (size_t i = 0; i < 5; i++) {
      c1.cards[i] = rrand(1, 13);
      c2.cards[i] = rrand(1, 13);
      c1.types[i] = rrand(1, 4);
      c2.types[i] = rrand(1, 4);
    }
    send_inital_bet(c1);
    send_inital_bet(c2);
    send_cards(c1);
    send_cards(c2);
    if (first) {
      send_first(c1, 2);
      send_first(c2, 1);
    } else {
      send_first(c1, 1);
      send_first(c2, 2);
    }
  }

  game_end(Array_get(clients, 0));
  game_end(Array_get(clients, 1));
  Array_destroy(clients);
  printf("Thank you for playing. Server will close.\n");


  return 0;
}
