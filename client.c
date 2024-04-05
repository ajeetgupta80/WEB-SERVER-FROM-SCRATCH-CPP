#include <netdb.h>
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

  if (argc < 3) {
    fprintf(stderr, "port no not provided. program terminated\n");
    exit(1);
  }

  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[BUFFER_SIZE];

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("error opening socket");
  }

  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "error no such host");
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  // copy n bytes from *server to serv_addr
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

  serv_addr.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("socket failed to connect ");
  }

  while (1) {
    bzero(buffer, 255);

    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
      error("error on writing");
    }
    bzero(buffer, 255);

    n = read(sockfd, buffer, 255);

    if (n < 0) {
      error("error while reading");
    }
    printf("server: %s", buffer);

    int i = strncmp("bye", buffer, 3);
    if (i == 0) {
      break;
    }
  }
  close(sockfd);
  return 0;
}
