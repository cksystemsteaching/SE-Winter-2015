CC			:= gcc
CC_OPT			:= -w -m32 -Wl,--unresolved-symbols=ignore-in-object-files -D'main(a, b)=main(int argc, char **argv)'
EXEC			:= selfie
MEM_SIZE		:= 32
RTS1			:= 2
RTS2			:= 10
RTS3			:= 100
TEST_INPUT		:= test.c
TEST_BINARY		:= myprog.mips
OS_BINARY		:= os.mips
KERNEL_BINARY		:= kernel.mips

all: test_binary

clena: clean
clan: clean
clane: clean
claen: clean
cean: clean
calen: clean
clea: clean
cclean: clean

compile:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)

test_binary:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)
	touch $(TEST_BINARY)
	touch $(KERNEL_BINARY)
	touch $(OS_BINARY)
	./$(EXEC) -c $(TEST_INPUT) -o $(TEST_BINARY)
	./$(EXEC) -c $(EXEC).c -o $(KERNEL_BINARY)
	./$(EXEC) -c $(EXEC).c -o $(OS_BINARY)
	./$(EXEC) -k 64 $(KERNEL_BINARY) ./$(EXEC) -l $(OS_BINARY) -os 32 -m 16 $(TEST_BINARY)

debug:
	ggdb --args ./$(EXEC) -k 64 $(KERNEL_BINARY) ./$(EXEC) -l $(OS_BINARY) -os 32 -m 16 $(TEST_BINARY)

clean:
	rm -f *.mips*
	rm -f $(EXEC)
	rm -f out
	rm -f $(TEST_BINARY)
	rm -f out.txt
	rm -f *~
	rm -f *.s
