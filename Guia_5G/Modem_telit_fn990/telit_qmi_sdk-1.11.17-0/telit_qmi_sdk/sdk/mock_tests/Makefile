SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=)

HEADERS  := $(wildcard ../*.h) $(wildcard ../../test_helpers/*.h)
INCLUDES := -I. -I../ -I../../test_helpers/
CFLAGS   := -O3 -Wall $(EXTRA_CFLAGS)
LDFLAGS  :=

DEPS := $(HEADERS) ../libtelitSDK.* ../../test_helpers/libtesthelpers.*

all: shared

shared: LDFLAGS += -L../ -ltelitSDK -L../../test_helpers/ -ltesthelpers
shared: $(OBJS)

static: LDFLAGS += -static -L../ -ltelitSDK -L../../test_helpers/ -ltesthelpers
static: $(OBJS)

% : %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@.o
	$(CC) $@.o $(LDFLAGS) -o $@

clean:
	$(RM) -f $(OBJS) *.o
