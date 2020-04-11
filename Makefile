# Makefile

CC := gcc
AR := ar

LDFLAGS := $(LDFLAGS)
LDFLAGS_SHARED := -shared
CFLAGS := -std=c99 -DCTC_DISABLE_GNUC=1 -I./include -Wall -Wextra -Wpedantic \
		-Wno-cast-function-type $(CFLAGS)
CFLAGS_SHARED := -fPIC
CFLAGS_DEBUG =

shared_object := libctc.so
static_library := libctc.a
objects := sorter.o binary-searcher.o prepare.o object.o
objects_static := $(patsubst %.o,%_static.o,$(objects))
headers := $(wildcard *.h include/ctc/*.h)
examples := example/hello example/lists

ifneq (,$(findstring debug,$(MAKECMDGOALS)))
debug: $(filter-out debug,$(MAKECMDGOALS))
endif

.PHONY: all default debug
default: shared ;
all: shared static examples ;
debug: ;

.PHONY: shared static examples
shared: $(shared_object) ;
static: $(static_library) ;
examples: $(examples) ;

.PHONY: clean
clean: clean-shared clean-static clean-objects clean-examples ;

debug: CFLAGS_DEBUG += -ggdb -O0 -Wcast-function-type \
		-Wno-error=cast-function-type

$(shared_object): $(objects)
	$(CC) $^ $(LDFLAGS) $(LDFLAGS_SHARED) -o $@

$(static_library): $(objects_static)
	$(AR) -rc $@ $^

$(objects):%.o:%.c $(headers)
	$(CC) -c $(CFLAGS) $(CFLAGS_SHARED) $(CFLAGS_DEBUG) $< -o $@

$(objects_static):%_static.o:%.c $(headers)
	$(CC) -c $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@

.PHONY: $(examples)
$(examples):%:%/Makefile $(static_library) $(headers)
	make -C $@

.PHONY: clean-shared
clean-shared:
	if [ -f $(shared_object) ]; then \
	  rm -f -- $(shared_object); \
	fi

.PHONY: clean-static
clean-static:
	if [ -f $(static_library) ]; then \
	  rm -f -- $(static_library); \
	fi

.PHONY: clean-objects
clean-objects:
	for object in $(objects) $(objects_static); do \
	  if [ -f $$object ]; then \
	    rm -f -- $$object; \
	  fi; \
	done

.PHONY: clean-examples
clean-examples:
	for example in $(examples); do \
	  make -C "$$example" clean; \
	done
