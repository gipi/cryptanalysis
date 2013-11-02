CPPFLAGS=-Wall

BINARIES=spn tweetcypher

all: $(BINARIES)

clean:
	rm -f *.o $(BINARIES)
