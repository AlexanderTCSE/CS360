pairsofwords: main.o crc64.o hash.o getWord.o
	gcc -g -o pairsofwords main.o crc64.o hash.o getWord.o

main.o: main.c include.h
	gcc -c main.c

crc64.o: crc64.c include.h
	gcc -c crc64.c

hash.o: hash.c include.h crc64.c
	gcc -c hash.c

clean:
	rm -f pairsofwords main.o crc64.o hash.o getWord.o
