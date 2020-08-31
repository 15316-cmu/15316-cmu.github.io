#include "common/csapp.h"

int main(int argc, char **argv) {
  char buf[MAXLINE];
  memset(buf, 0, MAXLINE);
  if (argc != 4) {
    fprintf(stderr, "usage: %s <hostname> <port> <file>\n ", argv[0]);
    return 1;
  }
  /* Send all contents of the script, script must end with newline */
  int client_fd = Open_clientfd(argv[1], atoi(argv[2]));
  int file = Open(argv[3], O_RDONLY, 0);
  rio_t rp;
  ssize_t read;
  Rio_readinitb(&rp, file);
  while ((read = Rio_readlineb(&rp, buf, MAXLINE-1)) != 0) {
    if(buf[read-1] != '\n')
      buf[read-1] = '\n';
    Rio_writen(client_fd, buf, read);
    memset(buf, 0, MAXLINE);
  }
  Close(file);
  // Signal end of packet
  Rio_writen(client_fd, "***\n", 5);

  /* Print out server output to the command line */
  Rio_readinitb(&rp, client_fd);
  while (Rio_readlineb(&rp, buf, MAXLINE) != 0) {
    printf("%s", buf);
  }
  Close(client_fd);
  return 0;
}