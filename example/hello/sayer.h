// sayer.h

#ifndef _CTC_EXAMPLE_SAYER_H
#define _CTC_EXAMPLE_SAYER_H 1

#include <stdarg.h>

#include <ctc/object.h>

struct sayer {
	void (*__init__)(ctc_t *, va_list);
	void (*__fini__)(ctc_t *);
	void (*say)(ctc_t *);
};

extern const struct ctc_iface sayer_iface;
extern const struct ctc_class sayer_class;

#endif
