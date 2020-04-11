// main.c

#include <stdio.h>
#include <alloca.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"
#include "array.h"
#include "linked_list.h"

static void show_contents(const struct list *, const ctc_t *);
static void play_with_arrays();
static void play_with_linkeds();

int main() {
	play_with_arrays();
	play_with_linkeds();
}

static void show_contents(
		const struct list *list_i, const ctc_t *list_d
) {
	printf("count: %zd, element size: %zd\n",
		   list_i->count(list_d),
		   list_i->element_size(list_d));

	for (size_t i = 0; list_i->count(list_d) > i; ++i) {
		const char *string = *(const char **)list_i->get((ctc_t *)list_d, i);
		printf("[%zd]: %s\n", i, string);
	}
}

static void play_with_arrays() {
	const struct array *array1_i;
	ctc_t *array1_d = alloca(sizeof(ctc_t));
	assert((array1_i = ctc_new(array1_d, &array_class, sizeof(char *))));

	const struct list *list1_i;
	assert((list1_i = ctc_as(array1_d, &list_iface)));

	puts("Inserting into first array object:");
	puts("");

	show_contents(list1_i, array1_d);

	const char *first_string = "First string!",
			   *second_string = "Second string!",
			   *first_string_second_obj = "First string, second array object!";

	list1_i->insert(array1_d,
			list1_i->count(array1_d),
			&first_string);
	show_contents(list1_i, array1_d);

	list1_i->insert(array1_d,
			list1_i->count(array1_d),
			&second_string);
	show_contents(list1_i, array1_d);

	const struct array *array2_i;
	ctc_t *array2_d = alloca(sizeof(ctc_t));
	assert((array2_i = ctc_new(array2_d, &array_class, sizeof(char *))));

	const struct list *list2_i;
	assert((list2_i = ctc_as(array2_d, &list_iface)));

	puts("");
	puts("Inserting into second array object:");
	puts("");

	show_contents(list2_i, array2_d);

	list2_i->insert(array2_d,
			list2_i->count(array2_d),
			&first_string_second_obj);
	show_contents(list2_i, array2_d);

	puts("");
	puts("Concatinating first and second array objects:");
	puts("");

	array1_i->concat(array1_d, array2_i, array2_d);
	show_contents(list1_i, array1_d);

	ctc_destroy(array1_d);
	ctc_destroy(array2_d);
}

static void play_with_linkeds() {
	const struct linked_list *obj;
	ctc_t *linked_d = alloca(sizeof(ctc_t));
	assert((obj = ctc_new(linked_d, &linked_list_class, sizeof(char *))));

	const struct list *list_obj;
	assert((list_obj = ctc_as(linked_d, &list_iface)));

	puts("Inserting into linked list object:");
	puts("");

	show_contents(list_obj, linked_d);

	const char *first_string = "First string!",
			   *second_string = "Second string!",
			   *third_string = "Third string!";

	list_obj->insert(linked_d,
			list_obj->count(linked_d),
			&first_string);
	show_contents(list_obj, linked_d);

	list_obj->insert(linked_d,
			list_obj->count(linked_d),
			&second_string);
	show_contents(list_obj, linked_d);

	list_obj->insert(linked_d,
			list_obj->count(linked_d),
			&third_string);
	show_contents(list_obj, linked_d);

	puts("");
	puts("Splitting linked list object at index 1:");
	puts("");

	ctc_t *linked2_d = alloca(sizeof(ctc_t));
	obj->split(linked_d, 1, &linked2_d);

	const struct list *list_obj_split;
	assert((list_obj_split = ctc_as(linked2_d, &list_iface)));

	show_contents(list_obj, linked_d);
	show_contents(list_obj_split, linked2_d);

	ctc_destroy(linked_d);
	ctc_destroy(linked2_d);
}
