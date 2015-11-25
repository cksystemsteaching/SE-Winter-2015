CC			:= clang
CC_OPT			:= -w -m32 -D'main(a, b)=main(int argc, char **argv)'
EXEC			:= selfie
MEM_SIZE		:= 32
RTS1			:= 2
RTS2			:= 10
RTS3			:= 100
TEST_INPUT		:= test.c
TEST_BINARY		:= myprog

all: test_binary

clena: clean
clan: clean
clane: clean
claen: clean
cean: clean
calen: clean
clea: clean
cclean: clean

self_host:
	touch $(EXEC)1.mips
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)
	touch $(EXEC)2.mips
	./$(EXEC) -c $(EXEC).c -o $(EXEC)1.mips -m $(MEM_SIZE) -c $(EXEC).c -o $(EXEC)2.mips
	diff -s $(EXEC)1.mips $(EXEC)2.mips

test_different_time_slices:
	./modifyTimeslice.sh $(RTS1) $(RTS2) $(RTS3)

test_binary:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)
	touch $(TEST_BINARY)
	./$(EXEC) -c $(TEST_INPUT) -o $(TEST_BINARY)
	./$(EXEC) -l $(TEST_BINARY) -m $(MEM_SIZE)

compile:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)

self_execute:
	$(CC) $(CC_OPT) $(EXEC).c -o $(EXEC)
	./$(EXEC) -c $(EXEC).c -o $(EXEC)1.mips -m 64 -l $(EXEC)1.mips -m $(MEM_SIZE) -c $(EXEC).c -o $(EXEC)2.mips
	diff -s $(EXEC)1.mips $(EXEC)2.mips

clean:
	rm -f *.mips*
	rm -f $(EXEC)
	rm -f out
	rm -f $(TEST_BINARY)
	rm -f out.txt
	rm -f *~
	rm -f *.s
