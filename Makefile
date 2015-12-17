CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc

default: a6

a6: selfie
	./build.sh



selfie: selfie.c
	$(CC) $(CFLAGS) $< -o selfie


%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie test init *~

