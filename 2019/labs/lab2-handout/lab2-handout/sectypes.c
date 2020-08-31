#include "tinyscript/sectypes.h"
#include <stdbool.h>
#include <stdlib.h>

bool sec_lessthan(sec_lattice *L, sec_label *l1, sec_label *l2) {
    // should implement the partial order relation to return true
    // if and only if l1 is less than l2
    // in the lattice defined by L, and false otherwise
   
    // l1 = pub or l2 = admin
    if ((strcmp(l1->name,"pub") == 0) || strcmp(l2->name, "admin") == 0) return true;

    // l1 = admin and l2 = pub
    if (strcmp(l1->name,"admin") == 0 && strcmp(l2->name,"pub") == 0) return false;
   
    // otherwise, neither l1 nor l2 is admin/pub
    return (strcmp(l1->name, l2->name) == 0); 
}

sec_label *sec_lub(sec_lattice *L, sec_label *l1, sec_label *l2) {
    // should implement the least-upper-bound operation to return
    // the smallest element of L that is at least as large as both
    // l1 and l2

    // both are pub -> return pub
    if (strcmp(l1->name,"pub") == 0 && strcmp(l2->name,"pub") == 0) return l1;

    // either is admin -> return admin
    if (strcmp(l1->name,"admin") == 0) return l1;
    if (strcmp(l2->name,"admin") == 0) return l2;

    // if one is pub, then the other is a user, so return the user
    if (strcmp(l1->name,"pub") == 0) return l2;
    if (strcmp(l2->name,"pub") == 0) return l1;

    // return admin if the user names aren't equal
    if (strcmp(l1->name, l2->name) == 0) {
        return l1;
    } else {
        return (sec_label*)(*ubarray_elem(L->uba, 1));
    }   
}

sec_label *type_aexp(sec_ctxt *G, sec_lattice *L, aexp *a) {
    // should implement type inference using rules for arithmetic
    // expressions to return a label reflecting the greatest label
    // of information that the given expression could carry under context G
    
}

sec_label *type_bexp(sec_ctxt *G, sec_lattice *L, bexp *b) {
    // should implement type inference using rules for boolean
    // expressions to return a label reflecting the greatest label of
    // information that the given expression could carry under context G
    
}

bool typecheck_com(sec_ctxt *G, sec_lattice *L, sec_label *lu, com *c) {
    // should implement type checking for commands to verify that
    // the script will not leak information to label lu in violation
    // of the lattice policy L in type context G
    
}


