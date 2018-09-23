P=quinn_p1
R=Filereader
CFLAGS= -g -Wall

all: $(P)

$(P): $(P).o $(R).o
	g++ $(CFLAGS) $(P).o $(R).o -o $(P)

$(P).o: $(P).cpp $(P).h $(R).h
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o

$(R).o: $(R).cpp $(R).h
	g++ $(CFLAGS) -c $(R).cpp -o $(R).o

prog1_accept: $(P)
	$(P) prog1_sample.txt '$000111$' 500
	
prog1_reject: $(P)
	$(P) prog1_sample.txt '$00011$' 500
	
prog1_quit: $(P)
	$(P) prog1_sample.txt '$000111$' 10
	
clean:
	rm -rf $(P)
	rm -rf $(P).o
	rm -rf $(R).o