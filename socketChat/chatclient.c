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

char username[10];

static void *receive_message(sock) {
  char server_reply[1000];

  for(;;) {
    recv(sock, server_reply, 1000 ,0);
    if(strlen(server_reply) > 0) {
      printf("%s", server_reply);
    }
    (void) memset(server_reply, 0, sizeof(server_reply));
  }
}

static void setup_connection(intptr_t *sock, char *i_value, char *p_value) {
  struct sockaddr_in server;

  // Attempt to create the socket
  if((*sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    printf("Could not create socket");

  printf("Socket created\n");

  server.sin_addr.s_addr = inet_addr(i_value);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(p_value));

  //Connect to remote server
  if (connect(*sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
      perror("connect failed. Error");
      exit(1);
  }

  printf("Connected\n");
}


int main(int argc , char *argv[]) {
  int i, o, gfd, pfd, flags, mode, num_read, rm;
  char message[1000], payload[5000];

  intptr_t sock;
  pthread_t thread;

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

  printf("Enter username: ");
  gets(username);

  setup_connection(&sock, i_value, p_value);

  // spin off a thread that is always listening for messages so we can have them
  // print out as they roll in.
  pthread_create(&thread, NULL, receive_message, (void *) sock);
  pthread_detach(thread);

  for(;;) {
    printf("%s%s",username,PROMPT);

    // user input
    gets(message);

    // format our message with the username and prompt so it looks like a
    // chat message
    sprintf(payload,"\r%s%s%s\n", username, PROMPT, message);

    // we don't want to send an empty message
		if(strlen(message) < 1) {
			continue;
		}

    if(strcmp(message, QUIT) == 0) {
      printf("Exiting Chat \n");
      break;
    }

    if(send(sock, payload, strlen(payload), 0) < 0) {
      puts("Sending message failed");
      return 1;
    }

		(void) memset(message, 0, sizeof(message));
  }

  // close socket
  close(sock);
}
