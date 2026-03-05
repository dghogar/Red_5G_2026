LIBRARY :=  libtelitSDK
STATIC  := $(LIBRARY:=.a)
SHARED  := $(LIBRARY:=.so)

HEADERS := $(wildcard *.h) $(wildcard ../*.h)
SRCS    := $(wildcard *.c)
ifeq ($(HAVE_STRLCPY),1)
  SRCS := $(filter-out strlcpy.c,$(SRCS))
endif
OBJS    := $(SRCS:%.c=%.o)
TOBJS   := $(SRCS:%.c=static/%.o)

IDIR    := -I. -I..
CFLAGS  += -O3 -Wall -D_GNU_SOURCE -fPIC $(EXTRA_CFLAGS)
LDFLAGS := -L/lib
ODIR    := .
PREFIX  ?= /usr

all: shared

stack: CFLAGS += -fstack-usage
stack: $(STATIC)

shared: LDFLAGS += -shared
shared: $(SHARED)
$(SHARED): $(SRCS) $(HEADERS)
	$(CC) $(IDIR) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(SHARED)

static: $(STATIC)
$(STATIC): $(TOBJS)
	ar -rcs $@ $^

static/%.o: %.c $(HEADERS)
	if [ ! -d ./static ]; then mkdir static; fi
	$(CC) $(CFLAGS) $(IDIR) -c $< -o $@

clean:
	rm -rf ./static
	rm -f ./$(LIBRARY)*

install:
	cp $(ODIR)/$(SHARED) $(PREFIX)/lib

uninstall:
	rm $(PREFIX)/lib/$(SHARED)
