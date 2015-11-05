CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc

default: a3

a3: selfie
	@touch out
	./selfie -c < test.c
	@mv out test
	./selfie -m 128 test

selfie: selfie.c
	$(CC) $(CFLAGS) $< -o selfie
	touch out


%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie out test *~

