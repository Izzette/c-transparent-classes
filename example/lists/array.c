// array.c

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ctc/object.h>
#include <ctc/method.h>
#include <ctc/class.h>

#include "list.h"
#include "array.h"

struct array_data {
	size_t element_size;
	size_t count;
	void *array;
};

static void array_init(ctc_t *, va_list);
static void array_fini(ctc_t *);
static size_t array_element_size(const ctc_t *);
static size_t array_count(const ctc_t *);
static void *array_get(ctc_t *, size_t);
static void *array_get_array(ctc_t *);
static void array_insert(ctc_t *, size_t, const void *);
static void array_remove(ctc_t *, size_t, void **);
static void array_concat(ctc_t *, const struct array *, const ctc_t *);

const struct ctc_iface array_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct array),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct array, __init__, array_init),
		CTC_METHOD(struct array, __fini__, array_fini),
		CTC_METHOD(struct array, element_size, array_element_size),
		CTC_METHOD(struct array, count, array_count),
		CTC_METHOD(struct array, get, array_get),
		CTC_METHOD(struct array, get_array, array_get_array),
		CTC_METHOD(struct array, insert, array_insert),
		CTC_METHOD(struct array, remove, array_remove),
		CTC_METHOD(struct array, concat, array_concat),
		NULL,
	},
};

const struct ctc_class array_class = {
	.name = "array",
	.cache = CTC_CLASS_CACHE_INITALIZER,
	.class_iface = &array_iface,
	.implements = (const struct ctc_impl *[]){
		CTC_IMPL(&array_iface, struct array),
		CTC_IMPL(&list_iface, struct list),
		NULL,
	}
};

static void array_init(ctc_t *this, va_list ap) {
	assert((this->un.p = malloc(sizeof(struct array_data))));

	struct array_data *data = this->un.p;
	*data = (struct array_data){
		.element_size = va_arg(ap, size_t),
		.count = 0,
		.array = NULL,
	};
}

static void array_fini(ctc_t *this) {
	struct array_data *data = this->un.p;

	free(data->array);
	free(data);
}

static size_t array_element_size(const ctc_t *this) {
	struct array_data *data = this->un.p;

	return data->element_size;
}

static size_t array_count(const ctc_t *this) {
	struct array_data *data = this->un.p;

	return data->count;
}

static void *array_get(ctc_t *this, size_t index) {
	struct array_data *data = this->un.p;

	return (char *)data->array + data->element_size * index;
}

static void *array_get_array(ctc_t *this) {
	return array_get(this, 0);
}

static void array_insert(
		ctc_t *this, size_t index, const void *element
) {
	struct array_data *data = this->un.p;

	data->count += 1;
	size_t new_length = data->element_size * data->count;
	assert((data->array = realloc(data->array, new_length)));

	memmove(array_get(this, index + 1), array_get(this, index),
			data->element_size * (data->count - 1 - index));

	memcpy(array_get(this, index), element, data->element_size);
}

static void array_remove(
		ctc_t *this, size_t index, void **element_out
) {
	struct array_data *data = this->un.p;

	memmove(array_get(this, index + 1), array_get(this, index),
			data->element_size * (data->count - 1 - index));

	if (element_out)
		memcpy(*element_out, array_get(this, index), data->element_size);

	data->count -= 1;
	size_t new_length = data->element_size * data->count;
	assert((data->array = realloc(data->array, new_length)));
}

static void array_concat(
		ctc_t *this, const struct array *other_i, const ctc_t *other_d
) {
	struct array_data *data = this->un.p;

	assert(data->element_size == other_i->element_size(other_d));

	size_t end_index = data->count;
	data->count += other_i->count(other_d);
	size_t new_length = data->element_size * data->count;
	assert((data->array = realloc(data->array, new_length)));

	// I promise not to modify any data refered to by other_d:
	const void *other_array = (const void *)other_i->get_array((ctc_t *)other_d);

	// TODO: avoid possible multiplication overflow.
	memcpy(array_get(this, end_index), other_array,
			data->element_size * other_i->count(other_d));
}
