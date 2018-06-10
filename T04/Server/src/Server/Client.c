#include "Client.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

unsigned int rangerand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}

void receive_header(Client c, unsigned char * header, int packet_id)
{
  int i = 0;
  recv(c.socket_id, header, 2, 0);
  while (header[0] != (char)packet_id) {
    if (i >= 5) {
      free(header);
      raise(SIGINT);
    }
    header[0] = 24u;
    header[1] = 0u;
    send(c.socket_id, header, 2, 0);
    recv(c.socket_id, header, 2, 0);
    i++;
  }
}

Client client_init(int socket_id, struct sockaddr_in client_data)
{
  Client *c_ptr = malloc(sizeof(Client));
  struct sockaddr_in* data = malloc(sizeof(struct sockaddr_in));
  memcpy(&client_data, data, sizeof(struct sockaddr_in));
  c_ptr->client_data = *data;
  c_ptr->socket_id = socket_id;
  c_ptr->valid_data = 0;
  c_ptr->cards = malloc(sizeof(int) * 5);
  c_ptr->pot = 1000;
  return *c_ptr;
}

void ask_for_nickname(Client c)
{
  unsigned char *header = malloc(2);
  header[0] = 3u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void receive_start_connection(Client c)
{
  unsigned char *packet = malloc(2);
  recv(c.socket_id, packet, 2, 0);
  int i = 0;
  while (packet[0] != 1u) {
    if (i >= 5) {
      free(packet);
      raise(SIGINT);
    }

    packet[0] = 24u;
    packet[1] = 0u;
    send(c.socket_id, packet, 2, 0);
    recv(c.socket_id, packet, 2, 0);
    i++;
  }
  free(packet);
}

void receive_nickname(Client c)
{
  unsigned char *header = malloc(2);
  recv(c.socket_id, header, 2, 0);
  int i = 0;
  while (header[0] != 4u) {
    if (i >= 5) {
      free(header);
      raise(SIGINT);
    }
    header[0] = 24u;
    header[1] = 0u;
    send(c.socket_id, header, 2, 0);
    recv(c.socket_id, header, 2, 0);
    i++;
  }

  int length = (int)header[1];
  c.nickname = malloc(length);
  recv(c.socket_id, &c.nickname, length, 0);
  c.nickname_length = length;
  free(header);
}

void opponent_found(Client c1, Client c2)
{
  unsigned char * header = malloc(2);
  header[0] = 5u;
  header[1] = (char)c2.nickname_length;
  send(c1.socket_id, header, 2, 0);
  send(c1.socket_id, c2.nickname, c2.nickname_length, 0);
  printf("%s %d\n", c2.nickname, c2.nickname_length);
  free(header);
}

void send_initial_pot(Client c)
{
  int pot_length = 1;
  int initial_pot = 1000;
  while (initial_pot > 255) {
    pot_length++;
    initial_pot = initial_pot/255;
  }
  unsigned char * header = malloc(2);
  header[0] = 6u;
  header[1] = pot_length;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(pot_length);
  initial_pot = 1000;
  while (initial_pot > 255) {
    payload[pot_length - 1] = initial_pot%255;
    initial_pot = initial_pot/255;
    pot_length--;
  }
  send(c.socket_id, payload, header[1], 0);
  free(header);
  free(payload);
}

void game_start(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 7u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void send_connection_established(Client c)
{
  unsigned char *header = malloc(2);
  header[0] = 2u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void send_inital_bet(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 9u;
  header[1] = 1u;
  send(c.socket_id, header, 2, 0);

  unsigned char * payload = malloc(1);
  payload[0] = 10u;
  send(c.socket_id, payload, 1, 0);
  free(header);
  free(payload);
}

void send_cards(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 10u;
  header[1] = 10u;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(10);
  for (size_t i = 0; i < 10; i++) {
    if (i%2 == 0) {
      payload[i] = c.cards[i/2];
    } else {
      payload[i] = c.types[i/2];
    }
  }
  send(c.socket_id, payload, 10, 0);
  free(header);
  free(payload);
}

void send_first(Client c, unsigned int first)
{
  unsigned char * header = malloc(2);
  header[0] = 11u;
  header[1] = 1;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(1);
  payload[0] = (unsigned char)first;
  send(c.socket_id, payload, 1, 0);
  free(header);
  free(payload);
}

void ask_cards_to_change(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 12u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void get_cards_to_change(Client c)
{
  unsigned char * header = malloc(2);
  recv(c.socket_id, header, 2, 0);
  int i = 0;
  while (header[0] != 13u) {
    if (i >= 5) {
      free(header);
      raise(SIGINT);
    }
    header[0] = 24u;
    header[1] = 0u;
    send(c.socket_id, header, 2, 0);
    recv(c.socket_id, header, 2, 0);
    i++;
  }
  if (header[1] > 0) {
    unsigned char * payload = malloc((int)header[1]);
    recv(c.socket_id, payload, (int)header[1], 0);
    int * cards_to_change = malloc((int)header[1]/2 * sizeof(int));

    // Selecciono una carta. payload es de la forma carta.tipo.carta.tipo...
    // Busco en las cartas del cliente cuales son iguales y recuerdo(cards_to_change) la posicion en el arreglo de cartas.
    for (size_t i = 0; i < (int)header[1]/2; i++) {
      int card_num = (int)payload[i*2];
      int card_type = (int)payload[i*2 + 1];
      for (size_t j = 0; j < 5; j++) {
        if (c.cards[j] == card_num && c.types[j] == card_type) {
          cards_to_change[i] = j;
        }
      }
    }

    for (size_t i = 0; i < (int)header[1]/2; i++) {
      c.cards[cards_to_change[i]] = rangerand(1, 13);
      c.types[cards_to_change[i]] = rangerand(1, 4);
    }
    free(cards_to_change);
    free(payload);
  }
  free(header);
}

void ask_bet(Client c, unsigned min_bet)
{
  int bets = 1;
  if (min_bet == 0) {
    bets = 4;
  } else if (min_bet == 100) {
    bets = 3;
  } else if (min_bet == 200) {
    bets = 2;
  } else if (min_bet == 500) {
    bets = 1;
  }
  unsigned char * header = malloc(2);
  header[0] = 14u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(1 + bets);
  payload[0] = 1;
  int j = 1;
  for (size_t i = bets; i > 0; i--) {
    payload[j] = 5-bets + 1;
    j++;
  }
  send(c.socket_id, payload, bets + 1, 0);
  free(header);
  free(payload);
}

void get_bet(Client c)
{
  unsigned char * header = malloc(2);
  receive_header(c, header, 15);
  unsigned char * payload = malloc(1);
  recv(c.socket_id, payload, 1, 0);
  if ((int)payload[0] == 1) {
    c.current_bet = -1;
  } else if ((int)payload[0] == 2) {
    c.current_bet = 0;
  } else if ((int)payload[0] == 3) {
    c.current_bet = 100;
  } else if ((int)payload[0] == 4) {
    c.current_bet = 200;
  } else if ((int)payload[0] == 5) {
    c.current_bet = 500;
  }
  free(header);
  free(payload);
}

void wrong_bet(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 16u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void ok_bet(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 17u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void end_round(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 18u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void send_opponent_cards(Client c1, Client c2)
{
  unsigned char * header = malloc(2);
  header[0] = 19u;
  header[1] = 10u;
  send(c1.socket_id, header, 2, 0);

  unsigned char * payload = malloc(10);
  for (size_t i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      payload[i] = c2.cards[i/2];
    } else {
      payload[i] = c2.types[i/2];
    }
  }
  send(c1.socket_id, payload, 10, 0);
  free(header);
  free(payload);
}

void send_winner(Client c, unsigned int result)
{
  unsigned char * header = malloc(2);
  header[0] = 20u;
  header[1] = 1u;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(1);
  payload[0] = (char)result;
  send(c.socket_id, payload, 1, 0);
  free(header);
  free(payload);
}

void update_pot(Client c)
{
  int pot_length = 1;
  int initial_pot = c.pot;
  while (initial_pot > 255) {
    pot_length++;
    initial_pot = initial_pot/255;
  }
  unsigned char * header = malloc(2);
  header[0] = 6u;
  header[1] = pot_length;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(pot_length);
  initial_pot = c.pot;
  while (initial_pot > 255) {
    payload[pot_length - 1] = initial_pot%255;
    initial_pot = initial_pot/255;
    pot_length--;
  }
  send(c.socket_id, payload, header[1], 0);
  free(header);
  free(payload);
}

void send_game_end(Client c)
{
  unsigned char * header = malloc(2);
  header[0] = 22u;
  header[1] = 0u;
  send(c.socket_id, header, 2, 0);
  free(header);
}

void send_image(Client c, FILE* image) {
  unsigned int file_size;
  fseek(image, 0, SEEK_END);
  file_size = ftell(image);
  fseek(image, 0, SEEK_SET);

  unsigned char * header = malloc(2);
  header[0] = 23u;
  header[1] = (char)file_size;
  send(c.socket_id, header, 2, 0);
  unsigned char * payload = malloc(file_size);
  fread(payload, 1, file_size, image);
  send(c.socket_id, payload, file_size, 0);

  free(payload);
  free(header);
}

void client_close(Client c)
{
  free(c.cards);
  free(c.types);
  free(c.nickname);
  free(&c.client_data);
  // write(c.socket_id, "Client closing");
  close(c.socket_id);
  free(&c);
}
