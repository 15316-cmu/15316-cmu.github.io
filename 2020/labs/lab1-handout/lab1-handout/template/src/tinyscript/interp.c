#include <stdio.h>

#include "tinyscript/ast.h"
#include "tinyscript/interp.h"
#include "common/safemem.h"

#define malloc(n) safe_malloc(n)
#define calloc(n, s) safe_calloc(n, s)
#define free(p) safe_free(p)

//
// TODO: Evaluate arithmetic expresions by their semantics
// Arguments:
//  state_t *s: current interpreter state
//  aexp *a: the expression to evaluate
//  int *v: final value should be stored here
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code interp_aexp(state_t *s, aexp *a, int *v) {
  ret_code rv;

  switch (a->optype) {
    case CONST:
      break;
    case IDENT:
      break;
    case PLUS:
      break;
    case MINUS:
      break;
    case TIMES:
      break;
    case APAREN:
      break;
    default:
      rv = -1;
      break;
  }

  return rv;
}

//
// TODO: Evaluate Boolean expresions by their semantics
// Arguments:
//  state_t *s: current interpreter state
//  bexp *b: the expression to evaluate
//  bool *v: final value should be stored here
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code interp_bexp(state_t *s, bexp *b, bool *v) {

  ret_code rv;

  switch (b->optype) {
    case CTRUE:
      break;
    case CFALSE:
      break;
    case NOT:
      break;
    case AND:
      break;
    case OR:
      break;
    case EQ:
      break;
    case LEQ:
      break;
    case HASDEF:
      break;
    case BPAREN:
      break;
    default:
      rv = -1;
      break;
  }

  return rv;
}

//
// TODO: Implement assignment commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  char *var: target variable
//  aexp *a: right-hand side of the assignment
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_asgn(state_t *s, char *var, aexp *a) {

  return 0;
}

//
// TODO: Implement composition commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  com *c1: first command in composition
//  com *c2: second command in composition
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_comp(state_t *s, com *c1, com *c2) {

  return 0;
}

//
// TODO: Implement conditional commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  bexp *b: test condition
//  com *c1: 'then' branch command
//  com *c2: 'else' branch command
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_cond(state_t *s, bexp *b, com *c1, com *c2) {

  return 0;
}

//
// TODO: Implement while commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  bexp *b: test condition
//  com *c: loop body
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_while(state_t *s, bexp *b, com *c) {

  return 0;
}

//
// TODO: Implement undef commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  char *var: variable to undefine
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_undef(state_t *s, char *var) {

  // Implement undef commands according to the semantics

  return 0;
}

//
// TODO: Implement output commands according to the semantics
// Arguments:
//  state_t *s: current interpreter state
//  aexp *a: expression to evaluate and output
// Any changes to the state should be reflected in *s after
// the command is done interpreting.
//
// Should return:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
ret_code do_output(state_t *s, aexp *a) {
  
  return 0;
}

//
// Interprets commands in the current state, updating the
// state as required by the language semantics.
// Arguments:
//  state_t *s: current interpreter state
//  com *c: the command to interpret
//
// Returns:
//  - 0 on success
//  - positive value for early termination without error
//  - negative value if error is encountered
//
int __attribute__ ((noinline)) interp_com(state_t *s, com *c) {
  int rval;

  switch (c->comtype) {
    case SKIP:
      // do nothing, return success code
      rval = 0;
      break;
    case ASGN:
      rval = do_asgn(s, c->data.asgn->var, c->data.asgn->rhs);
      break;
    case COMP:
      rval = do_comp(s, c->data.compose->c1, c->data.compose->c2);
      break;
    case COND:
      rval = do_cond(s,
                     c->data.conditional->b,
                     c->data.conditional->c1,
                     c->data.conditional->c2);
      break;
    case WHILE:
      rval = do_while(s, c->data.whilec->b, c->data.whilec->c);
      break;
    case UNDEF:
      rval = do_undef(s, c->data.undef->var);
      break;
    case OUTP:
      rval = do_output(s, c->data.output->a);
      break;
    default:
      rval = -1;
      break;
  }

  return rval;
}

//
// Initializes a new program state by reading in
// a database file and creating an extendible hash
// data structure to hold the mappings.
// Arguments:
//  char *dbfile: the database filename; if it does not
//    currently exist, then an empty hash table is
//    created and the corresponding file is initialized.
//
// Returns a pointer to the initialized state.
//
state_t *init_state(char *dbfile) {
  int fd = Open(dbfile, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
  state_t *state = hash_table_deserialize(fd);
  Close(fd);
  return state;
}

//
// Persists a program state in a database file on disk.
// Arguments:
//  state_t *state: the state to persist
//  char *dbfile: the database filename; it will be
//    truncated and overwritten if it already exists
//
// Returns void
//
void store_state(state_t *state, char * dbfile) {
  int fd = Open(dbfile, O_WRONLY | O_TRUNC, 0);
  hash_table_serialize(state, fd);
  Close(fd);
}