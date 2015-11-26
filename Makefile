CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc

default: a5

a5: selfie
	./selfie -c test.c -m 3

selfie: selfie.c
	$(CC) $(CFLAGS) $< -o selfie


%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie test *~

