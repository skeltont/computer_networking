/*
#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
#
# References:
#	help with sockets and server stuff:
#		http://codebase.eu/tutorial/linux-socket-programming-c/#creating_a_server/
#*******************************************************************************************
#
*/

#include "header_server.h"

using namespace std;

// global, but i think it's reasonable to have this be global.
// The username is very important.
char username[10];

static void *receive_message(void * sock) {
  char client_response[1000];

  for(;;) {
    recv((intptr_t) sock, client_response, 1000, 0);
    if(strlen(client_response) > 0) {
      printf("%s", client_response);
    }
    (void) memset(client_response, 0, sizeof(client_response));
  }
}

static void setup_connection(intptr_t *sock, int *socketfd){
  int yes = 1;
  char chatPort[10];

  struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
  struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
  struct sockaddr_storage their_addr;

  // initialize our structure.
  memset(&host_info, 0, sizeof host_info);
  host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
  host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
  host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

  printf("Please enter a port to open for chat: ");
  cin >> chatPort;

  // Now fill up the host_info struct with our address information.
  if(getaddrinfo(NULL, chatPort, &host_info, &host_info_list) != 0)
    printf("getaddrinfo error");

  // create the socket
  *socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
  if (*socketfd == -1)  printf("error creating socket\n");

  // bind the socket
  setsockopt(*socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if(bind(*socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen) == -1)
    printf("bind error\n");

  printf("waiting for incoming client connection\n");

  // wait for an attempted connection
  if(listen(*socketfd, 5) == -1) printf("error attempting to listen\n");

  socklen_t addr_size;
  addr_size = sizeof(their_addr);
  if ((*sock = accept(*socketfd, (struct sockaddr *) &their_addr, &addr_size)) == -1) {
      printf("error accepting connection\n");
      exit(1);
  }

  printf("connection accepted\n");
}

int main() {
    int socketfd;
    char message[1000], payload[1000];

    ssize_t bytes_received, bytes_sent;
    pthread_t thread;
    intptr_t sock;

    printf("Please enter a username to use for chat: ");
    cin >> username;

    setup_connection(&sock, &socketfd);

    // spin off a thread that is always listening for messages so we can have them
    // print out as they roll in.
    pthread_create(&thread, NULL, &receive_message, (void *) sock);
    pthread_detach(thread);

    for(;;) {
      printf("%s%s", username, PROMPT);
      cin.getline(message,sizeof(message));

      // we don't want to send an empty message
      if(strlen(message) < 1) {
  			continue;
  		}

      if(strcmp(message, QUIT) == 0) {
        printf("Exiting Chat \n");
        break;
      }

      // format our message with the username and prompt so it looks like a
      // chat message
      sprintf(payload, "\r%s%s%s\n", username, PROMPT, message);
      send(sock, payload, strlen(payload), 0);
      (void) memset(message, 0, sizeof(message));
    }

    // perform house keeping
    close(sock);
    close(socketfd);

    return 0;
}
