
#ifndef __PH_H
#define __PH_H 1

typedef unsigned __int64    uint64_t ;

typedef struct extent_s     extent_t;

struct extent_s {
	uint64_t		e_bits;
	void			*e_addr;
};

/* Root structure. */
#define ph(a_type)							\
struct {			                        \
	a_type	*ph_root;						\
}


typedef ph(extent_t) extent_tree_t;
typedef ph(extent_t) extent_heap_t;


#endif  /** __PH_H */
