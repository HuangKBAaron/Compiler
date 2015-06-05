comp: main.o scanner.o parser.o semantics.o
    g++ -o comp main.o scanner.o parser.o semantics.o

main.o: main.cpp scanner.h parser.h semantics.h
    g++ -c main.cpp

scanner.o: scanner.cpp scanner.h
    g++ -c scanner.cpp

parser.o: parser.cpp parser.h
    g++ -c parser.cpp

semantics.o: semantics.cpp semantics.h
    g++ -c semantics.cpp
