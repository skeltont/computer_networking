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


int main() {

    int status, socketfd, new_sd, len, yes = 1;
    char chatPort[10];
    char incomming_data_buffer[1000];

    ssize_t bytes_received, bytes_sent;

    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
    struct sockaddr_storage their_addr;
    char *msg = "thank you.";

    // initialize our structure.
    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..." << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    std::cout << "Please enter a port to open for chat: ";
    std::cin >> chatPort;

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
    new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_sd == -1) {
        std::cout << "listen error" << std::endl ;
    } else {
        std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
    }

    std::cout << "Waiting to recieve data..."  << std::endl;

    while (1) {
      bytes_received = recv(new_sd, incomming_data_buffer,1000, 0);
      // If no data arrives, the program will just wait here until some data arrives.
      if (bytes_received == 0) std::cout << "host shut down." << std::endl ;
      if (bytes_received == -1) std::cout << "recieve error!" << std::endl ;
      std::cout << bytes_received << " bytes recieved :" << std::endl ;
      incomming_data_buffer[bytes_received] = '\0';
      std::cout << incomming_data_buffer << std::endl;


      std::cout << "send()ing back a message..."  << std::endl;
      len = strlen(msg);
      bytes_sent = send(new_sd, msg, len, 0);
    }


    std::cout << "Stopping server..." << std::endl;

    // perform house keeping
    freeaddrinfo(host_info_list);
    close(new_sd);
    close(socketfd);

    return 0;
}
