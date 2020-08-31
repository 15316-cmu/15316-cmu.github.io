/*
 * divide.c - a minimal CGI program that divides two numbers
 */
#include "csapp.h"

int main(int argc, char **argv) {
  if (argc < 2) exit(1);
  char msg[MAXLINE], content[MAXLINE];
  int n1=1, n2=1;

  /* Extract the two arguments */
  if (sscanf(argv[1], "num=%d&div=%d\n", &n1, &n2) != 2) {
    sprintf(msg, "Can't parse query string '%s'", argv[1]);
  } else if (n2 == 0) {
    sprintf(msg, "Attempt to divide %d by zero", n1);
  } else {
    sprintf(msg, "%d / %d -> %d, remainder %d\n", n1, n2, n1/n2, n1%n2);
  }
  /* Make the response body */
  sprintf(content, "Welcome to divide.com:<p>%s<p>Bye!\n", msg);
  /* Generate the HTTP response */
  printf("Content-length: %d\r\n", (int) strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);
  exit(0);
}

