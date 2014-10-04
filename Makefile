
CFLAGS=-Wall \
		-Werror \
		-g -ggdb \

LDFLAGS= \
		-ldb \
		-g -ggdb \

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
	  src/observable.c \

.PHONY: db dbd test-data

OBJS=$(SRCS:.c=.o)

TARGET= \
		db \
		dbd

all: $(TARGET)

app: app.o $(OBJS)

tests/observable: tests/observable.o $(OBJS)
tests/buffer: tests/buffer.o $(OBJS)
tests/string: tests/string.o $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-$(RM) $(OBJS) app $(TARGET) $(TARGET:=.o) tests/buffer.o
