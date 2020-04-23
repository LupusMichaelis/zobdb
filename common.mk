#CC=clang

CFLAGS=-Wall \
		-Werror \
		-g -ggdb \
		-DTRACE_MALLOC_CALLS \
		-DDEBUG_HEAP \
		-DDEBUG \
		-fPIC \

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


