all: ncist say.so backloggd.so
.PHONY:	all

ncist: main.o plugin.o
	gcc -o $@ $^ -lncurses

%.o: %.c
	gcc -o $@ $< -O2 -c -g

%.so: %.c
	gcc -o plugins/$@ $< -fPIC -shared -lcurl -lcjson

clean:
	rm -f ncist plugins/* *.o
.PHONY: clean
