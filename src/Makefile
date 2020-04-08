DRAPI = `pkg-config --cflags --libs glib-2.0 --libs hiredis`

all:capture.cpp
	g++ -o capture $< -lhiredis -fpermissive $(DRAPI)
clean:
	rm -f capture
