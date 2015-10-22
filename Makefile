CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc

default: a1

a1: selfie
	./selfie -c < test.c
	mv out test
	./selfie -m 16 test

selfie: selfie.c
	$(CC) $(CFLAGS) $< -o selfie
	touch out


%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie out *~

