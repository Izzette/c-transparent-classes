// linked.c

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <ctc/object.h>
#include <ctc/class.h>
#include <ctc/method.h>

#include "list.h"
#include "linked_list.h"

struct linked_list_node;

struct linked_list_data {
	size_t element_size;
	struct linked_list_node *node;
};

struct linked_list_node {
	struct linked_list_node *next;
	// TODO: pad
	char element[];
};

static void linked_init(ctc_t *, va_list);
static void linked_fini(ctc_t *);
static size_t linked_element_size(const ctc_t *);
static size_t linked_count(const ctc_t *);
static void *linked_get(ctc_t *, size_t);
static void linked_insert(ctc_t *, size_t, const void *);
static void linked_remove(ctc_t *, size_t, void **);
static const struct linked_list *linked_split(ctc_t *, size_t, ctc_t **);

const struct ctc_iface linked_list_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct linked_list),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct linked_list, __init__, linked_init),
		CTC_METHOD(struct linked_list, __fini__, linked_fini),
		CTC_METHOD(struct linked_list, element_size, linked_element_size),
		CTC_METHOD(struct linked_list, count, linked_count),
		CTC_METHOD(struct linked_list, get, linked_get),
		CTC_METHOD(struct linked_list, insert, linked_insert),
		CTC_METHOD(struct linked_list, remove, linked_remove),
		CTC_METHOD(struct linked_list, split, linked_split),
		NULL,
	},
};

const struct ctc_class linked_list_class = {
	.name = "linked",
	.cache = CTC_CLASS_CACHE_INITALIZER,
	.class_iface = &linked_list_iface,
	.implements = (const struct ctc_impl *[]){
		CTC_IMPL(&linked_list_iface, struct linked_list),
		CTC_IMPL(&list_iface, struct list),
		NULL,
	}
};

static void linked_init(ctc_t *this, va_list ap) {
	assert((this->un.p = malloc(sizeof(struct linked_list_data))));

	struct linked_list_data *data = this->un.p;
	*data = (struct linked_list_data) {
		.element_size = va_arg(ap, size_t),
		.node = NULL,
	};
}

static void linked_fini(ctc_t *this) {
	struct linked_list_data *data = this->un.p;

	struct linked_list_node *node = data->node;
	while (node) {
		struct linked_list_node *prev_node = node;
		node = node->next;
		free(prev_node);
	}

	free(data);
}

static size_t linked_element_size(const ctc_t *this) {
	struct linked_list_data *data = this->un.p;

	return data->element_size;
}

static size_t linked_count(const ctc_t *this) {
	struct linked_list_data *data = this->un.p;

	size_t count = 0;
	for (struct linked_list_node *node = data->node; node; node = node->next)
		count += 1;

	return count;
}

static void *linked_get(ctc_t *this, size_t index) {
	struct linked_list_data *data = this->un.p;

	struct linked_list_node *node = data->node;
	for (size_t i = 0; index > i; ++i)
		node = node->next;

	return node->element;
}

static void linked_insert(
		ctc_t *this, size_t index, const void *element
) {
	struct linked_list_data *data = this->un.p;

	struct linked_list_node *new_node;
	assert((new_node = malloc(
			offsetof(struct linked_list_node, element) + data->element_size)));
	memcpy(&new_node->element[0], element, data->element_size);

	struct linked_list_node *prev_node = NULL, *next_node = data->node;
	if (index > 0) {
		for (size_t i = 0; index > i; ++i) {
			prev_node = next_node;
			next_node = next_node->next;
		}

		prev_node->next = new_node;
	} else {
		data->node = new_node;
	}

	new_node->next = next_node;
}

static void linked_remove(
		ctc_t *this, size_t index, void **element_out
) {
	struct linked_list_data *data = this->un.p;

	struct linked_list_node *prev_node = NULL, *rm_node = data->node;
	if (index > 0) {
		for (size_t i = 0; index > i; ++i) {
			prev_node = rm_node;
			rm_node = rm_node->next;
		}

		prev_node->next = rm_node->next;
	} else {
		data->node = rm_node->next;
	}

	if (element_out)
		memcpy(*element_out, &rm_node->element[0], data->element_size);

	free(rm_node);
}

static const struct linked_list *linked_split(
		ctc_t *this, size_t partition_index, ctc_t **new_d_out
) {
	struct linked_list_data *data = this->un.p;

	const struct linked_list *new_i;
	ctc_t *new_d = malloc(sizeof(struct linked_list));
	assert((new_i = ctc_new(new_d, &linked_list_class, data->element_size)));
	struct linked_list_data *new_data = new_d->un.p;

	struct linked_list_node *prev_node = NULL, *new_root_node = data->node;
	if (partition_index > 0) {
		for (size_t i = 0; partition_index > i; ++i) {
			prev_node = new_root_node;
			new_root_node = new_root_node->next;
		}

		prev_node->next = NULL;
	} else {
		data->node = NULL;
	}

	new_data->node = new_root_node;

	*new_d_out = new_d;
	return new_i;
}
