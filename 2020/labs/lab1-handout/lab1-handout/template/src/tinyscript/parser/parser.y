%{
  #include <stdio.h>
  #include "tinyscript/ast.h"
  #include "common/csapp.h"
  #include "common/safemem.h"

  #define malloc(n) safe_malloc(n)
  #define calloc(n, s) safe_calloc(n, s)
  #define free(p) safe_free(p)

  int yylex();
  void yyerror(prog **main_prog, const char *s) { printf("ERROR: %s\n", s); }

  int line_num = 1;
%}

%parse-param {prog **main_prog}

%union {
	aexp *AExp;
	bexp *BExp;
	com *Com;
	prog *Prog;
	int ival;
	char *sval;
}

%token TSEMICOLON TCOLON
%token TASSIGN
%token TIF TTHEN TELSE TFI
%token TWHILE TDO TDONE
%token TSKIP
%token TUNDEF
%token THASDEF
%token TEXIT
%token TOUTPUT
%token TTRUE TFALSE
%token TUSING
%token <ival> CONST_I
%token <sval> TVAR

%left TBOOL_EQ TBOOL_LEQ
%left TNOT TAND TOR
%left TADD TSUB
%left TMUL TDIV TMOD
%left TSEMICOLON
%left TLPAREN TRPAREN

%type <AExp> arith_exp
%type <BExp> bool_exp
%type <Com> command
%type <Prog> program

%start program

%%

program 	:	TUSING TVAR TCOLON command { $$ = new_program($2, $4); *main_prog = $$; }
			;

command		:	TSKIP { $$ = new_skip(); }
			|	TVAR TASSIGN arith_exp { $$ = new_asgn($1, $3); }
			|	command TSEMICOLON command { $$ = new_compose($1, $3); }
			|	TIF bool_exp TTHEN command TELSE command TFI { $$ = new_conditional($2, $4, $6); }
			|	TWHILE bool_exp TDO command TDONE { $$ = new_while($2, $4); }
			|	TUNDEF TLPAREN TVAR TRPAREN { $$ = new_undef($3); }
			|	TEXIT { $$ = new_exit(); }
			|	TOUTPUT arith_exp { $$ = new_output($2); }
			;

arith_exp	:	CONST_I { $$ = new_const($1); }
			|	TVAR { $$ = new_ident($1); }
			|	arith_exp TADD arith_exp { $$ = new_plus($1, $3); }
			|	arith_exp TSUB arith_exp { $$ = new_minus($1, $3); }
			|	arith_exp TMUL arith_exp { $$ = new_times($1, $3); }
			|	TLPAREN arith_exp TRPAREN { $$ = new_aparen($2); }
			;

bool_exp	:	TTRUE { $$ = new_true(); }
			|	TFALSE { $$ = new_false(); }
			|	TNOT bool_exp { $$ = new_not($2); }
			|	bool_exp TAND bool_exp { $$ = new_and($1, $3); }
			|	bool_exp TOR bool_exp { $$ = new_or($1, $3); }
			|	arith_exp TBOOL_EQ arith_exp { $$ = new_eq($1, $3); }
			|	arith_exp TBOOL_LEQ arith_exp { $$ = new_leq($1, $3); }
			|	THASDEF TLPAREN TVAR TRPAREN { $$ = new_hasdef(new_ident($3)); }
			|	TLPAREN bool_exp TRPAREN { $$ = new_bparen($2); }
			;
