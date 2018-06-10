#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void signal_handler(int sig){
  if (sig == SIGINT){
    // do stuff
    exit(0);
  }
}

unsigned char* card_to_char(int card_num){
  unsigned char* card;
  if (card_num == 1){
      card = "A";
  }
  if (card_num < 11 && 1 < card_num){
    if(card_num == 10){card = malloc(sizeof(unsigned char)*2);}
    else { card = malloc(sizeof(unsigned char));}
    snprintf (card, sizeof(card), "%d",card_num);
  }
  if (card_num == 11){
      card = "J";
  }
  if (card_num == 12){
      card = "Q";
  }
  if (card_num == 13){
      card = "K";
  }
  return card;
}

void unicode_print(int card_type) {
  if(card_type == 1){
    printf("|    \u2665     |  ");
  }
  if(card_type == 2){
    printf("|    \u2666     |  ");
  }
  if(card_type == 3){
    printf("|    \u2663     |  ");
  }
  if(card_type == 4){
    printf("|    \u2660     |  ");
  }
}

void show_cards(int* card_nums, int* card_types)
{
  printf("------------  ------------  ------------  ------------  ------------\n");
  printf("|%2s        |  |%2s        |  |%2s        |  |%2s        |  |%2s        |\n",
  card_to_char(card_nums[0]), card_to_char(card_nums[1]), card_to_char(card_nums[2]), card_to_char(card_nums[3]), card_to_char(card_nums[4]));
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|          |  |          |  |          |  |          |  |          |\n");
  for(int i = 0; i < 5; i++){
    unicode_print(card_types[i]);
  }
  printf("\n");
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|          |  |          |  |          |  |          |  |          |\n");
  printf("|        %2s|  |        %2s|  |        %2s|  |        %2s|  |        %2s|\n",
  card_to_char(card_nums[0]), card_to_char(card_nums[1]), card_to_char(card_nums[2]), card_to_char(card_nums[3]), card_to_char(card_nums[4]));
  printf("------------  ------------  ------------  ------------  ------------\n");
  printf("     (1)           (2)           (3)           (4)           (5)       \n");
}

void send_message(int client, int messageid, int payload_size, unsigned char* payload){
  unsigned char* header;
  header = malloc(sizeof(unsigned char)*2);
  header[0] = (char)messageid;
  header[1] = (char)payload_size;
  send(client, header, 2, 0);
  if (payload_size > 0){
    send(client, payload, payload_size, 0);
    }
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



  int* card_nums;
  int* card_types;
  int bet, pot;
  card_nums = malloc(sizeof(int)*5);
  card_types = malloc(sizeof(int)*5);

  printf("****** Welcome to 5-Card Draw Poker ******\n\n");
  printf("Establishing connection with Server\n");
  unsigned char* p;
  send_message(client, 1, 0, p);
  while (1) {
    unsigned char * header;
    unsigned char* payload;
    header = malloc(2);
    recv(client, header, 2, 0);
    int payload_size = (int)header[1];
    if(payload_size > 0){
      payload = malloc(payload_size);
      recv(client, payload, payload_size, 0);
    }
    int message_id = (int)header[0];
    //manipulacion previa del payload en casos especiales
    if (message_id == 6 || message_id == 8 || message_id == 21){
      if (payload_size > 1){
        pot = (int)payload[1];
        int temp = (int)payload[0];
        temp = temp*255;
        pot += temp;
        }
      else {
        pot = (int)payload[0];
      }
    }
    if (message_id == 10 || message_id == 19){
      int j = 0;
      int k = 0;
      for (int i = 0; i < 10; i++){
        if(i%2 == 0){
          card_nums[j] = (int)payload[i];
          j++;
        }
        else{
          card_types[k] = (int)payload[i];
          k++;
        }
      }
    }
    //manejo del protocolo
    if(message_id == 2){
      printf("Conection Established \n\n");
    }
    if(message_id == 3){
      printf("The game needs your Nickname:\n");
      unsigned char* nickname ;
      nickname = malloc(254);
      gets(nickname);
      printf("Thanks %s !!\n", nickname);
      printf("waiting for an opponent\n");
      send_message(client, 4, 254, nickname);
    }
    if(message_id == 5){
      printf("An opponent was found, his name is: %s\n", payload);
    }
    if(message_id == 6){
      printf("Initial pot: %d\n", pot);
    }
    if(message_id == 7){
      printf("The game has started successfully\n");
    }
    if(message_id == 8){
      printf("A raound has started\n");
      printf("Current pot: %d\n", pot);
    }
    if(message_id == 9){
      bet = (int)payload[0];
      printf("Initial bet: %d\n", bet);
    }
    if(message_id == 10){
      printf("Your cards:\n");
      show_cards(card_nums, card_types);
    }
    if(message_id == 11){
      int turn = (int)payload[0];
      if(turn == 1){
        printf("You are the first to bet\n");
      }
      else {
        printf("You are the second to bet\n");
      }
    }
    if(message_id == 12){
      printf("How many cards do you want to change (0 to 5)\n");
      int n;
      scanf ("%d",&n);
      int* to_change = malloc(sizeof(int)*n);
      for(int c = 0; c < n; c++){
        printf("position of the card to change:\n");
        scanf ("%d",&to_change[c]);
      }
      unsigned char* buffer = malloc(n*2);
      for(int i = 0; i < n; i++){
        buffer[i*2] = (char)card_nums[to_change[i]-1];
        buffer[i*2 + 1] = (char)card_types[to_change[i]-1];
      }
      send_message(client, 13, n*2, buffer);
    }
    if(message_id == 14){
      printf("Available bets: \n");
      for(int j = 0; j < payload_size; j++){
        if ((int)payload[j]==1){
          printf("%d) FOLD\n", j+1);
        }
        if ((int)payload[j]==2){
          printf("%d) 0\n", j+1);
        }
        if ((int)payload[j]==3){
          printf("%d) 100\n", j+1);
        }
        if ((int)payload[j]==4){
          printf("%d) 200\n", j+1);
        }
        if ((int)payload[j]==5){
          printf("%d) 500\n", j+1);
        }
      }
      printf("Select the number of the bet \n");
      int option;
      scanf ("%d",&option);
      unsigned char* select = malloc(1);
      select[0] = (char)payload[option-1];
      send_message(client, 15, 1, select);
    }

    if (message_id == 16){
      printf("we are zorry, your bet is invalid\n");
    }
    if (message_id == 17){
      printf("The bet has been accept\n");
    }
    if (message_id == 18){
      printf("The raound has finished\n");
    }
    if (message_id == 19){
      printf("Cards of your opponent:\n" );
      show_cards(card_nums, card_types);
    }
    if (message_id == 20){
      int winner = (int)payload[0];
      if (winner == 1){
        printf("Congrats!! You have win \n");
      }
      else{
        printf("Zorry. You lose\n");
      }
    }
    if (message_id == 21){
      printf("your new pot: %d\n", pot);
    }
    if (message_id == 22){
      printf("Game Over\n");
    }
    if (message_id < 1 || message_id > 24){
      unsigned char* placeholder;
      send_message(client, 24, 0, placeholder);

    }
  }


  return 0;
}
