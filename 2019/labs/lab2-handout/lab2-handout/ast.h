
#ifndef _AST_H
#define _AST_H

#define CONST	0
#define IDENT	1
#define PLUS	2
#define MINUS	3
#define TIMES	4
#define NOT		5
#define AND		6
#define OR		7
#define EQ		8
#define LEQ		9
#define HASDEF	10
#define CTRUE	11
#define CFALSE	12
#define SKIP	13
#define ASGN	14
#define COMP	15
#define COND	16
#define WHILE	17
#define CALL	18
#define UNDEF	19
#define EXIT	20
#define OUTP	21
#define APAREN	22
#define BPAREN	23

/********* ARITHMETIC EXPRESSIONS *********/

// The struct aexp specifies AST nodes for arithmetic expressions.
//
// If optype is CONST or IDENT, then both operand fields are NULL
// and the value union holds either the numeric constant or identifier
// string.
//
// If optype is PLUS, MINUS, or TIMES then both operand fields point
// to aexp structs for the subexpressions.
struct aexp {
	int optype;			// operation type (CONST, IDENT, PLUS, MINUS, TIMES)
	union {
		int intval;		// holds integer constant
		char *id;		// holds identifier
	} value;			// in case optype is CONST or IDENT
	struct aexp *op1;	// first operand
	struct aexp *op2;	// second operand
};
typedef struct aexp aexp;

aexp *new_const(int val);
aexp *new_ident(char *id);
aexp *new_plus(aexp *op1, aexp *op2);
aexp *new_minus(aexp *op1, aexp *op2);
aexp *new_times(aexp *op1, aexp *op2);
aexp *new_aparen(aexp *op);

/********* BOOLEAN EXPRESSIONS *********/

// The struct bexp specifies AST nodes for boolean expressions.
//
// If optype is AND, OR, EQ, or LEQ then both operand fields point to
// bexp structs for the subexpressions.
//
// If optype is NOT or HASDEF then the first operand field (op1) points to
// another bexp struct for the subexpression, and the second (op2) is NULL.
//
// If optype is TRUEC or FALSEC then both operand fields are NULL.
struct bexp {
	int optype;			// operation type (CTRUE, CFALSE, NOT, AND, OR, EQ, LEQ, HASDEF)
	union {
		struct aexp *a;	// use when optype is EQ, LEQ, HASDEF
		struct bexp *b; // use when optype is AND, OR, NOT
	} op1;				// first operand
	union {
		struct aexp *a;	// use when optype is EQ, LEQ
		struct bexp *b;	// use when optype is AND, OR
	} op2;				// second operand
};
typedef struct bexp bexp;

bexp *new_true();
bexp *new_false();
bexp *new_not(bexp *op);
bexp *new_and(bexp *op1, bexp *op2);
bexp *new_or(bexp *op1, bexp *op2);
bexp *new_eq(aexp *op1, aexp *op2);
bexp *new_leq(aexp *op1, aexp *op2);
bexp *new_hasdef(aexp *op1);
bexp *new_bparen(bexp *op);

/********* COMMANDS *********/

struct asgn_com {
	char *var;
	struct aexp *rhs;
};
typedef struct asgn_com asgn_com;

struct compose_com {
	struct com *c1;
	struct com *c2;
};
typedef struct compose_com compose_com;

struct conditional_com {
	struct bexp *b;
	struct com *c1;
	struct com *c2;
};
typedef struct conditional_com conditional_com;

struct while_com {
	struct bexp *b;
	struct com *c;
};
typedef struct while_com while_com;

struct call_com {
	struct aexp *id;
	int arity;
	struct aexp **args;
};
typedef struct call_com call_com;

struct undef_com {
	char *var;
};
typedef struct undef_com undef_com;

struct output_com {
	struct aexp *a;
};
typedef struct output_com output_com;

struct com {
	int comtype;
	union {
		struct asgn_com *asgn;
		struct compose_com *compose;
		struct conditional_com *conditional;
		struct while_com *whilec;
		struct call_com *call;
		struct undef_com *undef;
		struct output_com *output;
	} data;
};
typedef struct com com;

com *new_skip();
com *new_asgn(char *var, aexp *rhs);
com *new_compose(com *c1, com *c2);
com *new_conditional(bexp *b, com *c1, com *c2);
com *new_while(bexp *b, com *c);
com *new_call(aexp *id, int arity, aexp *args[]);
com *new_undef(char *var);
com *new_exit();
com *new_output(aexp *a);

/********* PROGRAMS *********/

struct prog {
	char *table;
	char *user;
	char *passwd;
	com *command;
};
typedef struct prog prog;

prog *new_program(char *table, char *user, char *passwd, com *command);

#endif
