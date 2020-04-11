// binary-binary_searcher.c

#include <stdarg.h>

#include <ctc/iface.h>
#include <ctc/class.h>
#include <ctc/object.h>
#include <ctc/method.h>

#include "binary-searcher.h"

static void binary_searcher_init(ctc_t *, va_list);
static void binary_searcher_fini(ctc_t *);
static const void *binary_searcher_search(
		const ctc_t *, const void *, size_t, const void **);

const struct ctc_iface binary_searcher_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct binary_searcher),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct binary_searcher, __init__, binary_searcher_init),
		CTC_METHOD(struct binary_searcher, __fini__, binary_searcher_fini),
		CTC_METHOD(struct binary_searcher, search, binary_searcher_search),
		NULL,
	},
};

const struct ctc_class binary_searcher_class = {
	.name = "binary_searcher",
	.cache = CTC_CLASS_CACHE_INITALIZER,
	.class_iface = &binary_searcher_iface,
	.implements = (const struct ctc_impl *[]){
		CTC_IMPL_STATIC(
			&binary_searcher_iface,
			&binary_searcher_static
		),
		NULL,
	},
};

struct binary_searcher *binary_searcher_static = &(struct binary_searcher){
	.__init__ = binary_searcher_init,
	.__fini__ = binary_searcher_fini,
	.search = binary_searcher_search,
};

static void binary_searcher_init(ctc_t *this, va_list ap) {
	this->un.f = (void (*)())va_arg(ap, binary_searcher_comparator_t);
}

static void binary_searcher_fini(ctc_t *this) {
	// __attribute__((used))
	(void)this;
}

static const void *binary_searcher_search(
		const ctc_t *this, const void *target, size_t count, const void **a
) {
	const binary_searcher_comparator_t comparator =
		(binary_searcher_comparator_t)this->un.f;

	size_t offset = 0, remaining_count = count;

	while (remaining_count > 0) {
		size_t index = remaining_count / 2;

		const void *elm = a[index + offset];

		int comparison = comparator(target, elm);
		if (comparison > 0) {  // target > elm
			offset += index + 1;
			// index distance from start, including self.
			remaining_count -= index + 1;
		} else if (comparison < 0) {  // target < elm
			// index distance from end, including self.
			remaining_count -= remaining_count - index;
		} else if (comparison == 0) {  // target == elm
			return elm;
		}
	}

	return NULL;
}
