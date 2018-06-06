#include "Client.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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
  return *c_ptr;
}

void client_close(Client c)
{
  free(c.cards);
  free(c.nickname);
  // write(c.socket_id, "Client closing");
  close(c.socket_id);
  free(&c);
}
