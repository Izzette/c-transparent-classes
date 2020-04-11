// list.c

#include <ctc/iface.h>
#include <ctc/method.h>

#include "list.h"

const struct ctc_iface list_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct list),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct list, element_size, NULL),
		CTC_METHOD(struct list, count, NULL),
		CTC_METHOD(struct list, get, NULL),
		CTC_METHOD(struct list, insert, NULL),
		CTC_METHOD(struct list, remove, NULL),
		NULL,
	},
};
