#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define MAX_CLIENT 5

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "port no not provided. program terminated\n");
    exit(1);
  }

  int sockfd, newsockfd, portno, n;

  char buffer[BUFFER_SIZE];
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t cli_len;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("error opening socket");
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("binding failed");
  }
  listen(sockfd, MAX_CLIENT);

  cli_len = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

  if (newsockfd < 0) {
    error("Error on Accept function");
  }

  while (1) {
    bzero(buffer, 255);

    n = read(newsockfd, buffer, 255);

    if (n < 0) {
      error("error on reading");
    }

    printf("client : %s\n", buffer);

    bzero(buffer, 255);
   
    fgets(buffer, 255, stdin);

    n = write(newsockfd, buffer, strlen(buffer));

    if (n < 0) {
      error("error on writing");
    }

    int i = strncmp("#", buffer, 1);

    if (i == 0) {
      break;
    }
  }
  close(newsockfd);
  close(sockfd);
  return 0;
}
