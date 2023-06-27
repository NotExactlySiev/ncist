CC 			:= gcc
CC_FLAGS	:= -std=gnu11 -O2 -g

EXEC	:= ncist
PLUGINS	:= say.so backloggd.so

all: $(EXEC) $(PLUGINS)
.PHONY:	all

ncist: main.o plugin.o
	$(CC) -o $@ $^ -lncurses

%.o: %.c
	$(CC) -o $@ $< -c $(CC_FLAGS)

%.so: %.c
	$(CC) -o plugins/$@ $< $(CC_FLAGS) -fPIC -shared -lcurl -lcjson

clean:
	rm -f ncist plugins/* *.o
.PHONY: clean
