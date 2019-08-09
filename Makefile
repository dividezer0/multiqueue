CC = gcc

CFLAGS = -lpthread

BUILDDIR = build
EXEC_NAME = queue_test
OBJS =queue.o main.o

OBJS_PATH = $(patsubst %,$(BUILDDIR)/%,$(OBJS))

$(EXEC_NAME): $(OBJS_PATH)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS)

$(BUILDDIR)/%.o: src/%.c $(BUILDDIR)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)
