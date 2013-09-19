
CFLAGS=-Wall \
		-Werror \
		-g -ggdb

LDFLAGS= \
		-g -ggdb

SRCS= \
	  src/log.c \
	  src/store.c \
	  src/app.c \
	  src/server.c \
	  src/client.c \


OBJS=$(SRCS:.c=.o)

TARGET=dbd db

all: $(TARGET)

app: app.o $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-rm -rf $(OBJS) app $(TARGET) $(TARGET:=.o)
