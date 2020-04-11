# Makefile

CC := gcc

LDFLAGS := ../../libctc.a $(LDFLAGS)
CFLAGS := -std=gnu99 -I../../include -Wall -Wextra -Wno-cast-function-type \
		  $(CFLAGS)
CFLAGS_DEBUG =

objects := sayer.o main.o
headers := $(wildcard *.h ../../include/ctc/*.h)
executable := hello

ifneq (,$(findstring debug,$(MAKECMDGOALS)))
debug: $(filter-out debug,$(MAKECMDGOALS))
endif

.PHONY: all default debug
default: $(executable) ;
all: $(executable) ;
debug: ;

.PHONY: clean
clean: clean-objects ;

debug: CFLAGS_DEBUG += -ggdb -O0 -Wcast-function-type \
		-Wno-error=cast-function-type

$(executable): $(objects)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $^ $(LDFLAGS) -o $@

$(objects):%.o:%.c $(headers)
	$(CC) -c $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@

.PHONY: clean-objects
clean-objects:
	for object in $(objects); do \
	  if [ -f $$object ]; then \
	    rm -f -- $$object; \
	  fi; \
	done
