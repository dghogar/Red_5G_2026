SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=)

HEADERS  := $(wildcard ../*.h)
INCLUDES := -I. -I../ -I../../sdk
CFLAGS   := -O3 -Wall $(EXTRA_CFLAGS)
LDFLAGS  :=

all: shared

shared: LDFLAGS += -L../../sdk -ltelitSDK -L../ -ltqrtr
shared: $(OBJS)

static: LDFLAGS += -static -L../../sdk -ltelitSDK -L../ -ltqrtr
static: $(OBJS)

% : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@.o
	$(CC) $@.o $(LDFLAGS) -o $@

clean:
	rm -f $(OBJS)
	rm -f *.o
