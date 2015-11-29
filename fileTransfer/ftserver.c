/*
#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
#
# References:
#   Directory listing in C:
#     http://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
#*******************************************************************************************
#
*/

#include "header_server.h"

static void setup_control_connection(char *port, intptr_t *sd, int *c){
  int yes = 1;
  char chatPort[10];
  struct sockaddr_in server;
  socklen_t addr_size;

  printf("> Opening connection on port #%s\n", port);

  *sd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sd == -1) {
		printf("> FAILED to create socket.\n");
    return;
	}
	printf("> Successfully created socket.\n");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(port));

  // Bind
  if(bind(*sd,(struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("> FAILED to bind to ip\n");
    return;
  }
  printf("> Successfully bound to an available ip.\n");

  //Accept and incoming connection
  printf("> Listening for incoming connections...\n");

  if(listen(*sd, 5) == -1) printf("> FAILED attempting to listen\n");
  *c = sizeof(struct sockaddr_in);

}

static void setup_data_connection(intptr_t *sock, char *p_value) {
  struct sockaddr_in server;

  // Attempt to create the socket
  if((*sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    printf("> FAILED Could not create socket");

  printf("> Data socket created\n");

  // server.sin_addr.s_addr = inet_addr(i_value);
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(p_value));

  //Connect to remote server
  if (connect(*sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
      perror("> FAILED connect failed. Error");
      exit(1);
  }

  printf("> Connected to data socket\n");
}

char *listDirectory() {
  FILE *fp;
  char buf[5000];
  char resp[5000];
  (void) memset(resp, 0, sizeof(resp));
  char *response = resp;
  fp = popen("ls *", "r");

  while (fgets(buf, sizeof(buf), fp) != NULL)
    strcat(resp, buf);
  pclose(fp);

  return response;
}

char *sendFile(char *filename, size_t size) {
  int gfd, flag, num_read;
  char buf[5000];
  char resp[5000];
  (void) memset(resp, 0, sizeof(resp));
  char *response = resp;

  flag = O_RDONLY;
  gfd = open(filename, flag);
  printf("> Filename requested: \"%s\"\n", filename);
  while((num_read = read(gfd, &buf, sizeof(buf))) != 0)
    strcat(resp, buf);

  printf("> Finished sending file. \n");

  (void) close(gfd);

  return response;
}


bool validateArgs(char *p_value){
  // Make sure they specify a port #
  if (p_value == NULL) {
    printf("\n Usage: \n");
    printf("\t my_socket_server -p <port #> \n\n");
    return false;
  }
  return true;
}

int main(int argc , char **argv) {
    int o, c, data_socket_created = 0, command_not_set = 0, command_need_file = 0;
    char *buffer, *p_value, *invalid_message = "File not found\n";
    char message[1000], port[10], command[100], filename[100];
    intptr_t sd, csd, dsd;
    struct sockaddr_in client;

    char *response;

    // Handle options supplied
    while ((o = getopt (argc, argv, "p:")) != -1) {
      switch (o) {
        case 'p':
          p_value = optarg;
          break;
        case '?':
          return 1;
      default:
        abort();
      }
    }

    if (!validateArgs(p_value)) exit(1);

    setup_control_connection(p_value, &sd, &c);
    printf("> Control connection accepted\n");

    while ((csd = accept(sd, (struct sockaddr *)&client, (socklen_t*)&c))) {
      recv(csd, message, 1000, 0);
      if(strlen(message) > 0) {
        if (data_socket_created == 0) {
          printf("> Data socket port received\n");
          // create data connection

          buffer = strtok(message, ";");
          while (buffer) {
            if (data_socket_created == 0) {
              strcpy(port, buffer);
              data_socket_created = 1;
            } else if (command_not_set == 0) {
              strcpy(command, buffer);
              command_not_set = 1;
              command_need_file = 1;
            } else if(command_need_file == 1) {
              strcpy(filename, buffer);
            }
            buffer = strtok(NULL, ";");
          }
          setup_data_connection(&dsd, port);
          printf("> Data socket created for transfer.\n");
          if(strcmp(command, CMD_LIST) == 0) {
            printf("> LIST command received, sending response on data socket.\n");
            // call LIST handler
            response = listDirectory();
            write(dsd, response, strlen(response));
          } else if(strncmp(command, CMD_GET, strlen(CMD_GET)) == 0) {
            printf("> GET command received, sending response on data socket.\n");
            // call GET handler
            response = sendFile(filename, sizeof(filename));
            write(dsd, response, strlen(response));
          } else {
            printf("> Invalid command received, sending response on control socket.\n");
            // let client know they sent an invalid command
            write(csd, invalid_message, strlen(invalid_message));

          }
        }
        // cleanup
        data_socket_created = 0;
        command_not_set = 0;
        command_need_file = 0;
        close(dsd);
      }
    }
    return 0;
}
