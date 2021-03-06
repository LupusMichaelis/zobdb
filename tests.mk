include common.mk

LDFLAGS += \
		-L$(HOME)/.local/lib64/ \
		-L$(HOME)/.local/lib/ \
		-lcgreen \

CFLAGS += \
		-I$(HOME)/.local/include/ \

TESTS= \
	   $(TESTDIR)/test_buffer \
	   $(TESTDIR)/test_string \

UNITTESTER=cgreen-runner --colours

.PHONY: all clean $(TESTS)

all: $(TESTS)

tests/%.so: tests/%.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^ $(LDFLAGS) -fPIC

tests/%.o: tests/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

# Buffer test ###########################################################
$(TESTDIR)/test_buffer.so: $(TESTDIR)/test_buffer.o \
	src/buffer.o \

$(TESTDIR)/test_buffer: $(TESTDIR)/test_buffer.so
	$(UNITTESTER) $^

# String test ###########################################################
$(TESTDIR)/test_string.so: $(TESTDIR)/test_string.o \
	src/string.o \
	src/buffer.o \

$(TESTDIR)/test_string: $(TESTDIR)/test_string.so
	$(UNITTESTER) $^

########################################################################
clean:
	-$(RM) $(wildcard tests/*.o tests/*.so)
