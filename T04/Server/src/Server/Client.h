#pragma once
#include <arpa/inet.h>

typedef struct client {
  int socket_id;
  struct sockaddr_in client_data;
  unsigned char* nickname;
  int nickname_length;
  // if false, the data below must be reset for the next game
  int valid_data;
  // total amount of money
  int pot;
  // current money being bet on the round
  int current_bet;
  // array of 5 cards.
  int* cards;
  int* types;
} Client;

Client client_init(int socket_id, struct sockaddr_in client_data);

void receive_start_connection(Client c);

void send_connection_established(Client c);

void ask_for_nickname(Client c);

void receive_nickname(Client c);

void opponent_found(Client c1, Client c2);

void send_initial_pot(Client c);

void game_start(Client c);

void send_inital_bet(Client c);

void send_cards(Client c);

void send_first(Client c, int is_first);

void client_close(Client c);
