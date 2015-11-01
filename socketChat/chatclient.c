/*
#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
#
# References:
#	help with sockets and server stuff:
#		http://www.binarytides.com/server-client-example-c-sockets-linux/
#*******************************************************************************************
#
*/
#include "header_client.h"

void *receive_message(sock) {
  char server_reply[1000];
  for(;;) {
    if(recv(sock, server_reply, 5000 ,0) < 0) {
        puts("recv failed");
    } else {
        printf("\n%s", server_reply);
    }
  }
}

int main(int argc , char *argv[]) {
  int i, o, gfd, pfd, flags, mode, num_read, rm;
  intptr_t sock;
  struct sockaddr_in server;
  pthread_t thread;
  char username[10], message[1000], payload[5000];

  // command line argument variables
	char* p_value = NULL;
	char* i_value = NULL;
	FILE *fp;

  // GET OPTIONS
  while ((o = getopt (argc, argv, "p:i:")) != -1) {
    switch (o) {
      case 'p':
        p_value = optarg;
        break;
      case 'i':
        i_value = optarg;
        break;
      case '?':
        return 1;
    default:
      abort();
    }
  }

  // Make sure they specify a port # and an ip address.
	if ((p_value == NULL) && (i_value == NULL)) {
		printf("\n Usage: \n");
		printf("\t my_socket_client -p <port #> -i <ip address>\n\n");
		exit(1);
	}

  // Attempt to create the socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
      printf("Could not create socket");
  }
  printf("Socket created\n");

  server.sin_addr.s_addr = inet_addr(i_value);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(p_value));

  //Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
      perror("connect failed. Error");
      return 1;
  }

  printf("Connected\n");

  printf("Enter username (up to 10 characters): ");
  gets(username);

  pthread_create(&thread, NULL, receive_message, (void *) sock);
  pthread_detach(thread);

  for(;;) {
    printf("%s%s",username,PROMPT);

    gets(message);
    sprintf(payload,"%s%s%s\n", username, PROMPT, message);

		if(strlen(message) < 1) {
			printf("You need to enter in a command, plz. \n");
			continue;
		}

    if(strcmp(message, CMD_EXIT) == 0) {
      printf("Exiting Chat \n");
    }

    if(send(sock, payload, strlen(payload), 0) < 0) {
      puts("Sending message failed");
      return 1;
    }
  }

  close(sock);
}
