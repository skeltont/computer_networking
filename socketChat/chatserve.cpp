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

static void *receive_message(void * new_sd) {
  char client_response[1000];
  for(;;) {
    recv((intptr_t) new_sd, client_response, 1000, 0);
    std::cout << client_response;
    (void) memset(client_response, 0, sizeof(client_response));
  }
}

int main() {

    int status, socketfd, len, yes = 1;
    char chatPort[10], username[10];
    char message[1000], payload[1000];

    ssize_t bytes_received, bytes_sent;
    pthread_t thread;
    intptr_t new_sd;

    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
    struct sockaddr_storage their_addr;

    // initialize our structure.
    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..." << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    std::cout << "Please enter a port to open for chat: ";
    std::cin >> chatPort;

    std::cout << "Please enter a username to use for chat: ";
    std::cin >> username;

    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo(NULL, chatPort, &host_info, &host_info_list);
    if (status != 0) std::cout << "getaddrinfo error" << gai_strerror(status) ;

    std::cout << "Creating a socket..."  << std::endl;

    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (socketfd == -1)  std::cout << "socket error " ;

    std::cout << "Binding socket..."  << std::endl;

    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "bind error" << std::endl ;

    std::cout << "Listen()ing for connections..."  << std::endl;

    status =  listen(socketfd, 5);
    if (status == -1)  std::cout << "listen error" << std::endl ;

    socklen_t addr_size = sizeof(their_addr);
    new_sd = accept(socketfd, (struct sockaddr *) &their_addr, &addr_size);
    if (new_sd == -1) {
        std::cout << "listen error" << std::endl ;
    } else {
        std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
    }


    pthread_create(&thread, NULL, &receive_message, (void *) new_sd);
    pthread_detach(thread);


    for(;;) {
      std::cout << username << PROMPT;
      std::cin.getline(message,sizeof(message));
      if(strlen(message) < 1) {
  			continue;
  		}

      sprintf(payload, "\r%s%s%s\n", username, PROMPT, message);
      send(new_sd, payload, strlen(payload), 0);
      (void) memset(message, 0, sizeof(message));
    }


    std::cout << "Stopping server..." << std::endl;

    // perform house keeping
    freeaddrinfo(host_info_list);
    close(new_sd);
    close(socketfd);

    return 0;
}
