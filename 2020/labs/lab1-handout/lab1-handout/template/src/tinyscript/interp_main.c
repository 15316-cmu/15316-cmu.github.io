#include <stdio.h>

#include "tinyscript/ast.h"
#include "tinyscript/interp.h"
#include "common/csapp.h"
#include "common/safemem.h"

int yywrap() { return 1; }

extern char pbuf[MAXLINE];
prog *parse_buf(char *buf, long len);

prog SANDBOX_SECTION *program;
state_t SANDBOX_SECTION *s;

int main(int argc, char *argv[]) {
  FILE *fp;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <file>\n", argv[0]);
    exit(1);
  }

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error: could not open %s for reading\n", argv[1]);
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  long fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if (fileSize > MAXLINE - 2) fileSize = MAXLINE - 2;

  fread(pbuf, fileSize, sizeof(char), fp);
  fclose(fp);

  pbuf[fileSize] = '\0';
  pbuf[fileSize + 1] = '\0';

  program = parse_buf(pbuf, fileSize);
  s = init_state(program->table);
  ret_code rv = interp_com(s, program->command);
  if (rv < 0)
    printf("ERROR: aborted script execution\n");
  else if (rv > 0)
    printf("ERROR: script terminated early due to insufficient resources\n");
  else
    store_state(s, program->table);
}