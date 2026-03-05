SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
NAME := multiple_rmnet

HEADERS  := $(wildcard ../../../sdk/*.h) $(wildcard ../../*.h) $(wildcard ../../../test_helpers/*.h)
INCLUDE := -I. -I../../../sdk -I../../ -I../../../test_helpers
CFLAGS   := -O3 -Wall -pthread $(INCLUDE) $(EXTRA_CFLAGS)
LDFLAGS  :=

DEPS := $(HEADERS) ../../../sdk/libtelitSDK.* ../../libtqcm.* ../../../test_helpers/libtesthelpers.*

all: shared

shared: LDFLAGS += -L../../../sdk -L../../../qrtr -L../../ -L/lib -ltelitSDK -ltqcm -ltqrtr -lpthread  -L../../../test_helpers/ -ltesthelpers
shared: $(OBJS)
	$(CC) -o $(NAME) $^ $(CFLAGS) $(LDFLAGS)

static: LDFLAGS += -L../../../sdk -L../../../qrtr -L../../ -ltqcm -ltelitSDK -ltqrtr -lpthread -L../../../test_helpers/ -ltesthelpers
static: $(OBJS)
	$(CC) -o $(NAME) $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -f *.o
