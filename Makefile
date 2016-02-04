
CFLAGS=-Wall \
		-Werror \
		-g -ggdb \

LDFLAGS= \
		-ldb \
		-g -ggdb \

SRCS= \
	  src/error.c \
	  src/allocator.c \
	  src/app.c \
	  src/buffer.c \
	  src/string.c \
	  src/config.c \
	  src/log.c \
	  src/client.c \
	  src/server.c \
	  src/store.c \
	  src/request-builder.c \
	  src/message.c \
# \
	  src/observable.c \

TESTS= \
	   tests/error \
	   tests/allocator \

# \
	   tests/vector \
	   tests/observable \
	   tests/buffer \
	   tests/string \

.PHONY: test-data tests

OBJS=$(SRCS:.c=.o)

TARGET= \
		zob \
		zobd

all: $(TESTS) $(TARGET)

app: main.o $(OBJS)
	$(CC) -o app $? $(CGLAGS) $(LDFLAGS)

tests: $(TESTS)

$(TESTS): $(@:=.o) $(OBJS)

$(TARGET): app
	-ln -s ./app $@

clean:
	-$(RM) $(OBJS) app main.o $(TARGET) $(TARGET:=.o) $(TESTS) $(TESTS:=.o)
