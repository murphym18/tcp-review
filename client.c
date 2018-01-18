#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <time.h>

#include "common.h"

int main (int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage %s hostname\n", argv[0]);
    exit(0);
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int port = read_port("server_port.txt");

  struct hostent *server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");

  char buffer[256];
  int n = read(sockfd, buffer, 255);
  close(sockfd);
  if (n < 0) 
    error("ERROR reading from socket");
  printf("%s\n",buffer);
  return 0;
}