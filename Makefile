
CFLAGS=-Wall \
		-Werror \
		-g -ggdb

LDFLAGS= \
		-g -ggdb

SRCS= \
	  src/config.c \
	  src/string.c \
	  src/buffer.c \
	  src/log.c \
	  src/store.c \
	  src/app.c \
	  src/server.c \
	  src/client.c \
	  src/message.c \
	  src/request-builder.c \


OBJS=$(SRCS:.c=.o)

TARGET= \
		client \
		readd \
		writed \
		notifyd \
		stored

all: $(TARGET)

app: app.o $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-rm -rf $(OBJS) app $(TARGET) $(TARGET:=.o)
