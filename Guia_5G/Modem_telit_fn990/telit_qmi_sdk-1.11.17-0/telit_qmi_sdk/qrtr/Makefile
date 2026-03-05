LIBRARY :=  libtqrtr
STATIC  := $(LIBRARY:=.a)
SHARED  := $(LIBRARY:=.so)

HEADERS := $(wildcard *.h)
SRCS    := $(wildcard *.c)
TOBJS   := $(SRCS:%.c=static/%.o)

PREFIX  ?= /usr
IDIR    := -I. -I../sdk
CFLAGS  += -O3 -Wall -Wextra -D_GNU_SOURCE -fPIC -pthread $(EXTRA_CFLAGS)
LDFLAGS := -L../sdk -L$(PREFIX)/lib -ltelitSDK
ODIR    := .

KVER = $(shell uname -r)
KMAJ = $(shell echo $(KVER) | \
sed -e 's/^\([0-9][0-9]*\)\.[0-9][0-9]*\.[0-9][0-9]*.*/\1/')
KMIN = $(shell echo $(KVER) | \
sed -e 's/^[0-9][0-9]*\.\([0-9][0-9]*\)\.[0-9][0-9]*.*/\1/')
KREV = $(shell echo $(KVER) | \
sed -e 's/^[0-9][0-9]*\.[0-9][0-9]*\.\([0-9][0-9]*\).*/\1/')

kver_ge = $(shell \
echo test | awk '{if($(KMAJ) < $(1)) {print 0} else { \
if($(KMAJ) > $(1)) {print 1} else { \
if($(KMIN) < $(2)) {print 0} else { \
if($(KMIN) > $(2)) {print 1} else { \
if($(KREV) < $(3)) {print 0} else { print 1 } \
}}}}}' \
)

ifeq ($(call kver_ge,5,0,0),1)
CFLAGS += -DTELIT_QRTR
endif

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
