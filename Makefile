
CFLAGS=-Wall \
		-Werror \
		-g -ggdb

LDFLAGS= \
		-g -ggdb

SRCS= \
	  src/server.c \
	  src/client.c \
	  src/app.c \


OBJS=$(SRCS:.c=.o)

TARGET=dbd db

all: $(TARGET)

dbd: dbd.o $(OBJS)

db: db.o $(OBJS)

clean:
	-rm -rf $(OBJS) $(TARGET) $(TARGET:=.o)
