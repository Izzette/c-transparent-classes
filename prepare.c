// prepare.c

#include <alloca.h>
#include <string.h>
#include <assert.h>

#include <ctc/prepare.h>
#include <ctc/object.h>
#include <ctc/class.h>
#include <ctc/method.h>

#include "sorter.h"
#include "binary-searcher.h"

static int ctc_impl_compare(const struct ctc_impl *, const struct ctc_impl *);
static size_t ctc_count_ptrarray(const void **);

void ctc_sort_methods(size_t count, const struct ctc_method **methods) {
	ctc_t *method_sorter_d = alloca(sizeof(ctc_t));
	sorter_static_init(method_sorter_d, ctc_method_compare);

	sorter_static->sort(method_sorter_d, count, (const void **)methods);

	sorter_static->__fini__(method_sorter_d);
}

size_t ctc_count_methods(const struct ctc_method **methods) {
	return ctc_count_ptrarray((const void **)methods);
}

void ctc_sort_impls(size_t count, const struct ctc_impl **impls) {
	ctc_t *impl_sorter_d = alloca(sizeof(ctc_t));
	sorter_static_init(impl_sorter_d, ctc_impl_compare);

	sorter_static->sort(impl_sorter_d, count, (const void **)impls);

	sorter_static->__fini__(impl_sorter_d);
}

size_t ctc_count_impls(const struct ctc_impl **impls) {
	return ctc_count_ptrarray((const void **)impls);
}

void ctc_construct_impl(
		const struct ctc_iface *class_iface,
		const struct ctc_iface *impl_iface,
		void *impl
) {
	ctc_ensure_iface_methods_sorted(class_iface);
	ctc_ensure_iface_methods_counted(class_iface);
	ctc_ensure_iface_methods_counted(impl_iface);

	ctc_t *method_binary_searcher_d = alloca(sizeof(ctc_t));
	binary_searcher_static_init(method_binary_searcher_d, ctc_method_compare);

	for (size_t i = 0; impl_iface->cache->methods_count > i; ++i) {
		const struct ctc_method *impl_method = impl_iface->methods[i];

		const struct ctc_method *class_method;
		assert((class_method = binary_searcher_static->search(
				method_binary_searcher_d, impl_method,
				class_iface->cache->methods_count,
				(const void **)class_iface->methods)));

		ctc_function_t **impl_function =
			(ctc_function_t **)((char *)impl + impl_method->offset);
		*impl_function = class_method->function;
	}

	sorter_static->__fini__(method_binary_searcher_d);
}

static size_t ctc_count_ptrarray(const void **a) {
	size_t count = 0;
	do {
		if (!a[count])
			break;

		count += 1;
	} while (1);

	return count;
}
