prog=balloc

include ../GNUmakefile

try: main.o libdeq.so
	gcc -o $@ $< -L. -ldeq -Wl,-rpath=.
