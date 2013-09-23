
CFLAGS=-Wall \
		-Werror \
		-g -ggdb

LDFLAGS= \
		-g -ggdb

SRCS= \
	  src/buffer.c \
	  src/log.c \
	  src/store.c \
	  src/app.c \
	  src/server.c \
	  src/client.c \
	  src/request.c \
	  src/request-builder.c \


OBJS=$(SRCS:.c=.o)

TARGET=dbd db

all: $(TARGET)

app: app.o $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-rm -rf $(OBJS) app $(TARGET) $(TARGET:=.o)
