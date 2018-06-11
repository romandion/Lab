
#ifndef __MACRO_H
#define __MACRO_H 1

#include "ph.h"

#define UNUSED

/*
 * The ph_gen() macro generates a type-specific pairing heap implementation,
 * based on the above cpp macros.
 */

#define ph_gen(a_attr , a_prefix, a_ph_type, a_type, a_field, a_cmp)	\
a_attr void								\
a_prefix##new(a_ph_type *ph) {						\
	memset(ph, 0, sizeof(a_ph_type));				\
}/**									\
a_attr bool								\
a_prefix##empty(a_ph_type *ph) {					\
	return (ph->ph_root == NULL);					\
}									\
a_attr a_type *								\
a_prefix##first(a_ph_type *ph) {					\
	if (ph->ph_root == NULL) {					\
		return NULL;						\
	}								\
	ph_merge_aux(a_type, a_field, ph, a_cmp);			\
	return ph->ph_root;						\
}									\
a_attr a_type *								\
a_prefix##any(a_ph_type *ph) {						\
	if (ph->ph_root == NULL) {					\
		return NULL;						\
	}								\
	a_type *aux = phn_next_get(a_type, a_field, ph->ph_root);	\
	if (aux != NULL) {						\
		return aux;						\
	}								\
	return ph->ph_root;						\
}									\
a_attr void								\
a_prefix##insert(a_ph_type *ph, a_type *phn) {				\
	memset(&phn->a_field, 0, sizeof(phn(a_type)));			\
	if (ph->ph_root == NULL) {					\
		ph->ph_root = phn;					\
	} else {							\
		phn_next_set(a_type, a_field, phn, phn_next_get(a_type,	\
		    a_field, ph->ph_root));				\
		if (phn_next_get(a_type, a_field, ph->ph_root) !=	\
		    NULL) {						\
			phn_prev_set(a_type, a_field,			\
			    phn_next_get(a_type, a_field, ph->ph_root),	\
			    phn);					\
		}							\
		phn_prev_set(a_type, a_field, phn, ph->ph_root);	\
		phn_next_set(a_type, a_field, ph->ph_root, phn);	\
	}								\
}									\
a_attr a_type *								\
a_prefix##remove_first(a_ph_type *ph) {					\
	a_type *ret;							\
									\
	if (ph->ph_root == NULL) {					\
		return NULL;						\
	}								\
	ph_merge_aux(a_type, a_field, ph, a_cmp);			\
									\
	ret = ph->ph_root;						\
									\
	ph_merge_children(a_type, a_field, ph->ph_root, a_cmp,		\
	    ph->ph_root);						\
									\
	return ret;							\
}									\
a_attr a_type *								\
a_prefix##remove_any(a_ph_type *ph) {					\
	if (ph->ph_root == NULL) {					\
		return NULL;						\
	}								\
	a_type *ret = phn_next_get(a_type, a_field, ph->ph_root);	\
	if (ret != NULL) {						\
		a_type *aux = phn_next_get(a_type, a_field, ret);	\
		phn_next_set(a_type, a_field, ph->ph_root, aux);	\
		if (aux != NULL) {					\
			phn_prev_set(a_type, a_field, aux,		\
			    ph->ph_root);				\
		}							\
		return ret;						\
	}								\
	ret = ph->ph_root;						\
	ph_merge_children(a_type, a_field, ph->ph_root, a_cmp,		\
	    ph->ph_root);						\
	return ret;							\
}									\
a_attr void								\
a_prefix##remove(a_ph_type *ph, a_type *phn) {				\
	a_type *replace, *parent;					\
									\
	if (ph->ph_root == phn) {					\
		if (phn_lchild_get(a_type, a_field, phn) == NULL) {	\
			ph->ph_root = phn_next_get(a_type, a_field,	\
			    phn);					\
			if (ph->ph_root != NULL) {			\
				phn_prev_set(a_type, a_field,		\
				    ph->ph_root, NULL);			\
			}						\
			return;						\
		}							\
		ph_merge_aux(a_type, a_field, ph, a_cmp);		\
		if (ph->ph_root == phn) {				\
			ph_merge_children(a_type, a_field, ph->ph_root,	\
			    a_cmp, ph->ph_root);			\
			return;						\
		}							\
	}								\
									\
	if ((parent = phn_prev_get(a_type, a_field, phn)) != NULL) {	\
		if (phn_lchild_get(a_type, a_field, parent) != phn) {	\
			parent = NULL;					\
		}							\
	}								\
	ph_merge_children(a_type, a_field, phn, a_cmp, replace);	\
	if (replace != NULL) {						\
		if (parent != NULL) {					\
			phn_prev_set(a_type, a_field, replace, parent);	\
			phn_lchild_set(a_type, a_field, parent,		\
			    replace);					\
		} else {						\
			phn_prev_set(a_type, a_field, replace,		\
			    phn_prev_get(a_type, a_field, phn));	\
			if (phn_prev_get(a_type, a_field, phn) !=	\
			    NULL) {					\
				phn_next_set(a_type, a_field,		\
				    phn_prev_get(a_type, a_field, phn),	\
				    replace);				\
			}						\
		}							\
		phn_next_set(a_type, a_field, replace,			\
		    phn_next_get(a_type, a_field, phn));		\
		if (phn_next_get(a_type, a_field, phn) != NULL) {	\
			phn_prev_set(a_type, a_field,			\
			    phn_next_get(a_type, a_field, phn),		\
			    replace);					\
		}							\
	} else {							\
		if (parent != NULL) {					\
			a_type *next = phn_next_get(a_type, a_field,	\
			    phn);					\
			phn_lchild_set(a_type, a_field, parent, next);	\
			if (next != NULL) {				\
				phn_prev_set(a_type, a_field, next,	\
				    parent);				\
			}						\
		} else {						\
			assert(phn_prev_get(a_type, a_field, phn) !=	\
			    NULL);					\
			phn_next_set(a_type, a_field,			\
			    phn_prev_get(a_type, a_field, phn),		\
			    phn_next_get(a_type, a_field, phn));	\
		}							\
		if (phn_next_get(a_type, a_field, phn) != NULL) {	\
			phn_prev_set(a_type, a_field,			\
			    phn_next_get(a_type, a_field, phn),		\
			    phn_prev_get(a_type, a_field, phn));	\
		}							\
	}								\
}
*/

#endif  /** __MACRO_H */