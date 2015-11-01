#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include <pthread.h>   	/* for threading , link with lpthread */
#include <stdio.h>      // for printf and stuff
#include <cstdlib>    // because i like c better than c++

# define PROMPT 		"> "
# define QUIT   "/quit"
