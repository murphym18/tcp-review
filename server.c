#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#include "common.h"

// must give a buffer with space for 9 chars: "HH:MM:SS\0"
void read_time(char *out) {
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  int hour = timeinfo->tm_hour % 12;
  int min = timeinfo->tm_min;
  int sec = timeinfo->tm_sec;

  sprintf(out, "%02d:%02d:%02d", hour, min, sec);
}

int main (int argc, char *argv[]) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  
  struct sockaddr_in server_addr, client_addr;
  bzero((uint8_t *) &server_addr, sizeof(server_addr));
  int port = read_port("server_port.txt");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    error("ERROR on binding");
  
  listen(sockfd,5);

  socklen_t client_len = sizeof(client_addr);
  int newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
  if (newsockfd < 0) 
    error("ERROR on accept");

  int n;
  // char buffer[256];
  // bzero(buffer, 256);
  // n = read(newsockfd, buffer, 255);
  // if (n < 0)
  //   error("ERROR reading from socket");
  // printf("Here is the message: %s\n", buffer);

  char t[9];
  read_time(t);
  n = write(newsockfd, t, 9);
  if (n < 0)
    error("ERROR writing to socket");

  close(newsockfd);
  close(sockfd);
  return 0;
}