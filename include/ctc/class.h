// ctc/class.h

#ifndef _CTC_CLASS_H
#define _CTC_CLASS_H 1

#include <stddef.h>
#include <stdbool.h>

#define __CTC_IMPL_DECL(impl_iface, impl_init, impl_ptr) \
	(&(struct ctc_impl){ \
		.iface = impl_iface, \
		.cache = impl_init, \
		.impl = impl_ptr, \
	})

#ifndef CTC_IMPL_DECL
# define CTC_IMPL_DECL(impl_iface, impl_init, impl_ptr) \
	__CTC_IMPL_DECL(impl_iface, impl_init, impl_ptr)
#endif
#define CTC_IMPL_STATIC(impl_iface, impl_ptr) \
	(__CTC_IMPL_DECL( \
		impl_iface, \
		&(struct ctc_impl_cache){ .init = true }, \
		impl_ptr \
	))
#define CTC_IMPL(impl_iface, impl_type) \
	(__CTC_IMPL_DECL( \
		impl_iface, \
		CTC_IMPL_CACHE_INITALIZER, \
		&(char[sizeof(impl_type)]){0} \
	))

#define CTC_CLASS_CACHE_INITALIZER (&(struct ctc_class_cache){ \
	.impls_sorted = false, \
	.impls_counted = false, \
	.impls_count = 0, \
})
#define CTC_IMPL_CACHE_INITALIZER (&(struct ctc_impl_cache){ \
	.init = false, \
})

struct ctc_class_cache {
	bool impls_sorted;
	bool impls_counted;
	size_t impls_count;
};

struct ctc_impl_cache {
	bool init;
};

struct ctc_impl {
	struct ctc_impl_cache *cache;
	const struct ctc_iface *iface;
	void *impl;
};

struct ctc_class {
	const char *name;
	struct ctc_class_cache *cache;
	const struct ctc_iface *class_iface;
	const struct ctc_impl **implements;
};

#endif
