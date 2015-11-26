CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc

default: a4

a4: selfie
	./selfie -c test.c -m 128

selfie: selfie.c
	$(CC) $(CFLAGS) $< -o selfie


%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie out test *~

