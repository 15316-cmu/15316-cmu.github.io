#include "common/csapp.h"

#define INTERP "../tinyscript/interp"

/**
 * @return Current time in milliseconds since Unix Epoch
 */
unsigned long long current_time_milli() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long) (tv.tv_sec) * 1000 +
      (unsigned long long) (tv.tv_usec) / 1000;
}

/**
 * Log a client connection
 * @param ip the ip of the incoming connection
 * @param log name of the log file to write to
 */
void write_to_log(const char *ip, const char *log) {
  int log_fd = Open(log, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  dprintf(log_fd, "%s %llu\n", ip, current_time_milli());
  Close(log_fd);
}

int main(int argc, char **argv) {
  int listenfd, connfd, port, temp_fd;
  socklen_t clientlen;
  struct sockaddr_in clientaddr;
  /* Check command line args */
  if (argc != 3) {
    fprintf(stderr, "usage: %s <port> <log>\n ", argv[0]);
    return 1;
  }
  port = atoi(argv[1]);
  listenfd = open_listenfd(port);
  while (1) {
    char buf[MAXLINE];
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);

    /* Get ip address in string and perform logging */
    write_to_log(inet_ntoa(clientaddr.sin_addr), argv[2]);

    /* Download client script to a local temp file */
    printf("connection received, downloading script file to temp.tiny\n");
    temp_fd = Open("temp.tiny", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    rio_t rp;
    rio_readinitb(&rp, connfd);
    while (1) {
      ssize_t size = rio_readlineb(&rp, buf, MAXLINE);
      if (strncmp("***", buf, 3) == 0) break;
      Rio_writen(temp_fd, buf, size);
    }
    Close(temp_fd);
    printf("script downloaded to temp.tiny\n");

    /* Fork a process and invoke the interpreter on temp file */
    char *arglist[3] = {INTERP, "temp.tiny", (char *) 0};
    printf("exec'ing %s %s\n", INTERP, "temp.tiny");
    if (Fork() == 0) {
      printf("invoking interpreter");
      Dup2(connfd, STDOUT_FILENO);
      Execve(INTERP, arglist, environ);
    }
    Wait(NULL);
    printf(
        "interpreter exited, removing temporary files and closing connection...\n");
    unlink("temp.tiny");
    Close(connfd);
  }
}
