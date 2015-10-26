CC			:= clang
CC_OPT			:= -w -m32 -D'main(a, b)=main(int argc, char **argv)'
EXEC			:= selfie
MEM_SIZE		:= 32
TEST_INPUT		:= test.c
TEST_BINARY		:= myprog

all: test_binary

self_host:
	$(CC) $(CC_OPT) -o $(EXEC) $(EXEC).c
	touch out
	./$(EXEC) -c < $(EXEC).c
	mv out $(EXEC).mips1
	touch out
	./$(EXEC) -m $(MEM_SIZE) $(EXEC).mips1 < selfie.c
	mv out $(EXEC).mips2

	echo '                                                 '
	echo '*** ---------- *** ---------- *** ----------- ***'
	diff -s $(EXEC).mips1 $(EXEC).mips2
	echo '*** ---------- *** ---------- *** ----------- ***'
	echo '						       '

test_binary:
	$(CC) $(CC_OPT) -o $(EXEC) $(EXEC).c
	touch out
	./$(EXEC) < $(EXEC).c
	mv out $(EXEC).mips1
	touch out
	./$(EXEC) < $(TEST_INPUT)
	mv out $(TEST_BINARY)
	./$(EXEC) -m $(MEM_SIZE) $(TEST_BINARY)

compile:
	$(CC) $(CC_OPT) -o $(EXEC) selfie.c

self_execute:
	$(CC) $(CC_OPT) -o $(EXEC) selfie.c
	touch out
	./$(EXEC) -c < $(INPUT)
	mv out $(EXEC).mips1
	touch out
	./$(EXEC) -m 64 $(EXEC).mips1 -m $(MEM_SIZE) $(EXEC).mips1 < $(INPUT)
	mv out $(EXEC).mips2
	diff -s $(EXEC).mips1 $(EXEC).mips2

clean:
	rm -f *.mips*
	rm -f $(EXEC)
	rm -f out
	rm -f $(TEST_BINARY)
