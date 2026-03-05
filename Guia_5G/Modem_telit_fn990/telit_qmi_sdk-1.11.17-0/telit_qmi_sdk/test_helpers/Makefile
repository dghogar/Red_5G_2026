LIBRARY :=  libtesthelpers
STATIC  := $(LIBRARY:=.a)
SHARED  := $(LIBRARY:=.so)

telitSDK := ../sdk

INCLUDES := -I. -I$(telitSDK)
HEADERS := $(wildcard *.h)
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:%.c=%.o)
TOBJS   := $(SRCS:%.c=%.o)

IDIR    := -I.
CFLAGS  += -Wall -O3 -Werror -D_GNU_SOURCE -g -fPIC $(EXTRA_CFLAGS)
LDFLAGS := -L/lib
ODIR    := .

all: shared

shared: LDFLAGS += -shared
shared: $(SHARED)
$(SHARED): $(SRCS) $(HEADERS)
	$(CC) $(IDIR) $(CFLAGS) $(LDFLAGS)  $(INCLUDES) $(SRCS) -o $(SHARED)

static: $(STATIC)
$(STATIC): $(TOBJS)
	ar -rcs $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $(IDIR) -c $< -o $@

clean:
	$(RM) -f *.o $(LIBRARY)*

install:
	cp $(ODIR)/$(SHARED) /usr/lib

uninstall:
	rm  /usr/lib/$(SHARED)
