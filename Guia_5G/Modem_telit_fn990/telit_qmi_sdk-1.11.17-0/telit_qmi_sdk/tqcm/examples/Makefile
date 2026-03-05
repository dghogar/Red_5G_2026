SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=)

HEADERS  := $(wildcard ../../sdk/*.h) $(wildcard ../*.h) $(wildcard ../../test_helpers/*.h)
INCLUDES := -I. -I../../sdk -I../ -I../../test_helpers
CFLAGS   := -O3 -Wall -pthread $(EXTRA_CFLAGS)
LDFLAGS  :=

DEPS := $(HEADERS) ../../sdk/libtelitSDK.* ../libtqcm.* ../../test_helpers/libtesthelpers.*

all: shared

shared: LDFLAGS += -L../../sdk -L../../qrtr -L../ -L/lib -ltelitSDK -ltqcm -ltqrtr -lpthread  -L../../test_helpers/ -ltesthelpers
shared: $(OBJS)

static: LDFLAGS += -L../../sdk -L../../qrtr -L../ -ltqcm -ltelitSDK -ltqrtr -lpthread  -L../../test_helpers/ -ltesthelpers
static: $(OBJS)

% : %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@.o
	$(CC) $@.o $(LDFLAGS) -o $@

clean:
	rm -f $(OBJS)
	rm -f *.o
