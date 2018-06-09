#pragma once
#include <arpa/inet.h>
#include <stdio.h>

typedef struct client {
  int socket_id;
  struct sockaddr_in client_data;
  unsigned char* nickname;
  unsigned int nickname_length;
  // if false, the data below must be reset for the next game
  int valid_data;
  // total amount of money
  unsigned int pot;
  // current money being bet on the round
  int current_bet;
  // array of 5 cards.
  unsigned int* cards;
  unsigned int* types;
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

void send_first(Client c, unsigned int is_first);

void ask_cards_to_change(Client c);

void get_cards_to_change(Client c);

void ask_bet(Client c, unsigned int min_bet);

void get_bet(Client c);

void wrong_bet(Client c);

void ok_bet(Client c);

void end_round(Client c);

void send_opponent_cards(Client c1, Client c2);

void send_winner(Client c, unsigned int result);

void update_pot(Client c);

void send_game_end(Client c);

void send_image(Client c, FILE* image);

void client_close(Client c);
