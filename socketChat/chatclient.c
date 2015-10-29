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

int main(int argc , char *argv[]) {
  int i, sock, o, gfd, pfd, flags, mode, num_read;
  struct sockaddr_in server;
  char username[11], message[1000], server_reply[5000], payload[5000];

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

  for(;;) {
    printf("%s%s",username,PROMPT);

    // TODO: needs to send the username with the prompt
    gets(message);

		if(strlen(message) < 1) {
			printf("You need to enter in a command, plz. \n");
			continue;
		}

    if(strcmp(message, CMD_EXIT) == 0) {
      printf("Exiting Chat \n");
    }

    if(send(sock, message, strlen(message), 0) < 0) {
      puts("Sending message failed");
      return 1;
    }

    if(recv(sock, server_reply, 5000 ,0) < 0) {
        puts("recv failed");
        break;
    }

    printf("%s", server_reply);
  }

  close(sock);
}
