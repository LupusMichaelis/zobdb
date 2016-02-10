CFLAGS=-Wall \
		-Werror \
		-g -ggdb \

LDFLAGS= \
		-ldb \
		-g -ggdb \

SRCDIR=./src
OBJDIR=$(SRCDIR)/.objs
TESTDIR=./tests

CFILES= \
	  error.c \
	  allocator.c \
	  app.c \
	  buffer.c \
	  string.c \
	  config.c \
	  log.c \
	  client.c \
	  server.c \
	  store.c \
	  request-builder.c \
	  message.c \
# \
	  src/observable.c \


