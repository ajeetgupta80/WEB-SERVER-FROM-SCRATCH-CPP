#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
  // creating a socket
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("webserver socket");
    return 1;
  }
  printf("socket created successfully\n");

  // creating address to bind with socket
  struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
  if (bind(s, &addr, sizeof(addr)) != 0) {
    perror("webserver (bind)");
    return 1;
  }
  // listen for incoming connections
  if (listen(s, 10) != 0) {
    perror("webserver (listen)");
    return 1;
  }
  printf("server listening for connections\n");
  while (1) {

    int client_fd = accept(s, NULL, NULL);
    if (client_fd < 0) {
      perror("webserver (accept)");
      continue;
    }
    printf("connection accepted\n");
    char buffer[BUFFER_SIZE] = {0};
    recv(client_fd, buffer, BUFFER_SIZE, 0);

    char *f = buffer + 5;
    *strchr(f, ' ') = 0;

    const char *message =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><head><title>ABHAY PATEL IS A "
        "GENIUS!</title></head>"
        "<body><h1>you heared it right!</h1><br><h1>Abhay "
        "Patel is the next big thing</h1></body></html>"
        "<br><h1>Abhay Patel is the next big thing</h1></body></html>"
        "<br><h1>Abhay Patel is the next big thing</h1></body></html>";
    send(client_fd, message, strlen(message), 0);

    close(client_fd);
  }

  close(s);
  return 0;
}
