// sort.h

#ifndef _CTC_INTERNAL_SORTER_H
#define _CTC_INTERNAL_SORTER_H 1

#include <stdarg.h>

#include <ctc/iface.h>
#include <ctc/class.h>
#include <ctc/object.h>

struct sorter {
	void (*__init__)(ctc_t *, va_list);
	void (*__fini__)(ctc_t *);
	void (*sort)(const ctc_t *, size_t, const void **);
};

typedef int (*sorter_comparator_t)(const void *, const void *);

extern const struct ctc_iface sorter_iface;
extern const struct ctc_class sorter_class;
extern struct sorter *sorter_static;

static inline void sorter_static_init(ctc_t *this, ...) {
	va_list ap;
	va_start(ap, this);

	sorter_static->__init__(this, ap);

	va_end(ap);
}

#endif
