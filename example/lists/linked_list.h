// linked.h

#ifndef _CTC_EXAMPLE_LINKED_H
#define _CTC_EXAMPLE_LINKED_H 1

#include <stddef.h>
#include <stdarg.h>

#include <ctc/object.h>
#include <ctc/class.h>

struct linked_list {
	void (*__init__)(ctc_t *, va_list);
	void (*__fini__)(ctc_t *);
	size_t (*element_size)(const ctc_t *);
	size_t (*count)(const ctc_t *);
	void *(*get)(ctc_t *, size_t);
	void (*insert)(ctc_t *, size_t, const void *);
	void (*remove)(ctc_t *, size_t, void **);
	const struct linked_list *(*split)(ctc_t *, size_t, ctc_t **);
};

extern const struct ctc_iface linked_list_iface;
extern const struct ctc_class linked_list_class;

#endif
