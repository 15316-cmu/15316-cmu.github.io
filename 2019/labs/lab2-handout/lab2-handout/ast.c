
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tinyscript/ast.h"
#include "common/safemem.h"

#define malloc(n) safe_malloc(n)
#define calloc(n, s) safe_calloc(n, s)
#define free(p) safe_free(p)

aexp *new_const(int val) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = CONST;
	a->value.intval = val;
	a->op1 = NULL;
	a->op2 = NULL;

	return a;
}

aexp *new_ident(char *id) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = IDENT;
	a->value.id = id;
	a->op1 = NULL;
	a->op2 = NULL;

	return a;
}


aexp *new_plus(aexp *op1, aexp *op2) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = PLUS;
	a->op1 = op1;
	a->op2 = op2;

	return a;
}

aexp *new_minus(aexp *op1, aexp *op2) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = MINUS;
	a->op1 = op1;
	a->op2 = op2;

	return a;
}

aexp *new_times(aexp *op1, aexp *op2) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = TIMES;
	a->op1 = op1;
	a->op2 = op2;

	return a;
}

aexp *new_aparen(aexp *op) {
	aexp *a = (aexp *)malloc(sizeof(aexp));
	a->optype = APAREN;
	a->op1 = op;
	a->op2 = NULL;

	return a;	
}

bexp *new_true() {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = CTRUE;
	b->op1.a = NULL;
	b->op2.a = NULL;

	return b;
}

bexp *new_false() {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = CFALSE;
	b->op1.a = NULL;
	b->op2.a = NULL;

	return b;
}

bexp *new_not(bexp *op) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = NOT;
	b->op1.b = op;
	b->op2.a = NULL;

	return b;
}

bexp *new_and(bexp *op1, bexp *op2) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = AND;
	b->op1.b = op1;
	b->op2.b = op2;

	return b;
}

bexp *new_or(bexp *op1, bexp *op2) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = OR;
	b->op1.b = op1;
	b->op2.b = op2;

	return b;
}

bexp *new_eq(aexp *op1, aexp *op2) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = EQ;
	b->op1.a = op1;
	b->op2.a = op2;

	return b;
}

bexp *new_leq(aexp *op1, aexp *op2) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = LEQ;
	b->op1.a = op1;
	b->op2.a = op2;

	return b;
}

bexp *new_hasdef(aexp *op1) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = HASDEF;
	b->op1.a = op1;
	b->op2.a = NULL;

	return b;	
}

bexp *new_bparen(bexp *op) {
	bexp *b = (bexp *)malloc(sizeof(bexp));
	b->optype = BPAREN;
	b->op1.b = op;
	b->op2.b = NULL;

	return b;	
}

com *new_skip() {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = SKIP;

	return c;	
}

com *new_asgn(char *var, aexp *rhs) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = ASGN;
	c->data.asgn = (asgn_com *)malloc(sizeof(asgn_com));
	c->data.asgn->var = var;
	c->data.asgn->rhs = rhs;

	return c;
}

com *new_compose(com *c1, com *c2) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = COMP;
	c->data.compose = (compose_com *)malloc(sizeof(compose_com));
	c->data.compose->c1 = c1;
	c->data.compose->c2 = c2;

	return c;	
}

com *new_conditional(bexp *b, com *c1, com *c2) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = COND;
	c->data.conditional = (conditional_com *)malloc(sizeof(conditional_com));
	c->data.conditional->b = b;
	c->data.conditional->c1 = c1;
	c->data.conditional->c2 = c2;

	return c;	
}

com *new_while(bexp *b, com *c1) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = WHILE;
	c->data.whilec = (while_com *)malloc(sizeof(while_com));
	c->data.whilec->b = b;
	c->data.whilec->c = c1;

	return c;
}

com *new_call(aexp *id, int arity, aexp *args[]) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = CALL;
	c->data.call = (call_com *)malloc(sizeof(call_com));
	c->data.call->id = id;
	c->data.call->arity = arity;
	c->data.call->args = args;

	return c;
}

com *new_undef(char *var) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = UNDEF;
	c->data.undef = (undef_com *)malloc(sizeof(undef_com));
	c->data.undef->var = var;

	return c;
}

com *new_exit() {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = EXIT;

	return c;
}

com *new_output(aexp *a) {
	com *c = (com *)malloc(sizeof(com));
	c->comtype = OUTP;
	c->data.output = (output_com *)malloc(sizeof(output_com));
	c->data.output->a = a;

	return c;	
}

prog *new_program(char *table, char *user, char *passwd, com *command) {
	prog *p = (prog *)malloc(sizeof(prog));
	p->table = malloc(strlen(table));
	p->user = malloc(strlen(user));
	p->passwd = malloc(strlen(passwd));
	p->command = command;
	strcpy(p->table, table);
	strcpy(p->user, user);
	strcpy(p->passwd, passwd);
	return p;
}