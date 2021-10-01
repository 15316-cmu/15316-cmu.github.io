/*************************************************************************
*
* malloc, calloc, and free compatible with region-based SFI.
* All memory managed by this library is confined to static array mempool,
* which is in the sandbox region of the executable.
*
* The implementation of malloc, calloc, and free is adapted from:
*   The C Programming Language, 2nd Edition
*   Brian Kernighan and Dennis Ritchie
*   Prentice Hall, 1988
*
*************************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "common/safemem.h"

typedef long Align;

union header {
  struct {
    union header *ptr;
    unsigned size;
  } s;
  Align x;
};
typedef union header Header;

static char SANDBOX_SECTION mempool[MAXHEAP];
static unsigned int SANDBOX_SECTION ba = 0;

static Header SANDBOX_SECTION base;
static Header SANDBOX_SECTION *freep = NULL;

static char *safe_sbrk(size_t n) {
	if(ba+n >= MAXHEAP)
		return (char *)-1;
	ba += n;
	return mempool+ba-n;
}

void safe_free(void *ap) {
  Header *bp, *p;

  bp = ((Header *)ap) - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
  	if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; 
  
  if(bp + bp->s.size == p->s.ptr) {
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp) {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header *safe_morecore(size_t nu) {
  char *cp;
  Header *up;

  cp = safe_sbrk(nu * sizeof(Header));
  if (cp == (char *) -1) /* no space at all */
    return NULL;
  up = (Header *) cp;
  up->s.size = nu;
  safe_free((void *)(up+1));

  return freep;
}

void *safe_malloc(size_t nbytes) {
	Header *p, *prevp;
	unsigned int nunits;

	nunits = ((nbytes+sizeof(Header)-1)/sizeof(Header))+1;
	if((prevp = freep) == NULL) {
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}
	for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if(p->s.size >= nunits) {
      if(p->s.size == nunits)
		    prevp->s.ptr = p->s.ptr;
		  else {
		    p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
		  }
		  freep = prevp;
		  return (void *)(p+1);
  	}
  	if(p == freep) {
  		if((p = safe_morecore(nunits)) == NULL)
  			return NULL;
  	}
  }
}

void *safe_calloc(size_t n, size_t size) {
  size_t tot = n*size;
  void *m = safe_malloc(tot);
  if(!m) return NULL;
  return memset(m, 0, tot);
}
