
#ifndef _INTERP_H
#define _INTERP_H

#include "common/extendible_hash.h"
#include "tinyscript/ast.h"

typedef hash_table_t state_t;
typedef int ret_code;

state_t *init_state(char *);
void store_state(state_t *, char *);
int interp_com(state_t *, com *);

#endif