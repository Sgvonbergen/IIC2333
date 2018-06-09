#include "Client.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

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
  unsigned char * nickname = malloc(length);
  recv(c.socket_id, nickname, length, 0);
  c.nickname = nickname;
  c.nickname_length = length;
  free(nickname);
  free(header);
}

void opponent_found(Client c1, Client c2)
{
  unsigned char * header = malloc(2);
  header[0] = 5u;
  header[1] = (char)c2.nickname_length;
  send(c1.socket_id, header, 2, 0);
  send(c1.socket_id, c2.nickname, c2.nickname_length, 0);
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

void send_cards(Client c,)
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

void client_close(Client c)
{
  free(c.cards);
  free(c.nickname);
  // write(c.socket_id, "Client closing");
  close(c.socket_id);
  free(&c);
}
