/*
#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
#*******************************************************************************************
#
*/

#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <signal.h>
#include <sys/stat.h>		/* things like S_IRUSR and such I believe */
#include <sys/wait.h>		/* for the wait constants and such */
#include <stdio.h>
#include <fcntl.h>			/* for things like O_WRONLY and O_RDONLY */
#include <sys/types.h>  /* Type definitions used by many programs */
#include <dirent.h>     /* for scanning directory */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <sys/socket.h> /* sockets yo */
#include <arpa/inet.h> 	/* inet_addr */
#include <pthread.h>   	/* for threading , link with lpthread */
#include <stdbool.h>

# define CMD_LIST	"-l"
# define CMD_GET  "-g"
