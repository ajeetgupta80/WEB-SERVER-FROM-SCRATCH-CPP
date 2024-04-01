#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "arguments in wrong order check please..");
    exit(1);
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("error creating socket ");
  }

  int portno = atoi(argv[2]);

  char *ip = argv[1];

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(portno);

  inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

  int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

  if (result == 0) {
    printf("connection success ...\n");
  }

  char *message = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";

  send(sockfd, message, strlen(message), 0);

  char buffer[1024];

  recv(sockfd, buffer, 1024, 0);

  printf("response from ip %s\n", buffer);

  return 0;
}
