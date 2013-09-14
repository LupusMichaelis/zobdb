
CFLAGS=-Wall \
		-Werror \
		-g -ggdb

LDFLAGS= \
		-g -ggdb

SRCS= \
	  dbd.c \
	  db.c \


OBJS=$(SRCS:.cpp=.o)

TARGET=dbd db

target: $(TARGET)

clean:
	-echo rm -rf $(OBJS) $(TARGET) $(TARGET:=.o)
