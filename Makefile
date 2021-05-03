s-talk:	s-talk.o List.o
		gcc -pthread -o s-talk s-talk.o List.o

s-talk.o: s-talk.c List.c
		gcc -c s-talk.c

List.o: List.c List.h
		gcc -c List.c

clean:
		rm s-talk.o s-talk List.o
