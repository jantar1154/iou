CC = gcc
name = iou
args = -Wall -fsanitize=address -g -O0 -lncurses
srcdir = src
objdir = build

src = $(wildcard $(srcdir)/*.c)
obj = $(src:$(srcdir)/%.c=$(objdir)/%.o)

######

$(name): $(obj)
	$(CC) $(args) -o $@ $^

$(objdir)/%.o: $(srcdir)/%.c
	mkdir -p $(objdir)
	$(CC) $(args) -o $@ -c $<

clean:
	rm $(name) $(obj)
	rmdir $(objdir)