// array.h

#ifndef _CTC_EXAMPLE_ARRAY_H
#define _CTC_EXAMPLE_ARRAY_H 1

#include <stddef.h>
#include <stdarg.h>

#include <ctc/object.h>
#include <ctc/class.h>

struct array {
	void (*__init__)(ctc_t *, va_list);
	void (*__fini__)(ctc_t *);
	size_t (*element_size)(const ctc_t *);
	size_t (*count)(const ctc_t *);
	void *(*get)(ctc_t *, size_t);
	void *(*get_array)(ctc_t *);
	void (*insert)(ctc_t *, size_t, const void *);
	void (*remove)(ctc_t *, size_t, void **);
	void (*concat)(ctc_t *, const struct array *, const ctc_t *);
};

extern const struct ctc_iface array_iface;
extern const struct ctc_class array_class;

#endif
