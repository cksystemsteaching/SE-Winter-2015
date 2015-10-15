CC			:= clang
CC_OPT			:= -w -m32 -D'main(a, b)=main(int argc, char **argv)'
EXEC			:= selfie
MEM_SIZE		:= 32
TEST_LIST_OPT		:= -t

all: self_host test clean

self_host:
	$(CC) $(CC_OPT) -o $(EXEC) selfie.c
	touch out
	./$(EXEC) -c < selfie.c
	mv out $(EXEC).mips1
	touch out
	./$(EXEC) -m $(MEM_SIZE) $(EXEC).mips1 < selfie.c
	mv out $(EXEC).mips2

	echo '                                                 '
	echo '*** ---------- *** ---------- *** ----------- ***'
	diff -s $(EXEC).mips1 $(EXEC).mips2
	echo '*** ---------- *** ---------- *** ----------- ***'
	echo '						       '

test:
	./$(EXEC) -m $(MEM_SIZE) $(EXEC).mips1 $(TEST_LIST_OPT)

clean:
	rm *.mips*
	rm $(EXEC)
