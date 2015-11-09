
CC= g++ -std=c++11

re: nfa.o re2post.o operator.o main.o 
	$(CC) -o $@ $^


nfa.o: nfa.c nfa.h
	$(CC) -c $<

re2post.o: re2post.c re2post.h operator.h
	$(CC) -c $<

operator.o: operator.c operator.h
	$(CC) -c $<

main.o: main.c re2post.h operator.h
	$(CC) -c $<

clean:
	rm -f *.o *.exe output/*
