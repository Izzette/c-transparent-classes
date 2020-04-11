// sorter.c

#include <stddef.h>
#include <stdarg.h>

#include <ctc/method.h>
#include <ctc/export.h>

#include "sorter.h"

static void sorter_init(ctc_t *, va_list);
static void sorter_fini(ctc_t *);
static void sorter_sort(const ctc_t *, size_t, const void **);

static void sorter_quicksort(
		const ctc_t *, const void **, size_t, size_t);
static size_t sorter_partition(
		const ctc_t *, const void **, size_t, size_t);

const struct ctc_iface sorter_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct sorter),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct sorter, __init__, sorter_init),
		CTC_METHOD(struct sorter, __fini__, sorter_fini),
		CTC_METHOD(struct sorter, sort, sorter_sort),
		NULL,
	},
};

CTC_EXPORT_IFACE(sorter_iface)

const struct ctc_class sorter_class = {
	.name = "sorter",
	.cache = CTC_CLASS_CACHE_INITALIZER,
	.class_iface = &sorter_iface,
	.implements = (const struct ctc_impl *[]){
		CTC_IMPL_STATIC(
			&sorter_iface,
			&sorter_static
		),
		NULL,
	},
};

CTC_EXPORT_CLASS(sorter_class)

struct sorter *sorter_static = &(struct sorter){
	.__init__ = sorter_init,
	.__fini__ = sorter_fini,
	.sort = sorter_sort,
};

static void sorter_init(ctc_t *this, va_list ap) {
	this->un.f = (void (*)())va_arg(ap, sorter_comparator_t);
}

static void sorter_fini(ctc_t *this) {
	// __attribute__((used))
	(void)this;
}

static void sorter_sort(const ctc_t *this, size_t count, const void **a) {
	sorter_quicksort(this, a, 0, count);
}

static void sorter_quicksort(
		const ctc_t *this, const void **a, size_t low, size_t xhigh
) {
	if (1 >= xhigh - low)
		return;  // Range is sorted.

	size_t p = sorter_partition(this, a, low, xhigh);

	// sorter range low <= i < p
	sorter_quicksort(this, a, low, p);
	// sorter range p < i < xhigh
	sorter_quicksort(this, a, p + 1, xhigh);
}

static size_t sorter_partition(
		const ctc_t *this, const void **a, size_t low, size_t xhigh
) {
	const sorter_comparator_t comparator = (sorter_comparator_t)this->un.f;

	const void *pivot = a[xhigh - 1];

	size_t i = low;
	for (size_t j = low; xhigh - 1 > j; ++j) {
		// a[j] < pivot
		if (0 > comparator(a[j], pivot)) {
			const void *tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
			i += 1;
		}
	}

	a[xhigh - 1] = a[i];
	a[i] = pivot;

	return i;
}
