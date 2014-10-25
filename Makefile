
CFLAGS=-Wall \
		-Werror \
		-g -ggdb \

LDFLAGS= \
		-ldb \
		-g -ggdb \

SRCS= \
	  src/error.c \

# \
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

TESTS= \
	   tests/error \
	   tests/vector \
	   tests/observable \
	   tests/buffer \
	   tests/string \

.PHONY: db dbd test-data tests

OBJS=$(SRCS:.c=.o)

TARGET= \
		db \
		dbd

all: $(TESTS) $(TARGET)

app: app.o $(OBJS)

tests: $(TESTS)

$(TESTS): $(@:=.o) $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-$(RM) $(OBJS) app app.o $(TARGET) $(TARGET:=.o) $(TESTS) $(TESTS:=.o)
