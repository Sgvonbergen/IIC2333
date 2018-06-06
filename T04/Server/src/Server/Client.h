#pragma once
#include <arpa/inet.h>

typedef struct client {
  int socket_id;
  struct sockaddr_in client_data;
  char* nickname;
  // if false, the data below must be reset for the next game
  int valid_data;
  // total amount of money
  int pot;
  // current money being bet on the round
  int current_bet;
  // array of 5 cards.
  int* cards;
} Client;

Client client_init(int socket_id, struct sockaddr_in client_data);

//void ask_for_nickname(Client c);

// Resets all data on client for a new game
//void start_game(Client c);

//void ask_for_card_change(Client c);

//void create_and_send_hand(Client c);

void client_close(Client c);
