
include common.mk

SRCS=$(addprefix $(SRCDIR)/, $(CFILES))
OBJS=$(addprefix $(SRCDIR)/, $(CFILES:.c=.o))

.PHONY: test-data tests clean

TARGET= \
		zob \
		zobd

all: $(TESTS) $(TARGET)

app: main.o $(OBJS)
	$(CC) -o $@ $^ $(CGLAGS) $(LDFLAGS)

tests:
	$(MAKE) -f tests.mk

$(TARGET): app
	-ln -sf ./app $@

clean:
	-$(RM) $(OBJS) app main.o $(TARGET) $(TARGET:=.o)
	-$(MAKE) -f tests.mk clean
