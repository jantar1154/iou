CC = gcc
name = iou
args = -Wall -fsanitize=address -g -O0
srcdir = src
objdir = build

src = $(wildcard $(srcdir)/*.c)
obj = $(src:$(srcdir)/%.c=$(objdir)/%.o)

######

$(name): $(obj)
	$(CC) $(args) -o $@ $^

$(objdir)/%.o: $(srcdir)/%.c
	$(CC) $(args) -o $@ -c $<

clean:
	rm $(name) $(obj)