// binary-searcher.h

#ifndef _CTC_INTERNAL_BINARY_SEARCHER_H
#define _CTC_INTERNAL_BINARY_SEARCHER_H 1

#include <stdarg.h>

#include <ctc/iface.h>
#include <ctc/class.h>
#include <ctc/object.h>

struct binary_searcher {
	void (*__init__)(ctc_t *, va_list);
	void (*__fini__)(ctc_t *);
	const void *(*search)(const ctc_t *, const void *, size_t, const void **);
};

typedef int (*binary_searcher_comparator_t)(const void *, const void *);

extern const struct ctc_iface binary_searcher_iface;
extern const struct ctc_class binary_searcher_class;
extern struct binary_searcher *binary_searcher_static;

static inline void binary_searcher_static_init(ctc_t *this, ...) {
	va_list ap;
	va_start(ap, this);

	binary_searcher_static->__init__(this, ap);

	va_end(ap);
}

#endif
