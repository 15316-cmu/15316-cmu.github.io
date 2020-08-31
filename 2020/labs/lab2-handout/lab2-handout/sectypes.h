#ifndef _SECTYPES_H
#define _SECTYPES_H

#include "common/extendible_hash.h"
#include "tinyscript/ast.h"
struct sec_label {
    char *name; // human-readable name associated with label
};
typedef struct sec_label sec_label;

struct sec_lattice {
    sec_label* user_label;
    ubarray* uba;     // ubarray of sec_label* for pub, users, and admin
};
typedef struct sec_lattice sec_lattice;

struct sec_ctxt {
    sec_label* pc;    // label for the pc
    hash_table_t* ht; // maps variable ids (char *) to indices in the lattice uba
};
typedef struct sec_ctxt sec_ctxt;

bool sec_lessthan(sec_lattice *L, sec_label *l1, sec_label *l2);
sec_label *sec_lub(sec_lattice *L, sec_label *l1, sec_label *l2);
sec_label *type_aexp(sec_ctxt *G, sec_lattice *L, aexp *a);
sec_label *type_bexp(sec_ctxt *G, sec_lattice *L, bexp *b);
bool typecheck_com(sec_ctxt *G, sec_lattice *L, sec_label *lu, com *c);

#endif