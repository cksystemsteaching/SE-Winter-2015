CC			:= clang
CC_OPT			:= -w -m32 -D'main(a, b)=main(int argc, char **argv)'
EXEC			:= selfie
MEM_SIZE		:= 32
RTS1			:= 2
RTS2			:= 10
RTS3			:= 100
TEST_INPUT		:= test.c
TEST_BINARY		:= myprog.mips

all: test_binary

clena: clean
clan: clean
clane: clean
claen: clean
cean: clean
calen: clean
clea: clean
cclean: clean

test_binary:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)
	touch $(TEST_BINARY)
	./$(EXEC) -c $(TEST_INPUT) -o $(TEST_BINARY)
	./$(EXEC) -c $(EXEC).c -o $(EXEC).mips -m $(MEM_SIZE) -u $(TEST_BINARY)

clean:
	rm -f *.mips*
	rm -f $(EXEC)
	rm -f out
	rm -f $(TEST_BINARY)
	rm -f out.txt
	rm -f *~
	rm -f *.s
