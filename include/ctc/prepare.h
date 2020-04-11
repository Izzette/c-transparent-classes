// ctc/prepare.h

#ifndef _CTC_PREPARE_H
#define _CTC_PREPARE_H 1

#include <stddef.h>
#include <string.h>

#include <ctc/method.h>
#include <ctc/iface.h>
#include <ctc/class.h>

#if defined(__GNUC__) && !defined(CTC_DISABLE_GNUC)
# define __CTC_PREP_CLASS_ONLOAD(class_sym) \
	__attribute__((constructor, used)) \
	static void __ctc_prepare_##class_sym##_constructor() { \
		ctc_ensure_class_impls_sorted(&class_sym); \
		ctc_ensure_class_impls_counted(&class_sym); \
		ctc_ensure_class_impls_constructed(&class_sym); \
	}
# define __CTC_PREP_IFACE_ONLOAD(iface_sym) \
	__attribute__((constructor, used)) \
	static void __ctc_prepare_##iface_sym##_constructor() { \
		ctc_ensure_iface_methods_sorted(&iface_sym); \
		ctc_ensure_iface_methods_counted(&iface_sym); \
	}
#else
# define __CTC_PREP_CLASS_ONLOAD(class_sym)
# define __CTC_PREP_IFACE_ONLOAD(iface_sym);
#endif

#ifndef CTC_PREP_CLASS_ONLOAD
# define CTC_PREP_CLASS_ONLOAD(class_sym) __CTC_PREP_CLASS_ONLOAD(class_sym)
#endif
#ifndef CTC_PREP_IFACE_ONLOAD
# define CTC_PREP_IFACE_ONLOAD(iface_sym) __CTC_PREP_IFACE_ONLOAD(iface_sym)
#endif

void ctc_sort_methods(size_t, const struct ctc_method **);
size_t ctc_count_methods(const struct ctc_method **);
void ctc_sort_impls(size_t, const struct ctc_impl **);
size_t ctc_count_impls(const struct ctc_impl **);
void ctc_construct_impl(
		const struct ctc_iface *, const struct ctc_iface *, void *);

static inline void ctc_ensure_iface_methods_counted(const struct ctc_iface *);
static inline void ctc_ensure_class_impls_counted(const struct ctc_class *);

static inline void ctc_ensure_iface_methods_sorted(
		const struct ctc_iface *iface
) {
	if (iface->cache->methods_sorted)
		return;

	ctc_ensure_iface_methods_counted(iface);
	ctc_sort_methods(iface->cache->methods_count, iface->methods);
	iface->cache->methods_sorted = true;
}

static inline void ctc_ensure_iface_methods_counted(
		const struct ctc_iface *iface
) {
	if (iface->cache->methods_counted)
		return;

	size_t count = ctc_count_methods(iface->methods);
	iface->cache->methods_count = count;
	iface->cache->methods_counted = true;
}

static inline void ctc_ensure_class_impls_sorted(
		const struct ctc_class *class
) {
	if (class->cache->impls_sorted)
		return;

	ctc_ensure_class_impls_counted(class);
	ctc_sort_impls(class->cache->impls_count, class->implements);
	class->cache->impls_sorted = true;
}

static inline void ctc_ensure_class_impls_counted(
		const struct ctc_class *class
) {
	if (class->cache->impls_counted)
		return;

	size_t count = ctc_count_impls(class->implements);
	class->cache->impls_count = count;
	class->cache->impls_counted = true;
}

static inline void ctc_ensure_class_impl_constructed(
		const struct ctc_class *class, const struct ctc_impl *impl
) {
	if (impl->cache->init)
		return;

	ctc_construct_impl(class->class_iface, impl->iface, impl->impl);
	impl->cache->init = true;
}

static inline void ctc_ensure_class_impls_constructed(
		const struct ctc_class *class
) {
	for (size_t i = 0; class->implements[i]; ++i) {
		const struct ctc_impl *impl = class->implements[i];
		ctc_ensure_class_impl_constructed(class, impl);
	}
}

static inline int ctc_method_compare(
		const struct ctc_method *a, const struct ctc_method *b
) {
	return strcmp(a->name, b->name);
}

static inline int ctc_impl_compare(
		const struct ctc_impl *a, const struct ctc_impl *b
) {
	if (a->iface > b->iface)
		return 1;
	else if (a->iface < b->iface)
		return -1;
	else
		return 0;
}

#endif
