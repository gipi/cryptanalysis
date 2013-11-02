CPPFLAGS=-Wall

BINARIES=spn

all: $(BINARIES)

clean:
	rm -f *.o $(BINARIES)
