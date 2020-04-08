DRAPI = `pkg-config --cflags --libs glib-2.0 --libs hiredis`

all:capture.c
	gcc -o capture $< -lhiredis $(DRAPI)
clean:
	rm -f capture
