// list.h

#ifndef _CTC_EXAMPLE_LIST_H
#define _CTC_EXAMPLE_LIST_H 1

#include <stddef.h>

#include <ctc/object.h>
#include <ctc/iface.h>

// pointers to elements may change between calls.
struct list {
	size_t (*element_size)(const ctc_t *);
	size_t (*count)(const ctc_t *);
	void *(*get)(ctc_t *, size_t);
	void (*insert)(ctc_t *, size_t, const void *);
	void (*remove)(ctc_t *, size_t, void **);
};

extern const struct ctc_iface list_iface;

#endif
