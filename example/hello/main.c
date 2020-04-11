// main.c

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <alloca.h>

#include <ctc/object.h>
#include <ctc/method.h>
#include <ctc/iface.h>

#include "sayer.h"

int main() {
	const struct sayer *sayer_i;
	ctc_t *inst = alloca(sizeof(ctc_t));
	assert((sayer_i = ctc_new(inst, &sayer_class, "Hello World!")));

	sayer_i->say(inst);

	ctc_destroy(inst);
}
