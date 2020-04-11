// ctc/object.h

#ifndef _CTC_OBJECT_H
#define _CTC_OBJECT_H 1

#include <stddef.h>
#include <stdarg.h>

#include <ctc/class.h>
#include <ctc/iface.h>

struct ctc_object {
	const struct ctc_class *class;
	union {
		void *p;
		unsigned long int i;
		void (*f)();
	} un;
};

typedef struct ctc_object ctc_t;

#define CTC_OBJECT_SET_METHOD(obj, off, ptr) \
	(*(ctc_function_t **)((char *)obj + off) = ptr)

const void *ctc_as(const ctc_t *, const struct ctc_iface *);
const void *ctc_new_v(ctc_t *, const struct ctc_class *, va_list);
void ctc_destroy(ctc_t *);

static inline const void *ctc_new(
		ctc_t *this_d, const struct ctc_class *class, ...
) {
	va_list ap;
	va_start(ap, class);

	const void *this_i = ctc_new_v(this_d, class, ap);

	va_end(ap);

	return this_i;
}

#endif
