LIBRARY :=  libtqcm
STATIC  := $(LIBRARY:=.a)
SHARED  := $(LIBRARY:=.so)

HEADERS := $(wildcard *.h)
SRCS    := $(wildcard *.c)
TOBJS   := $(SRCS:%.c=static/%.o)

PREFIX  ?= /usr
IDIR    := -I. -I../sdk -I../qrtr
CFLAGS  += -O3 -Wall -D_GNU_SOURCE -fPIC -pthread $(EXTRA_CFLAGS)
LDFLAGS := -L../sdk -L../qrtr -L$(PREFIX)/lib -ltelitSDK -ltqrtr -lpthread
ODIR    := .

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
