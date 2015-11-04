
CC= g++

re: re2post.o operator.o main.o 
	$(CC) -o $@ $^



re2post.o: re2post.c re2post.h operator.h
	$(CC) -c $<

operator.o: operator.c operator.h
	$(CC) -c $<

main.o: main.c re2post.h operator.h
	$(CC) -c $<

clean:
	rm -f *.o re.exe
