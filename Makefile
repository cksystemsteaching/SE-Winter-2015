CFLAGS=-g
LDFLAGS=
CC=/usr/bin/gcc
PAGER=more
default: selfie



selfie: selfie.c
	cat README 
	$(CC) $(CFLAGS) $< -o selfie
	./selfie -c init.c -o init
	./selfie -c kernel.c -k 64 init



%.o: %c
	$(CC) -c $<

clean:
	-rm -rf $(OBJECTS) $(PROGRAMS) selfie test init *~

