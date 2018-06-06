#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int* card_nums;
char** card_types;
int bet, pot;

void signal_handler(int sig){
  if (sig == SIGINT){
    // do stuff
    exit(0);
  }
}

void show_game_state()
{
  printf("\nYour Current cards:\n");
  printf("------------  ------------  ------------  ------------  ------------\n");
  printf("|%d         |  |%d         |  |%d         |  |%d         |  |%d         |\n", card_nums[0], card_nums[1], card_nums[2], card_nums[3], card_nums[4]);
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|    %s     |  |     %s    |  |     %s    |  |     %s    |  |     %s    |\n", card_types[0], card_types[1], card_types[2], card_types[3], card_types[4]);
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|         %d|  |         %d|  |         %d|  |         %d|  |         %d|\n", card_nums[0], card_nums[1], card_nums[2], card_nums[3], card_nums[4]);
  printf("------------  ------------  ------------  ------------  ------------\n");
  printf("Current bet: %d\nTotal Pot: %d\n", bet, pot);
}

int main(int argc, char* argv[])
{
  signal(SIGINT,signal_handler);
  if (argc != 5) {
    printf("Incorrect parameters, to run the program do:\n$ ./client -i <ip_address> -p <tcp-port>\n");
    return 1;
  }
  int port;
  char* ip;
  for (size_t i = 0; i < 5; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      ip = argv[i+1];
    }
    if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i+1]);
    }
  }

  int client;
  struct sockaddr_in server_address;

  if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Failed to create client socket\n");
    exit(1);
  }

  memset(&server_address, '0', sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_address.sin_addr);

  if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address))) {
    printf("Connection Failed\n");
    exit(1);
  }



  return 0;
}
