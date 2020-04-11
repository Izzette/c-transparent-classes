// ctc/iface.h

#ifndef _CTC_IFACE_H
#define _CTC_IFACE_H 1

#include <stddef.h>
#include <stdbool.h>

#define CTC_IFACE_CACHE_INITALIZER (&(struct ctc_iface_cache){ \
	.methods_sorted = false, \
	.methods_counted = false, \
	.methods_count = 0, \
})

struct ctc_iface_cache {
	bool methods_sorted;
	bool methods_counted;
	size_t methods_count;
};

struct ctc_iface {
	struct ctc_iface_cache *cache;
	const size_t size;
	const struct ctc_method **methods;
};

#endif
