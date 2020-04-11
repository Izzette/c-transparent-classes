// ctc/method.h

#ifndef _CTC_METHOD_H
#define _CTC_METHOD_H 1

#include <stddef.h>
#include <stdarg.h>

#include <ctc/object.h>

#define CTC_METHOD_INIT "__init__"
#define CTC_METHOD_FINI "__fini__"

#define CTC_METHOD(type, member, foo) \
	(&(struct ctc_method){ \
		.name = #member, \
		.offset = offsetof(type, member), \
		.function = (ctc_function_t *)foo, \
	})

typedef void ctc_function_t(struct ctc_object *);
typedef void ctc_function_init_t(struct ctc_object *, va_list);
typedef void ctc_function_fini_t(struct ctc_object *);

struct ctc_method {
	const char *name;
	ptrdiff_t offset;
	ctc_function_t *function;
	// TODO: method_signature?
};

#endif
