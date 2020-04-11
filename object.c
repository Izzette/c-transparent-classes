// object.c

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#include <ctc/class.h>
#include <ctc/method.h>
#include <ctc/iface.h>
#include <ctc/object.h>
#include <ctc/prepare.h>

#include "binary-searcher.h"

// Undefined when (false == ctc_is_a(this, iface)).
const void *ctc_as(const ctc_t *this, const struct ctc_iface *iface) {
	const struct ctc_class *class = this->class;

	ctc_ensure_class_impls_sorted(class);
	ctc_ensure_class_impls_counted(class);
	size_t impls_count = class->cache->impls_count;

	ctc_t *impl_binary_searcher_d = alloca(sizeof(ctc_t));
	binary_searcher_static_init(impl_binary_searcher_d, ctc_impl_compare);

	void *target_impl_struct = alloca(iface->size);
	struct ctc_impl *target_impl = CTC_IMPL_DECL(
			iface, CTC_IMPL_CACHE_INITALIZER, target_impl_struct);
	const struct ctc_impl *impl = binary_searcher_static->search(
			impl_binary_searcher_d, target_impl, impls_count,
			(const void **)class->implements);

	binary_searcher_static->__fini__(impl_binary_searcher_d);

	ctc_ensure_class_impl_constructed(class, impl);

	return impl->impl;
}

const void *ctc_new_v(
		ctc_t *this, const struct ctc_class *class, va_list ap
) {
	const struct ctc_iface *class_iface = class->class_iface;

	*this = (ctc_t){
		.class = class,
	};
	memset(&this->un, '\0', sizeof(this->un));


	// TODO: implement initable?
	const struct ctc_method *class_method;
	for (size_t i = 0; (class_method = class_iface->methods[i]); ++i) {
		if (0 == strcmp(class_method->name, CTC_METHOD_INIT)) {
			((ctc_function_init_t *)class_method->function)(this, ap);

			break;
		}
	}

	return ctc_as(this, class->class_iface);
}

void ctc_destroy(ctc_t *this) {
	const struct ctc_iface *class_iface = this->class->class_iface;

	// TODO: implement destroyable?
	const struct ctc_method *class_method;
	for (size_t i = 0; (class_method = class_iface->methods[i]); ++i) {
		if (0 == strcmp(class_method->name, CTC_METHOD_FINI)) {
			((ctc_function_fini_t *)class_method->function)(this);

			break;
		}
	}
}
