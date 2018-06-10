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
unsigned int min_bet = 0;
Array* clients;

void signal_handler(int sig){
  if (sig == SIGINT){
    Array_destroy(clients);
    exit(0);
  }
}

int compare_function(const void *a,const void *b)
{
  int *x = (int *) a;
  int *y = (int *) b;
  return *x - *y;
}

int is_royal_flush(Client c)
{
  int type = c.types[0];
  for (size_t i = 1; i < 5; i++) {
    if (c.types[i] != type) {
      return 0;
    }
  }
  int* numbers = malloc(5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  if (numbers[0] != 10) {
    return 0;
  }
  for (size_t i = 0; i < 4; i++) {
    if (numbers[i+1] - numbers[i] != 1) {
      return 0;
    }
  }
  return 1;
}

int is_straight_flush(Client c, int* def)
{
  int type = c.types[0];
  for (size_t i = 1; i < 5; i++) {
    if (c.types[i] != type) {
      return 0;
    }
  }
  int* numbers = malloc(5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  for (size_t i = 0; i < 4; i++) {
    if (numbers[i+1] - numbers[i] != 1) {
      free(numbers);
      return 0;
    }
  }
  *def = numbers[0];
  free(numbers);
  return 1;
}

int is_four(Client c, int* def)
{
  int* numbers = malloc(sizeof(int) * 5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  if (numbers[0] == numbers[3] || numbers[1] == numbers[4]) {
    *def = numbers[2];
    free(numbers);
    return 1;
  }
  return 0;
}

int is_full(Client c, int* def)
{
  int* numbers = malloc(sizeof(int) * 5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  if ((numbers[0] == numbers[1] && numbers[1] == numbers[2] && numbers[3] == numbers[4]) || (numbers[0] == numbers[1] && numbers[2] == numbers[3] && numbers[3] == numbers[4])) {
    *def = numbers[0] + numbers[5];
    free(numbers);
    return 1;
  }
  return 0;
}

int is_flush(Client c)
{
  for (size_t i = 0; i < 4; i++) {
    if (c.types[i] != c.types[i+1]) {
      return 0;
    }
  }
  return 1;
}

int is_straight(Client c, int* def)
{
  int* numbers = malloc(5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  for (size_t i = 0; i < 4; i++) {
    if (numbers[i+1] - numbers[i] != 1) {
      free(numbers);
      return 0;
    }
  }
  *def = numbers[0];
  free(numbers);
  return 1;
}

int is_trio(Client c, int *def)
{
  int* numbers = malloc(5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  *def = numbers[2];
  if (numbers[2] == numbers[1] && numbers[2] == numbers[0]) {
    free(numbers);
    return 1;
  } else if (numbers[2] == numbers[1] && numbers[2] == numbers[3]) {
    free(numbers);
    return 1;
  } else if (numbers[2] == numbers[3] && numbers[2] == numbers[4]) {
    free(numbers);
    return 1;
  }
  return 0;
}

int is_two_pair(Client c, int *def)
{
  int p1;
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 5; j++) {
      if (i != j && c.cards[i] == c.cards[j]) {
        p1 = c.cards[i];
      }
    }
  }
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 5; j++) {
      if (i != j && c.cards[i] == c.cards[j] && c.cards[i] != p1) {
        if (p1 > c.cards[i]) {
          *def = p1;
        } else {
          *def = c.cards[i];
        }
        return 1;
      }
    }
  }
  return 0;
}

int is_pair(Client c, int * def)
{
  int max_pair = -1;
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 5; j++) {
      if (i != j && c.cards[i] == c.cards[j] && c.cards[i] > max_pair) {
        max_pair = c.cards[i];
      }
    }
  }
  if (max_pair != -1) {
    *def = max_pair;
    return 1;
  }
  return 0;
}

int highest(Client c, int* def)
{
  int* numbers = malloc(5);
  memcpy(numbers, c.cards, 5);
  qsort(numbers, 5, sizeof(int), compare_function);
  *def = numbers[4];
  free(numbers);
  return 1;
}

int find_winner(Client c1, Client c2)
{
  int p1, p2, def1, def2;
  if (is_royal_flush(c1)) {
    p1 = 10;
  } else if (is_straight_flush(c1, &def1)) {
    p1 = 9;
  } else if (is_four(c1, &def1)) {
    p1 = 8;
  } else if (is_full(c1, &def1)) {
    p1 = 7;
  } else if (is_flush(c1)) {
    p1 = 6;
  } else if (is_straight(c1, &def1)) {
    p1 = 5;
  } else if (is_trio(c1, &def1)) {
    p1 = 4;
  } else if (is_two_pair(c1, &def1)) {
    p1 = 3;
  } else if (is_pair(c1, &def1)) {
    p1 = 2;
  } else if (highest(c1, &def1)) {
    p1 = 1;
  } else {
    p1 = 0;
  }
  if (is_royal_flush(c2)) {
    p2 = 10;
  } else if (is_straight_flush(c2, &def2)) {
    p2 = 9;
  } else if (is_four(c2, &def2)) {
    p2 = 8;
  } else if (is_full(c2, &def2)) {
    p2 = 7;
  } else if (is_flush(c2)) {
    p2 = 6;
  } else if (is_straight(c2, &def2)) {
    p2 = 5;
  } else if (is_trio(c2, &def2)) {
    p2 = 4;
  } else if (is_two_pair(c2, &def2)) {
    p2 = 3;
  } else if (is_pair(c2, &def2)) {
    p2 = 2;
  } else if (highest(c2, &def2)) {
    p2 = 1;
  } else {
    p2 = 0;
  }
  if (p1 > p2 || (p1 == p2 && def1 >= def2)) {
    return 0;
  } else if (p2 > p1 || (p1 == p2 && def2 > def1)) {
    return 1;
  } else {
    return 0;
  }
}

unsigned int rrand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}

void change_cards(Client c)
{
  ask_cards_to_change(c);
  get_cards_to_change(c);
  send_cards(c);
}

void bet(Client c)
{
  ask_bet(c, min_bet);
  get_bet(c);
  min_bet = c.current_bet;
  while (c.current_bet > c.pot) {
    wrong_bet(c);
    ask_bet(c, min_bet);
    get_bet(c);
    min_bet = c.current_bet;
  }
  ok_bet(c);
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

  Client * players = malloc(sizeof(Client) * 2);
  while (c1.pot > 10 && c2.pot > 10)
  {
    // if first == 0, c1 is first. if first == 1, c2 is first.
    unsigned int first = rrand(0, 1);
    c1.cards = malloc(5*sizeof(unsigned int));
    c2.cards = malloc(5*sizeof(unsigned int));
    c1.types = malloc(5*sizeof(unsigned int));
    c2.types = malloc(5*sizeof(unsigned int));
    for (size_t i = 0; i < 5; i++) {
      c1.cards[i] = rrand(1, 13);
      c2.cards[i] = rrand(1, 13);
      c1.types[i] = rrand(1, 4);
      c2.types[i] = rrand(1, 4);
    }
    send_inital_bet(c1);
    c1.current_bet = 10;
    send_inital_bet(c2);
    c2.current_bet = 10;
    send_cards(c1);
    send_cards(c2);
    if (first) {
      players[0] = c2;
      players[1] = c1;
    } else {
      players[0] = c1;
      players[1] = c2;
    }
    send_first(players[0], 1);
    send_first(players[1], 2);
    change_cards(players[0]);
    change_cards(players[1]);
    bet(players[0]);
    bet(players[1]);
    if (players[1].current_bet != -1 && players[0].current_bet < players[1].current_bet) {
      bet(players[0]);
    }
    end_round(players[0]);
    end_round(players[1]);
    send_opponent_cards(players[0], players[1]);
    send_opponent_cards(players[1], players[0]);
    int winner = find_winner(players[0], players[1]);
    if (winner) {
      send_winner(players[1], 1);
      send_winner(players[0], 2);
      players[1].pot += players[0].current_bet;
      players[0].pot -= players[0].current_bet;
    } else {
      send_winner(players[0], 1);
      send_winner(players[1], 2);
      players[0].pot += players[1].current_bet;
      players[1].pot -= players[1].current_bet;
    }
    update_pot(players[0]);
    update_pot(players[1]);
  }

  send_game_end(c1);
  send_game_end(c2);

  FILE * winner = fopen("winner.jpg", "r");
  FILE * loser = fopen("loser.jpg", "r");

  if (c1.pot > c2.pot) {
    send_winner(c1, 1);
    send_winner(c2, 2);
    send_image(c1, winner);
    send_image(c2, loser);
  } else {
    send_winner(c1, 2);
    send_winner(c2, 1);
    send_image(c2, winner);
    send_image(c1, loser);
  }
  fclose(winner);
  fclose(loser);
  Array_destroy(clients);
  free(players);
  printf("Thank you for playing. The server will close.\n");
  return 0;
}
