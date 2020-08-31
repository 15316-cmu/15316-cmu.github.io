%{
#include <stdlib.h>
#include <string.h>
#define TOKEN(t) (yylval.token = t)
int yywrap();
#include "tinyscript/ast.h"
#include "common/csapp.h"
#include "common/safemem.h"
#include "parser.h"

#define malloc(n) safe_malloc(n)
#define calloc(n, s) safe_calloc(n, s)
#define free(p) safe_free(p)

extern int line_num;
%}

%%

[ \t]+ ;
\n { line_num++; }
; { return TSEMICOLON; }
: { return TCOLON; }
using { return TUSING; }
as { return TAS; }
with { return TWITH; }
if { return TIF; }
then { return TTHEN; }
else { return TELSE; }
endif { return TFI; }
while { return TWHILE; }
do { return TDO; }
done { return TDONE; }
skip { return TSKIP; }
undef { return TUNDEF; }
hasdef { return THASDEF; }
exit { return TEXIT; }
output { return TOUTPUT; }
true { return TTRUE; }
false { return TFALSE; }
\== { return TBOOL_EQ; }
\<= { return TBOOL_LEQ; }
\! { return TNOT; }
\|\| { return TOR; }
&& { return TAND; }
\:= { return TASSIGN; }
\+ { return TADD; }
\- { return TSUB; }
\* { return TMUL; }
\( { return TLPAREN; }
\) { return TRPAREN; }
-?[0-9]+ { yylval.ival = atoi(yytext); return CONST_I; }
[a-zA-Z_][a-zA-Z0-9_.]* { yylval.sval = malloc(strlen(yytext)); strcpy(yylval.sval, yytext); return TVAR; }
.

%%

	char pbuf[MAXLINE];
	prog *parse_buf(char *buf, long len) {

	  YY_BUFFER_STATE buffer = yy_scan_buffer(buf, len + 2);
	  prog *program = NULL;
	  yyparse(&program);
	  yy_delete_buffer(buffer);

	  return program;
	}	
