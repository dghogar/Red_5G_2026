SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=)

HEADERS  := $(wildcard ../*.h)
INCLUDES := -I. -I../
CFLAGS   := -O3 -Wall $(EXTRA_CFLAGS)
LDFLAGS  :=

DEPS := $(HEADERS) ../libtelitSDK.*

all: shared

shared: LDFLAGS += -L../ -ltelitSDK
shared: $(OBJS)

static: LDFLAGS += -static -L../ -ltelitSDK
static: $(OBJS)

% : %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@.o
	$(CC) $@.o $(LDFLAGS) -o $@

clean:
	rm -f $(OBJS)
	rm -f *.o
