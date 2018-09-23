P=Turing
R=Filereader
CFLAGS= -g -Wall

all: $(P)

$(P): $(P).o $(R).o
	g++ $(CFLAGS) $(P).o $(R).o -o $(P)

$(P).o: $(P).cpp $(P).h $(R).h
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o

$(R).o: $(R).cpp $(R).h
	g++ $(CFLAGS) -c $(R).cpp -o $(R).o

clean:
	rm -rf $(P)
	rm -rf $(P).o
	rm -rf $(R).o