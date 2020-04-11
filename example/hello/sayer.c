// sayer.c

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include <ctc/object.h>
#include <ctc/method.h>
#include <ctc/export.h>
#include <ctc/prepare.h>

#include "sayer.h"

struct sayer_data {
	const char *sayer_string;
};

static void sayer_init(ctc_t *, va_list);
static void sayer_fini(ctc_t *);
static void sayer_say(ctc_t *);

const struct ctc_iface sayer_iface = {
	.cache = CTC_IFACE_CACHE_INITALIZER,
	.size = sizeof(struct sayer),
	.methods = (const struct ctc_method *[]){
		CTC_METHOD(struct sayer, __init__, sayer_init),
		CTC_METHOD(struct sayer, __fini__, sayer_fini),
		CTC_METHOD(struct sayer, say, sayer_say),
		NULL,
	},
};

CTC_EXPORT_IFACE(sayer_iface);
CTC_PREP_IFACE_ONLOAD(sayer_iface)

const struct ctc_class sayer_class = {
	.name = "sayer",
	.cache = CTC_CLASS_CACHE_INITALIZER,
	.class_iface = &sayer_iface,
	.implements = (const struct ctc_impl *[]){
		CTC_IMPL(&sayer_iface, struct sayer),
		NULL,
	},
};

CTC_EXPORT_CLASS(sayer_class);
CTC_PREP_CLASS_ONLOAD(sayer_class)

static void sayer_init(ctc_t *this, va_list ap) {
	assert((this->un.p = malloc(sizeof(struct sayer_data))));

	struct sayer_data *data = this->un.p;
	data->sayer_string = va_arg(ap, const char *);
}

static void sayer_fini(ctc_t *this) {
	free(this->un.p);
}

static void sayer_say(ctc_t *this) {
	struct sayer_data *data = this->un.p;

	puts(data->sayer_string);
}
