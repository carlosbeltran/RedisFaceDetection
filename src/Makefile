DRAPI = `pkg-config --cflags --libs glib-2.0 --libs hiredis --libs opencv`

all:capture.cpp
	g++ -o capture $< -lhiredis -fpermissive $(DRAPI)
clean:
	rm -f capture
boxes:boxparser.cpp
	g++ -o boxparser $< $(DRAPI)
