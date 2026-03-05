SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
NAME := telit_data_connection

HEADERS  := $(wildcard ../../../sdk/*.h) $(wildcard ../../*.h)
INCLUDE := -I. -I../../../sdk -I../../
CFLAGS   := -O3 -Wall -pthread $(INCLUDE) $(EXTRA_CFLAGS)
LDFLAGS  :=

DEPS := $(HEADERS) ../../../sdk/libtelitSDK.* ../../libtqcm.*

all: shared

shared: LDFLAGS += -L../../../sdk -L../../../qrtr -L../../ -L/lib -ltelitSDK -ltqcm -ltqrtr -lpthread
shared: $(OBJS)
	$(CC) -o $(NAME) $^ $(CFLAGS) $(LDFLAGS)

static: LDFLAGS += -L../../../sdk -L../../../qrtr -L../../ -ltqcm -ltelitSDK -ltqrtr -lpthread
static: $(OBJS)
	$(CC) -o $(NAME) $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -f *.o
