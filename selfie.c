// Copyright (c) 2015, the Selfie Project authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is
// governed by a BSD license that can be found in the LICENSE file.
//
// Selfie is a project of the Computational Systems Group at the
// Department of Computer Sciences of the University of Salzburg
// in Austria. For further information and code please refer to:
//
// http://selfie.cs.uni-salzburg.at
//
// The Selfie Project provides an educational platform for teaching
// undergraduate and graduate students the design and implementation
// of programming languages and runtime systems. The focus is on the
// construction of compilers, libraries, operating systems, and even
// virtual machine monitors. The common theme is to identify and
// resolve self-reference in systems code which is seen as the key
// challenge when teaching systems engineering, hence the name.
//
// Selfie is a fully self-referential 4k-line C implementation of:
//
// 1. a self-compiling compiler called cstarc that compiles
//    a tiny but powerful subset of C called C Star (C*) to
//    a tiny but powerful subset of MIPS32 called MIPSter,
// 2. a self-executing emulator called mipster that executes
//    MIPSter code including itself when compiled with cstarc, and
// 3. a tiny C* library called libcstar utilized by cstarc and mipster.
//
// Selfie is kept minimal for simplicity and implemented in a single file.
// There is no linker, assembler, or debugger. However, there is minimal
// operating system support in the form of MIPS32 o32 system calls built
// into the emulator. Selfie is meant to be extended in numerous ways.
//
// C* is a tiny Turing-complete subset of C that includes dereferencing
// (the * operator) but excludes data structures, Boolean expressions, and
// many other features. There are only signed 32-bit integers and pointers,
// and character constants for constructing word-aligned strings manually.
// This choice turns out to be helpful for students to understand the
// true role of composite data structures such as arrays and records.
// Bitwise operations are implemented in libcstar using signed integer
// arithmetics helping students gain true understanding of two's complement.
// C* is supposed to be close to the minimum necessary for implementing
// a self-compiling, single-pass, recursive-descent compiler. C* can be
// taught in around two weeks of classes depending on student background.
//
// The compiler can readily be extended to compile features missing in C*
// and to improve performance of the generated code. The compiler generates
// MIPSter executables that can directly be executed by the emulator or
// written to a file in a simple, custom-defined format. Support of standard
// MIPS32 ELF binaries should be easy but remains future work.
//
// MIPSter is a tiny Turing-complete subset of the MIPS32 instruction set.
// It only features arithmetic, memory, and control-flow instructions but
// neither bitwise nor byte-level instructions. MIPSter can be properly
// explained in a single week of classes.
//
// The emulator implements minimal operating system support that is meant
// to be extended by students, first as part of the emulator, and then
// ported to run on top of it, similar to an actual operating system or
// virtual machine monitor. The fact that the emulator can execute itself
// helps exposing the self-referential nature of that challenge.
//
// Selfie is the result of many years of teaching systems engineering.
// The design of the compiler is inspired by the Oberon compiler of
// Professor Niklaus Wirth from ETH Zurich.

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     L I B R A R Y     ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

void initLibrary();

int twoToThePowerOf(int p);
int leftShift(int n, int b);
int rightShift(int n, int b);

int  stringLength(int *s);
void stringReverse(int *s);
int  stringCompare(int *s, int *t);

int  atoi(int *s);
int* itoa(int n, int *s, int b, int a);

void print(int *s);
void println();

void printCharacter(int character);
void printString(int *s);

void exit(int code);
int* malloc(int size);

// ------------------------ GLOBAL CONSTANTS -----------------------

int CHAR_EOF          = -1; // end of file
int CHAR_TAB          = 9;  // ASCII code 9  = tabulator
int CHAR_LF           = 10; // ASCII code 10 = line feed
int CHAR_CR           = 13; // ASCII code 13 = carriage return
int CHAR_SPACE        = ' ';
int CHAR_SEMICOLON    = ';';
int CHAR_PLUS         = '+';
int CHAR_DASH         = '-';
int CHAR_ASTERISK     = '*';
int CHAR_HASH         = '#';
int CHAR_SLASH        = '/';
int CHAR_UNDERSCORE   = '_';
int CHAR_EQUAL        = '=';
int CHAR_LPARENTHESIS = '(';
int CHAR_RPARENTHESIS = ')';
int CHAR_LBRACE       = '{';
int CHAR_RBRACE       = '}';
int CHAR_COMMA        = ',';
int CHAR_LT           = '<';
int CHAR_GT           = '>';
int CHAR_EXCLAMATION  = '!';
int CHAR_PERCENTAGE   = '%';
int CHAR_SINGLEQUOTE  = 39; // ASCII code 39 = '
int CHAR_DOUBLEQUOTE  = '"';

int *power_of_two_table;

int INT_MAX; // maximum numerical value of an integer
int INT_MIN; // minimum numerical value of an integer

int *string_buffer;

// ------------------------- INITIALIZATION ------------------------

void initLibrary() {
    int i;

    power_of_two_table = malloc(31*4);

    *power_of_two_table = 1; // 2^0

    i = 1;

    while (i < 31) {
        *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;

        i = i + 1;
    }

    // computing INT_MAX and INT_MIN without overflows
    INT_MAX = (twoToThePowerOf(30) - 1) * 2 + 1;
    INT_MIN = -INT_MAX - 1;

    // accommodate 32-bit numbers for itoa
    string_buffer = malloc(33);
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     C O M P I L E R   ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- SCANNER ----------------------------
// -----------------------------------------------------------------

void initScanner();

void printSymbol(int symbol);
void printLineNumber(int* message);

void syntaxErrorMessage(int *message);
void syntaxErrorCharacter(int character);

int isCharacterWhitespace();
int findNextCharacter();
int isCharacterLetter();
int isCharacterDigit();
int isCharacterLetterOrDigitOrUnderscore();
int isNotDoubleQuoteOrEOF();
int identifierStringMatch(int stringIndex);
int identifierOrKeyword();
int getSymbol();

// ------------------------ GLOBAL CONSTANTS -----------------------

int SYM_EOF          = -1; // end of file
int SYM_IDENTIFIER   = 0;  // identifier
int SYM_INTEGER      = 1;  // integer
int SYM_VOID         = 2;  // VOID
int SYM_INT          = 3;  // INT
int SYM_SEMICOLON    = 4;  // ;
int SYM_IF           = 5;  // IF
int SYM_ELSE         = 6;  // ELSE
int SYM_PLUS         = 7;  // +
int SYM_MINUS        = 8;  // -
int SYM_ASTERISK     = 9;  // *
int SYM_DIV          = 10; // /
int SYM_EQUALITY     = 11; // ==
int SYM_ASSIGN       = 12; // =
int SYM_LPARENTHESIS = 13; // (
int SYM_RPARENTHESIS = 14; // )
int SYM_LBRACE       = 15; // {
int SYM_RBRACE       = 16; // }
int SYM_WHILE        = 17; // WHILE
int SYM_RETURN       = 18; // RETURN
int SYM_COMMA        = 19; // ,
int SYM_LT           = 20; // <
int SYM_LEQ          = 21; // <=
int SYM_GT           = 22; // >
int SYM_GEQ          = 23; // >=
int SYM_NOTEQ        = 24; // !=
int SYM_MOD          = 25; // %
int SYM_CHARACTER    = 26; // character
int SYM_STRING       = 27; // string

int *SYMBOLS; // array of strings representing symbols

int maxIdentifierLength = 64; // maximum number of characters in an identifier
int maxIntegerLength    = 10; // maximum number of characters in an integer
int maxStringLength     = 64; // maximum number of characters in a string

// ------------------------ GLOBAL VARIABLES -----------------------

int lineNumber = 1; // current Line Number for error reporting

int *identifier = (int*) 0; // stores scanned identifier as string
int *integer    = (int*) 0; // stores scanned integer as string
int *string     = (int*) 0; // stores scanned string

int constant = 0; // stores numerical value of scanned integer or character

int initialValue = 0; // stores initial value of variable definitions

int mayBeINTMINConstant = 0; // support INT_MIN constant
int isINTMINConstant    = 0;

int character; // most recently read character
int symbol;    // most recently recognized symbol

// ------------------------- INITIALIZATION ------------------------

void initScanner () {
    SYMBOLS = malloc(28*4);

    *(SYMBOLS + SYM_IDENTIFIER)   = (int) "identifier";
    *(SYMBOLS + SYM_INTEGER)      = (int) "integer";
    *(SYMBOLS + SYM_VOID)         = (int) "void";
    *(SYMBOLS + SYM_INT)          = (int) "int";
    *(SYMBOLS + SYM_SEMICOLON)    = (int) ";";
    *(SYMBOLS + SYM_IF)           = (int) "if";
    *(SYMBOLS + SYM_ELSE)         = (int) "else";
    *(SYMBOLS + SYM_PLUS)         = (int) "+";
    *(SYMBOLS + SYM_MINUS)        = (int) "-";
    *(SYMBOLS + SYM_ASTERISK)     = (int) "*";
    *(SYMBOLS + SYM_DIV)          = (int) "/";
    *(SYMBOLS + SYM_EQUALITY)     = (int) "==";
    *(SYMBOLS + SYM_ASSIGN)       = (int) "=";
    *(SYMBOLS + SYM_LPARENTHESIS) = (int) "(";
    *(SYMBOLS + SYM_RPARENTHESIS) = (int) ")";
    *(SYMBOLS + SYM_LBRACE)       = (int) "{";
    *(SYMBOLS + SYM_RBRACE)       = (int) "}";
    *(SYMBOLS + SYM_WHILE)        = (int) "while";
    *(SYMBOLS + SYM_RETURN)       = (int) "return";
    *(SYMBOLS + SYM_COMMA)        = (int) ",";
    *(SYMBOLS + SYM_LT)           = (int) "<";
    *(SYMBOLS + SYM_LEQ)          = (int) "<=";
    *(SYMBOLS + SYM_GT)           = (int) ">";
    *(SYMBOLS + SYM_GEQ)          = (int) ">=";
    *(SYMBOLS + SYM_NOTEQ)        = (int) "!=";
    *(SYMBOLS + SYM_MOD)          = (int) "%";
    *(SYMBOLS + SYM_CHARACTER)    = (int) "character";
    *(SYMBOLS + SYM_STRING)       = (int) "string";

    character  = getchar();
    symbol     = SYM_EOF;
}

// -----------------------------------------------------------------
// ------------------------- SYMBOL TABLE --------------------------
// -----------------------------------------------------------------

void createSymbolTableEntry(int which, int *string, int data, int class, int type, int value);
int* getSymbolTableEntry(int *string, int class, int *symbol_table);

int* getNext(int *entry);
int* getString(int *entry);
int  getData(int *entry);
int  getClass(int *entry);
int  getType(int *entry);
int  getValue(int *entry);
int  getRegister(int *entry);

void setNext(int *entry, int *next);
void setString(int *entry, int *identifier);
void setData(int *entry, int data);
void setClass(int *entry, int class);
void setType(int *entry, int type);
void setValue(int *entry, int value);
void setRegister(int *entry, int reg);

// ------------------------ GLOBAL CONSTANTS -----------------------

// classes
int VARIABLE = 1;
int FUNCTION = 2;
int STRING   = 3;

// types
int INT_T     = 1;
int INTSTAR_T = 2;
int VOID_T    = 3;

// symbol tables
int GLOBAL_TABLE = 1;
int LOCAL_TABLE  = 2;

// ------------------------ GLOBAL VARIABLES -----------------------

// table pointers
int *global_symbol_table = (int*) 0;
int *local_symbol_table  = (int*) 0;

// -----------------------------------------------------------------
// ---------------------------- PARSER -----------------------------
// -----------------------------------------------------------------

void initParser();

int isNotRbraceOrEOF();
int isExpression();
int isStarOrDivOrModulo();
int isPlusOrMinus();
int isComparison();

int lookForFactor();
int lookForStatement();
int lookForType();

void save_temporaries();
void restore_temporaries(int numberOfTemporaries);

void syntaxErrorSymbol(int expected);
void syntaxErrorUnexpected();
int* putType(int type);
void typeWarning(int expected, int found);

int* getVariable(int *variable);
int  load_variable(int *variable);
void load_integer();
void load_string();

int  help_call_codegen(int *entry, int *procedure);
void help_procedure_prologue(int localVariables);
void help_procedure_epilogue(int parameters);

int  gr_call(int *procedure);
int  gr_factor();
int  gr_term();
int  gr_simpleExpression();
int  gr_expression();
void gr_while();
void gr_if();
void gr_return(int returnType);
void gr_statement();
int  gr_type();
void gr_variable(int offset);
void gr_initialization(int *name, int offset, int type);
void gr_procedure(int *procedure, int returnType);
void gr_cstar();

// ------------------------ GLOBAL CONSTANTS -----------------------

int maxBinaryLength;

// ------------------------ GLOBAL VARIABLES -----------------------

int allocatedTemporaries = 0; // number of allocated temporaries

int allocatedMemory = 0; // number of bytes for global variables and strings

int mainJumpAddress = 0; // address of main function
int returnBranches  = 0; // fixup chain for return statements

int *currentProcedureName = (int*) 0; // name of currently parsed procedure

// ------------------------- INITIALIZATION ------------------------

void
initParser() {
    // set maximum code length in bytes for emitting code
    maxBinaryLength = twoToThePowerOf(17);
}

// -----------------------------------------------------------------
// ---------------------- MACHINE CODE LIBRARY ---------------------
// -----------------------------------------------------------------

void emitLeftShiftBy(int b);
void emitMainEntry();

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// -------------------     I N T E R F A C E     -------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- REGISTER ---------------------------
// -----------------------------------------------------------------

void initRegister();

void printRegister(int reg);

// ------------------------ GLOBAL CONSTANTS -----------------------

int REG_ZR = 0;
int REG_AT = 1;
int REG_V0 = 2;
int REG_V1 = 3;
int REG_A0 = 4;
int REG_A1 = 5;
int REG_A2 = 6;
int REG_A3 = 7;
int REG_T0 = 8;
int REG_T1 = 9;
int REG_T2 = 10;
int REG_T3 = 11;
int REG_T4 = 12;
int REG_T5 = 13;
int REG_T6 = 14;
int REG_T7 = 15;
int REG_S0 = 16;
int REG_S1 = 17;
int REG_S2 = 18;
int REG_S3 = 19;
int REG_S4 = 20;
int REG_S5 = 21;
int REG_S6 = 22;
int REG_S7 = 23;
int REG_T8 = 24;
int REG_T9 = 25;
int REG_K0 = 26;
int REG_K1 = 27;
int REG_GP = 28;
int REG_SP = 29;
int REG_FP = 30;
int REG_RA = 31;

int *REGISTERS; // array of strings representing registers

// ------------------------- INITIALIZATION ------------------------

void initRegister() {
    REGISTERS = malloc(32*4);

    *(REGISTERS + REG_ZR) = (int) "$zero";
    *(REGISTERS + REG_AT) = (int) "$at";
    *(REGISTERS + REG_V0) = (int) "$v0";
    *(REGISTERS + REG_V1) = (int) "$v1";
    *(REGISTERS + REG_A0) = (int) "$a0";
    *(REGISTERS + REG_A1) = (int) "$a1";
    *(REGISTERS + REG_A2) = (int) "$a2";
    *(REGISTERS + REG_A3) = (int) "$a3";
    *(REGISTERS + REG_T0) = (int) "$t0";
    *(REGISTERS + REG_T1) = (int) "$t1";
    *(REGISTERS + REG_T2) = (int) "$t2";
    *(REGISTERS + REG_T3) = (int) "$t3";
    *(REGISTERS + REG_T4) = (int) "$t4";
    *(REGISTERS + REG_T5) = (int) "$t5";
    *(REGISTERS + REG_T6) = (int) "$t6";
    *(REGISTERS + REG_T7) = (int) "$t7";
    *(REGISTERS + REG_S0) = (int) "$s0";
    *(REGISTERS + REG_S1) = (int) "$s1";
    *(REGISTERS + REG_S2) = (int) "$s2";
    *(REGISTERS + REG_S3) = (int) "$s3";
    *(REGISTERS + REG_S4) = (int) "$s4";
    *(REGISTERS + REG_S5) = (int) "$s5";
    *(REGISTERS + REG_S6) = (int) "$s6";
    *(REGISTERS + REG_S7) = (int) "$s7";
    *(REGISTERS + REG_T8) = (int) "$t8";
    *(REGISTERS + REG_T9) = (int) "$t9";
    *(REGISTERS + REG_K0) = (int) "$k0";
    *(REGISTERS + REG_K1) = (int) "$k1";
    *(REGISTERS + REG_GP) = (int) "$gp";
    *(REGISTERS + REG_SP) = (int) "$sp";
    *(REGISTERS + REG_FP) = (int) "$fp";
    *(REGISTERS + REG_RA) = (int) "$ra";
}

// -----------------------------------------------------------------
// ---------------------------- ENCODER ----------------------------
// -----------------------------------------------------------------

int encodeRFormat(int opcode, int rs, int rt, int rd, int function);
int encodeIFormat(int opcode, int rs, int rt, int immediate);
int encodeJFormat(int opcode, int instr_index);

int getOpcode(int instruction);
int getRS(int instruction);
int getRT(int instruction);
int getRD(int instruction);
int getFunction(int instruction);
int getImmediate(int instruction);
int getInstrIndex(int instruction);
int signExtend(int immediate);

// -----------------------------------------------------------------
// ---------------------------- DECODER ----------------------------
// -----------------------------------------------------------------

void initDecoder();

void printOpcode(int opcode);
void printFunction(int function);

void decode();
void decodeRFormat();
void decodeIFormat();
void decodeJFormat();

// ------------------------ GLOBAL CONSTANTS -----------------------

int OP_SPECIAL = 0;
int OP_J       = 2;
int OP_JAL     = 3;
int OP_BEQ     = 4;
int OP_BNE     = 5;
int OP_ADDIU   = 9;
int OP_LW      = 35;
int OP_SW      = 43;
    
int *OPCODES; // array of strings representing MIPS opcodes

int FCT_NOP     = 0;
int FCT_JR      = 8;
int FCT_SYSCALL = 12;
int FCT_MFHI    = 16;
int FCT_MFLO    = 18;
int FCT_MULTU   = 25;
int FCT_DIVU    = 27;
int FCT_ADDU    = 33;
int FCT_SUBU    = 35;
int FCT_SLT     = 42;
int FCT_TEQ     = 52;

int *FUNCTIONS; // array of strings representing MIPS functions

// ------------------------ GLOBAL VARIABLES -----------------------

int opcode      = 0;
int rs          = 0;
int rt          = 0;
int rd          = 0;
int immediate   = 0;
int function    = 0;
int instr_index = 0;

// ------------------------- INITIALIZATION ------------------------

void initDecoder() {
    OPCODES = malloc(44*4);

    *(OPCODES + OP_SPECIAL) = (int) "nop";
    *(OPCODES + OP_J)       = (int) "j";
    *(OPCODES + OP_JAL)     = (int) "jal";
    *(OPCODES + OP_BEQ)     = (int) "beq";
    *(OPCODES + OP_BNE)     = (int) "bne";
    *(OPCODES + OP_ADDIU)   = (int) "addiu";
    *(OPCODES + OP_LW)      = (int) "lw";
    *(OPCODES + OP_SW)      = (int) "sw";

    FUNCTIONS = malloc(53*4);

    *(FUNCTIONS + FCT_NOP)     = (int) "nop";
    *(FUNCTIONS + FCT_JR)      = (int) "jr";
    *(FUNCTIONS + FCT_SYSCALL) = (int) "syscall";
    *(FUNCTIONS + FCT_MFHI)    = (int) "mfhi";
    *(FUNCTIONS + FCT_MFLO)    = (int) "mflo";
    *(FUNCTIONS + FCT_MULTU)   = (int) "multu";
    *(FUNCTIONS + FCT_DIVU)    = (int) "divu";
    *(FUNCTIONS + FCT_ADDU)    = (int) "addu";
    *(FUNCTIONS + FCT_SUBU)    = (int) "subu";
    *(FUNCTIONS + FCT_SLT)     = (int) "slt";
    *(FUNCTIONS + FCT_TEQ)     = (int) "teq";
}

// -----------------------------------------------------------------
// ---------------------------- MEMORY -----------------------------
// -----------------------------------------------------------------

void initMemory(int size, int* name);

int tlb(int vaddr);

int  loadMemory(int vaddr);
void storeMemory(int vaddr, int data);

// ------------------------ GLOBAL VARIABLES -----------------------

int *memory;
int  memorySize;
int *memoryBumpPointer;
int  usedMemorySize;
int memoryStartAddress;

int *blockedQueue; // blocked processes, wait for unlock
int *readyQueue; // ready processes

int *currProcess;
int  counterProcesses;

int *segmentationTable;
int *currSegment;
int currSegmentPos;
int currSegmentSize;

int isEmulating=0;

int *binaryName;
int  binaryLength;

int lock = 0;
int lockID;

// ------------------------- INITIALIZATION ------------------------

void initMemory(int size, int *name) {
    memory     = malloc(size);
    memoryStartAddress = (int)memory;
    memoryBumpPointer = memory;
    currSegmentPos = (int)memory;
    usedMemorySize = 0;
    memorySize = size;
    currSegmentSize = memorySize;

    binaryName   = name;
    binaryLength = 0;
}

// -----------------------------------------------------------------
// ---------------------------- BINARY -----------------------------
// -----------------------------------------------------------------

void emitInstruction(int instruction);
void emitRFormat(int opcode, int rs, int rt, int rd, int function);
void emitIFormat(int opcode, int rs, int rt, int immediate);
void emitJFormat(int opcode, int instr_index);

void fixup_relative(int fromAddress);
void fixup_absolute(int fromAddress, int toAddress);
void fixlink_absolute(int fromAddress, int toAddress);

int copyStringToMemory(int *s, int a);

void emitBinary();
void loadBinary();

// -----------------------------------------------------------------
// --------------------------- SYSCALLS ----------------------------
// -----------------------------------------------------------------

void emitExit();
void syscall_exit();

void emitRead();
void syscall_read();

void emitWrite();
void syscall_write();

void emitOpen();
void syscall_open();

void emitMalloc();
void syscall_malloc();

void emitGetchar();
void syscall_getchar();

void emitPutchar();

void emitYield();
void syscall_yield();

void emitLock();
void syscall_lock();

void emitUnlock();
void syscall_unlock();

// ------------------------ GLOBAL CONSTANTS -----------------------

int SYSCALL_EXIT    = 4001;
int SYSCALL_READ    = 4003;
int SYSCALL_WRITE   = 4004;
int SYSCALL_OPEN    = 4005;
int SYSCALL_MALLOC  = 5001;
int SYSCALL_GETCHAR = 5002;
int SYSCALL_YIELD   = 6000;
int SYSCALL_LOCK	= 6001;
int SYSCALL_UNLOCK	= 6002;

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     E M U L A T O R   ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ------------------------- INSTRUCTIONS --------------------------
// -----------------------------------------------------------------

void fct_syscall();
void fct_nop();
void op_jal();
void op_j();
void op_beq();
void op_bne();
void op_addiu();
void fct_jr();
void op_lui();
void fct_mfhi();
void fct_mflo();
void fct_multu();
void fct_divu();
void fct_addu();
void fct_subu();
void op_lw();
void fct_slt();
void op_sw();
void op_teq();

void appendListElement(int *newElement, int *list);
void saveProcessState();
void printListElement(int *element);
void printList(int *list, int type);
int* findElementByKey(int key, int *list);
void setProcessState();
int* pollListHead(int *borders);
int* removeFirst(int *borders);
void switchProcess(int finished);
void continueExecuting();

// -----------------------------------------------------------------
// -------------------------- INTERPRETER --------------------------
// -----------------------------------------------------------------

void initInterpreter();

void printException(int enumber);

void exception_handler(int enumber);

void pre_debug();
void post_debug();

void fetch();
void execute();
void run();

void debug_boot(int memorySize);
void parse_args(int argc, int *argv);
void up_push(int value);
int  up_malloc(int size);
void up_copyArguments(int argc, int *argv);

int main_emulator(int argc, int *argv);

// ------------------------ GLOBAL CONSTANTS -----------------------

int debug_load = 0;

int debug_read    = 0;
int debug_write   = 0;
int debug_open    = 0;
int debug_malloc  = 0;
int debug_getchar = 0;

int debug_registers   = 0;
int debug_disassemble = 0;

int EXCEPTION_SIGNAL             = 1;
int EXCEPTION_ADDRESSERROR       = 2;
int EXCEPTION_UNKNOWNINSTRUCTION = 3;
int EXCEPTION_HEAPOVERFLOW       = 4;
int EXCEPTION_UNKNOWNSYSCALL     = 5;
int EXCEPTION_UNKNOWNFUNCTION    = 6;
int EXCEPTION_SEGMENTATIONFAULT  = 7;

int *EXCEPTIONS; // array of strings representing exceptions

// ------------------------ GLOBAL VARIABLES -----------------------

int *registers; // general purpose registers

int pc = 0; // program counter
int ir = 0; // instruction record

int reg_hi = 0; // hi register for multiplication/division
int reg_lo = 0; // lo register for multiplication/division

// ------------------------- INITIALIZATION ------------------------

void initInterpreter() {
    EXCEPTIONS = malloc(8*4);

    *(EXCEPTIONS + EXCEPTION_SIGNAL)             = (int) "signal";
    *(EXCEPTIONS + EXCEPTION_ADDRESSERROR)       = (int) "address error";
    *(EXCEPTIONS + EXCEPTION_UNKNOWNINSTRUCTION) = (int) "unknown instruction";
    *(EXCEPTIONS + EXCEPTION_HEAPOVERFLOW)       = (int) "heap overflow";
    *(EXCEPTIONS + EXCEPTION_UNKNOWNSYSCALL)     = (int) "unknown syscall";
    *(EXCEPTIONS + EXCEPTION_UNKNOWNFUNCTION)    = (int) "unknown function";
    *(EXCEPTIONS + EXCEPTION_SEGMENTATIONFAULT)  = (int) "segmentation fault [added]";

    registers = malloc(32*4);
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     L I B R A R Y     ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

int twoToThePowerOf(int p) {
    // assert: 0 <= p < 31
    return *(power_of_two_table + p);
}

int leftShift(int n, int b) {
    // assert: b >= 0;

    if (b > 30)
        return 0;
    else
        return n * twoToThePowerOf(b);
}

int rightShift(int n, int b) {
    // assert: b >= 0

    if (b > 30)
        return 0;
    else if (n >= 0)
        return n / twoToThePowerOf(b);
    else
        // works even if n == INT_MIN
        return ((n + 1) + INT_MAX) / twoToThePowerOf(b) +
            (INT_MAX / twoToThePowerOf(b) + 1);
}

int getCharacter(int *s, int i) {
    // assert: i >= 0
    int a;

    a = i / 4;

    return rightShift(leftShift(*(s + a), 24 - (i % 4) * 8), 24);
}

int* putCharacter(int *s, int i, int c) {
    // assert: i >= 0, all characters are 7-bit
    int a;

    a = i / 4;

    *(s + a) = (*(s + a) - leftShift(getCharacter(s, i), (i % 4) * 8)) + leftShift(c, (i % 4) * 8);
    
    return s;
}

int stringLength(int *s) {
    int i;

    i = 0;

    while (getCharacter(s, i) != 0)
        i = i + 1;

    return i;
}

void stringReverse(int *s) {
    int i;
    int j;
    int tmp;

    i = 0;
    j = stringLength(s) - 1;

    while (i < j) {
        tmp = getCharacter(s, i);
        
        putCharacter(s, i, getCharacter(s, j));
        putCharacter(s, j, tmp);

        i = i + 1;
        j = j - 1;
    }
}

int stringCompare(int *s, int *t) {
    int i;

    i = 0;

    while (1)
        if (getCharacter(s, i) == 0)
            if (getCharacter(t, i) == 0)
                return 1;
            else
                return 0;
        else if (getCharacter(s, i) == getCharacter(t, i))
            i = i + 1;
        else
            return 0;
}

int atoi(int *s) {
    int i;
    int n;

    i = 0;

    n = 0;

    while (getCharacter(s, i) != 0) {
        n = n * 10 + getCharacter(s, i) - '0';
        
        i = i + 1;
    }

    return n;
}

int* itoa(int n, int *s, int b, int a) {
    // assert: b in {2,4,8,10,16}

    int i;
    int sign;

    i = 0;

    sign = 0;

    if (n == 0) {
        putCharacter(s, 0, '0');

        i = 1;
    } else if (n < 0) {
        sign = 1;

        if (b == 10) {
            if (n == INT_MIN) {
                // rightmost decimal digit of 32-bit INT_MIN
                putCharacter(s, 0, '8');

                n = -(n / 10);
                i = i + 1;
            } else
                n = -n;
        } else {
            if (n == INT_MIN) {
                // rightmost non-decimal digit of INT_MIN
                putCharacter(s, 0, '0');

                n = (rightShift(INT_MIN, 1) / b) * 2;
                i = i + 1;
            } else
                n = rightShift(leftShift(n, 1), 1);
        }
    }

    while (n != 0) {
        if (n % b > 9)
            putCharacter(s, i, n % b - 10 + 'A');
        else
            putCharacter(s, i, n % b + '0');

        n = n / b;
        i = i + 1;

        if (i == 1) {
            if (sign) {
                if (b != 10)
                    n = n + (rightShift(INT_MIN, 1) / b) * 2;
            }
        }
    }

    if (b != 10) {
        while (i < a) {
            putCharacter(s, i, '0'); // align with zeros

            i = i + 1;
        }

        if (b == 16) {
            putCharacter(s, i, 'x');
            putCharacter(s, i + 1, '0');

            i = i + 2;
        }
    } else if (sign) {
        putCharacter(s, i, '-');

        i = i + 1;
    }

    putCharacter(s, i, 0); // null terminated string

    stringReverse(s);

    return s;
}

void print(int *s) {
    int i;

    i = 0;

    while (getCharacter(s, i) != 0) {
        putchar(getCharacter(s, i));

        i = i + 1;
    }
}

void println() {
    putchar(CHAR_LF);
}

void printCharacter(int character) {
    putchar(CHAR_SINGLEQUOTE);

    if (character == CHAR_EOF)
        print((int*) "end of file");
    else if (character == CHAR_TAB)
        print((int*) "tabulator");
    else if (character == CHAR_LF)
        print((int*) "line feed");
    else if (character == CHAR_CR)
        print((int*) "carriage return");
    else
        putchar(character);

    putchar(CHAR_SINGLEQUOTE);
}

void printString(int *s) {
    putchar(CHAR_DOUBLEQUOTE);

    print(s);
    
    putchar(CHAR_DOUBLEQUOTE);
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     C O M P I L E R   ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- SCANNER ----------------------------
// -----------------------------------------------------------------

void printSymbol(int symbol) {
    putchar(CHAR_DOUBLEQUOTE);

    if (symbol == SYM_EOF)
        print((int*) "end of file");
    else
        print((int*) *(SYMBOLS + symbol));

    putchar(CHAR_DOUBLEQUOTE);
}

void printLineNumber(int* message) {
    print((int*) "cstarc: ");
    print(message);
    print((int*) " in line ");
    print(itoa(lineNumber, string_buffer, 10, 0));
    print((int*) ": ");
}

void syntaxErrorMessage(int *message) {
    printLineNumber((int*) "error");

    print(message);
    
    println();
}

void syntaxErrorCharacter(int expected) {
    printLineNumber((int*) "error");

    printCharacter(expected);
    print((int*) " expected but ");

    printCharacter(character);
    print((int*) " found");

    println();
}

int isCharacterWhitespace() {
    if (character == CHAR_SPACE)
        return 1;
    else if (character == CHAR_TAB)
        return 1;
    else if (character == CHAR_LF)
        return 1;
    else if (character == CHAR_CR)
        return 1;
    else
        return 0;
}

int findNextCharacter() {
    int inComment;

    inComment = 0;

    while (1) {
        if (inComment) {
            character = getchar();

            if (character == CHAR_LF)
                inComment = 0;
            else if (character == CHAR_CR)
                inComment = 0;
            else if (character == CHAR_EOF)
                return character;

        } else if (isCharacterWhitespace()) {
            if (character == CHAR_LF)
                lineNumber = lineNumber + 1;
            else if (character == CHAR_CR)
                lineNumber = lineNumber + 1;

            character = getchar();

        } else if (character == CHAR_HASH) {
            character = getchar();
            inComment = 1;

        } else if (character == CHAR_SLASH) {
            character = getchar();
            if (character == CHAR_SLASH)
                inComment = 1;
            else {
                symbol = SYM_DIV;
                return character;
            }

        } else
            return character;
    }
}

int isCharacterLetter() {
    if (character >= 'a')
        if (character <= 'z')
            return 1;
        else
            return 0;
    else if (character >= 'A')
        if (character <= 'Z')
            return 1;
        else
            return 0;
    else
        return 0;
}

int isCharacterDigit() {
    if (character >= '0')
        if (character <= '9')
            return 1;
        else
            return 0;
    else
        return 0;
}

int isCharacterLetterOrDigitOrUnderscore() {
    if (isCharacterLetter())
        return 1;
    else if (isCharacterDigit())
        return 1;
    else if (character == CHAR_UNDERSCORE)
        return 1;
    else
        return 0;
}

int isNotDoubleQuoteOrEOF() {
    if (character == CHAR_DOUBLEQUOTE)
        return 0;
    else if (character == CHAR_EOF)
        return 0;
    else
        return 1;
}

int identifierStringMatch(int keyword) {
    return stringCompare(identifier, (int*) *(SYMBOLS + keyword));
}

int identifierOrKeyword() {
    if (identifierStringMatch(SYM_WHILE))
        return SYM_WHILE;
    if (identifierStringMatch(SYM_IF))
        return SYM_IF;
    if (identifierStringMatch(SYM_INT))
        return SYM_INT;
    if (identifierStringMatch(SYM_ELSE))
        return SYM_ELSE;
    if (identifierStringMatch(SYM_RETURN))
        return SYM_RETURN;
    if (identifierStringMatch(SYM_VOID))
        return SYM_VOID;
    else
        return SYM_IDENTIFIER;
}

int getSymbol() {
    int i;

    symbol = SYM_EOF;

    if (findNextCharacter() == CHAR_EOF)
        return SYM_EOF;
    else if (symbol == SYM_DIV)
        // check here because / was recognized instead of //
        return SYM_DIV;

    if (isCharacterLetter()) {
        identifier = malloc(maxIdentifierLength + 1);

        i = 0;

        while (isCharacterLetterOrDigitOrUnderscore()) {
            if (i >= maxIdentifierLength) {
                syntaxErrorMessage((int*) "identifier too long");
                exit(-1);
            }

            putCharacter(identifier, i, character);

            i = i + 1;

            character = getchar();
        }

        putCharacter(identifier, i, 0); // null terminated string

        symbol = identifierOrKeyword();

    } else if (isCharacterDigit()) {
        integer = malloc(maxIntegerLength + 1);

        i = 0;

        while (isCharacterDigit()) {
            if (i >= maxIntegerLength) {
                syntaxErrorMessage((int*) "integer out of bound");
                exit(-1);
            }

            putCharacter(integer, i, character);

            i = i + 1;
            
            character = getchar();
        }

        putCharacter(integer, i, 0); // null terminated string

        constant = atoi(integer);

        if (constant < 0) {
            if (constant == INT_MIN) {
                if (mayBeINTMINConstant)
                    isINTMINConstant = 1;
                else {
                    syntaxErrorMessage((int*) "integer out of bound");
                    exit(-1);
                }
            } else {
                syntaxErrorMessage((int*) "integer out of bound");
                exit(-1);
            }
        }

        symbol = SYM_INTEGER;

    } else if (character == CHAR_SINGLEQUOTE) {
        character = getchar();

        constant = 0;

        if (character == CHAR_EOF) {
            syntaxErrorMessage((int*) "reached end of file looking for a character constant");

            exit(-1);
        } else
            constant = character;

        character = getchar();

        if (character == CHAR_SINGLEQUOTE)
            character = getchar();
        else if (character == CHAR_EOF) {
            syntaxErrorCharacter(CHAR_SINGLEQUOTE);

            exit(-1);
        } else
            syntaxErrorCharacter(CHAR_SINGLEQUOTE);

        symbol = SYM_CHARACTER;

    } else if (character == CHAR_DOUBLEQUOTE) {
        character = getchar();

        string = malloc(maxStringLength + 1);

        i = 0;

        while (isNotDoubleQuoteOrEOF()) {
            if (i >= maxStringLength) {
                syntaxErrorMessage((int*) "string too long");
                exit(-1);
            }

            putCharacter(string, i, character);

            i = i + 1;
            
            character = getchar();
        }

        if (character == CHAR_DOUBLEQUOTE)
            character = getchar();
        else {
            syntaxErrorCharacter(CHAR_DOUBLEQUOTE);

            exit(-1);
        }

        putCharacter(string, i, 0); // null terminated string

        symbol = SYM_STRING;

    } else if (character == CHAR_SEMICOLON) {
        character = getchar();
        symbol = SYM_SEMICOLON;

    } else if (character == CHAR_PLUS) {
        character = getchar();
        symbol = SYM_PLUS;

    } else if (character == CHAR_DASH) {
        character = getchar();
        symbol = SYM_MINUS;

    } else if (character == CHAR_ASTERISK) {
        character = getchar();
        symbol = SYM_ASTERISK;

    } else if (character == CHAR_EQUAL) {
        character = getchar();
        if (character == CHAR_EQUAL) {
            character = getchar();
            symbol = SYM_EQUALITY;
        } else
            symbol = SYM_ASSIGN;

    } else if (character == CHAR_LPARENTHESIS) {
        character = getchar();
        symbol = SYM_LPARENTHESIS;

    } else if (character == CHAR_RPARENTHESIS) {
        character = getchar();
        symbol = SYM_RPARENTHESIS;

    } else if (character == CHAR_LBRACE) {
        character = getchar();
        symbol = SYM_LBRACE;

    } else if (character == CHAR_RBRACE) {
        character = getchar();
        symbol = SYM_RBRACE;

    } else if (character == CHAR_COMMA) {
        character = getchar();
        symbol = SYM_COMMA;

    } else if (character == CHAR_LT) {
        character = getchar();
        if (character == CHAR_EQUAL) {
            character = getchar();
            symbol = SYM_LEQ;
        } else
            symbol = SYM_LT;

    } else if (character == CHAR_GT) {
        character = getchar();
        if (character == CHAR_EQUAL) {
            character = getchar();
            symbol = SYM_GEQ;
        } else
            symbol = SYM_GT;

    } else if (character == CHAR_EXCLAMATION) {
        character = getchar();
        if (character == CHAR_EQUAL)
            character = getchar();
        else
            syntaxErrorCharacter(CHAR_EQUAL);

        symbol = SYM_NOTEQ;

    } else if (character == CHAR_PERCENTAGE) {
        character = getchar();
        symbol = SYM_MOD;

    } else {
        printLineNumber((int*) "error");
        print((int*) "found unknown character ");
        printCharacter(character);
        
        println();

        exit(-1);
    }

    return symbol;
}

// -----------------------------------------------------------------
// ------------------------- SYMBOL TABLE --------------------------
// -----------------------------------------------------------------

void createSymbolTableEntry(int whichTable, int *string, int data, int class, int type, int value) {
    int *newEntry;

    // symbol table entry:
    // +----+----------+
    // |  0 | next     | pointer to next entry
    // |  1 | string   | identifier string, string constant
    // |  2 | data     | VARIABLE: offset, FUNCTION: address, STRING: offset
    // |  3 | class    | VARIABLE, FUNCTION, STRING
    // |  4 | type     | INT_T, INTSTAR_T, VOID_T
    // |  5 | value    | VARIABLE: constant value
    // |  6 | register | REG_GP, REG_FP
    // +----+----------+

    newEntry = malloc(7 * 4);

    setString(newEntry, string);
    setData(newEntry, data);
    setClass(newEntry, class);
    setType(newEntry, type);
    setValue(newEntry, value);

    // create entry at head of symbol table
    if (whichTable == GLOBAL_TABLE) {
        setRegister(newEntry, REG_GP);
        setNext(newEntry, global_symbol_table);
        global_symbol_table = newEntry;
    } else {
        setRegister(newEntry, REG_FP);
        setNext(newEntry, local_symbol_table);
        local_symbol_table = newEntry;
    }
}

int* getSymbolTableEntry(int *string, int class, int *symbol_table) {
    while ((int) symbol_table != 0) {
        if (stringCompare(string, getString(symbol_table)))
            if (class == getClass(symbol_table))
                return symbol_table;
        
        // keep looking
        symbol_table = getNext(symbol_table);
    }

    return (int*) 0;
}

int* getNext(int *entry) {
    // cast only works if size of int and int* is equivalent
    return (int*) *entry;
}

int* getString(int *entry) {
    // cast only works if size of int and int* is equivalent
    return (int*) *(entry + 1);
}

int getData(int *entry) {
    return *(entry + 2);
}

int getClass(int *entry) {
    return *(entry + 3);
}

int getType(int *entry) {
    return *(entry + 4);
}

int getValue(int *entry) {
    return *(entry + 5);
}

int getRegister(int *entry) {
    return *(entry + 6);
}

void setNext(int *entry, int *next) {
    // cast only works if size of int and int* is equivalent
    *entry = (int) next;
}

void setString(int *entry, int *identifier) {
    // cast only works if size of int and int* is equivalent
    *(entry + 1) = (int) identifier;
}

void setData(int *entry, int data) {
    *(entry + 2) = data;
}

void setClass(int *entry, int class) {
    *(entry + 3) = class;
}

void setType(int *entry, int type) {
    *(entry + 4) = type;
}

void setValue(int *entry, int value) {
    *(entry + 5) = value;
}

void setRegister(int *entry, int reg) {
    *(entry + 6) = reg;
}

// -----------------------------------------------------------------
// ---------------------------- PARSER -----------------------------
// -----------------------------------------------------------------

int isNotRbraceOrEOF() {
    if (symbol == SYM_RBRACE)
        return 0;
    else if(symbol == SYM_EOF)
        return 0;
    else
        return 1;
}

int isExpression() {
    if (symbol == SYM_MINUS)
        return 1;
    else if (symbol == SYM_LPARENTHESIS)
        return 1;
    else if (symbol == SYM_IDENTIFIER)
        return 1;
    else if (symbol == SYM_INTEGER)
        return 1;
    else if (symbol == SYM_ASTERISK)
        return 1;
    else if (symbol == SYM_STRING)
        return 1;
    else if (symbol == SYM_CHARACTER)
        return 1;
    else
        return 0;
}

int isConstant() {
    if (symbol == SYM_INTEGER)
        return 1;
    else if (symbol == SYM_CHARACTER)
        return 1;
    else
        return 0;
}

int isStarOrDivOrModulo() {
    if (symbol == SYM_ASTERISK)
        return 1;
    else if (symbol == SYM_DIV)
        return 1;
    else if (symbol == SYM_MOD)
        return 1;
    else
        return 0;
}

int isPlusOrMinus() {
    if (symbol == SYM_MINUS)
        return 1;
    else if (symbol == SYM_PLUS)
        return 1;
    else
        return 0;
}

int isComparison() {
    if (symbol == SYM_EQUALITY)
        return 1;
    else if (symbol == SYM_NOTEQ)
        return 1;
    else if (symbol == SYM_LT)
        return 1;
    else if (symbol == SYM_GT)
        return 1;
    else if (symbol == SYM_LEQ)
        return 1;
    else if (symbol == SYM_GEQ)
        return 1;
    else
        return 0;
}

int lookForFactor() {
    if (symbol == SYM_LPARENTHESIS)
        return 0;
    else if (symbol == SYM_ASTERISK)
        return 0;
    else if (symbol == SYM_IDENTIFIER)
        return 0;
    else if (symbol == SYM_INTEGER)
        return 0;
    else if (symbol == SYM_CHARACTER)
        return 0;
    else if (symbol == SYM_STRING)
        return 0;
    else if (symbol == SYM_EOF)
        return 0;
    else
        return 1;
}

int lookForStatement() {
    if (symbol == SYM_ASTERISK)
        return 0;
    else if (symbol == SYM_IDENTIFIER)
        return 0;
    else if (symbol == SYM_WHILE)
        return 0;
    else if (symbol == SYM_IF)
        return 0;
    else if (symbol == SYM_RETURN)
        return 0;
    else if (symbol == SYM_EOF)
        return 0;
    else
        return 1;
}

int lookForType() {
    if (symbol == SYM_INT)
        return 0;
    else if (symbol == SYM_VOID)
        return 0;
    else if (symbol == SYM_EOF)
        return 0;
    else
        return 1;
}

void talloc() {
    // we use registers REG_T0-REG_T9 and REG_S0-REG_S7 for temporaries
    if (allocatedTemporaries < REG_T9 - REG_A3)
        allocatedTemporaries = allocatedTemporaries + 1;
    else {
        syntaxErrorMessage((int*) "out of registers");

        exit(-1);
    }
}

int currentTemporary() {
    if (allocatedTemporaries > 0)
        return allocatedTemporaries + REG_A3;
    else {
        syntaxErrorMessage((int*) "illegal register access");

        exit(-1);
    }
}

int previousTemporary() {
    if (allocatedTemporaries > 1)
        return currentTemporary() - 1;
    else {
        syntaxErrorMessage((int*) "illegal register access");

        exit(-1);
    }
}

int nextTemporary() {
    if (allocatedTemporaries < REG_T9 - REG_A3)
        return currentTemporary() + 1;
    else {
        syntaxErrorMessage((int*) "out of registers");

        exit(-1);
    }
}

void tfree(int numberOfTemporaries) {
    allocatedTemporaries = allocatedTemporaries - numberOfTemporaries;

    if (allocatedTemporaries < 0) {
        syntaxErrorMessage((int*) "illegal register deallocation");

        exit(-1);
    }
}

void save_temporaries() {
    while (allocatedTemporaries > 0) {
        emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4);
        emitIFormat(OP_SW, REG_SP, currentTemporary(), 0);

        tfree(1);
    }
}

void restore_temporaries(int numberOfTemporaries) {
    while (allocatedTemporaries < numberOfTemporaries) {
        talloc();

        emitIFormat(OP_LW, REG_SP, currentTemporary(), 0);
        emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);
    }
}

void syntaxErrorSymbol(int expected) {
    printLineNumber((int*) "error");

    printSymbol(expected);
    print((int*) " expected but ");

    printSymbol(symbol);
    print((int*) " found");

    println();
}

void syntaxErrorUnexpected() {
    printLineNumber((int*) "error");

    print((int*) "unexpected symbol ");
    printSymbol(symbol);
    print((int*) " found");

    println();
}

int* putType(int type) {
    if (type == INT_T)
        return (int*) "int";
    else if (type == INTSTAR_T)
        return (int*) "int*";
    else if (type == VOID_T)
        return (int*) "void";
    else
        return (int*) "unknown";
}

void typeWarning(int expected, int found) {
    printLineNumber((int*) "warning");

    print((int*) "type mismatch, ");

    print(putType(expected));

    print((int*) " expected but ");

    print(putType(found));

    print((int*) " found");

    println();
}

int* getVariable(int *variable) {
    int *entry;

    entry = getSymbolTableEntry(variable, VARIABLE, local_symbol_table);

    if (entry == (int*) 0) {
        entry = getSymbolTableEntry(variable, VARIABLE, global_symbol_table);

        if (entry == (int*) 0) {
            printLineNumber((int*) "error");

            print(variable);

            print((int*) " undeclared");
            println();

            exit(-1);
        }
    }

    return entry;
}

int load_variable(int *variable) {
    int *entry;

    entry = getVariable(variable);

    talloc();

    emitIFormat(OP_LW, getRegister(entry), currentTemporary(), getData(entry));

    return getType(entry);
}

void load_integer() {
    // assert: constant >= 0 or constant == INT_MIN

    talloc();

    if (constant >= 0) {
        if (constant < twoToThePowerOf(15))
            // ADDIU can only load numbers < 2^15 without sign extension
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), constant);
        else if (constant < twoToThePowerOf(28)) {
            // load 14 msbs of a 28-bit number first
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), rightShift(constant, 14));

            // shift left by 14 bits
            emitLeftShiftBy(14);

            // and finally add 14 lsbs
            emitIFormat(OP_ADDIU, currentTemporary(), currentTemporary(), rightShift(leftShift(constant, 18), 18));
        } else {
            // load 14 msbs of a 31-bit number first
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), rightShift(constant, 17));

            emitLeftShiftBy(14);

            // then add the next 14 msbs
            emitIFormat(OP_ADDIU, currentTemporary(), currentTemporary(), rightShift(leftShift(constant, 15), 18));

            emitLeftShiftBy(3);

            // and finally add the remaining 3 lsbs
            emitIFormat(OP_ADDIU, currentTemporary(), currentTemporary(), rightShift(leftShift(constant, 29), 29));
        }
    } else {
        // load largest positive 16-bit number with a single bit set: 2^14
        emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), twoToThePowerOf(14));

        // and then multiply 2^14 by 2^14*2^3 to get to 2^31 == INT_MIN
        emitLeftShiftBy(14);
        emitLeftShiftBy(3);
    }
}

void load_string() {
    int l;

    l = stringLength(string) + 1;

    allocatedMemory = allocatedMemory + l;

    if (l % 4 != 0)
        allocatedMemory = allocatedMemory + 4 - l % 4;

    createSymbolTableEntry(GLOBAL_TABLE, string, -allocatedMemory, STRING, INTSTAR_T, 0);

    talloc();

    emitIFormat(OP_ADDIU, REG_GP, currentTemporary(), -allocatedMemory);
}

int help_call_codegen(int *entry, int *procedure) {
    int type;

    if (entry == (int*) 0) {
        // CASE 1: function call, no definition, no declaration.
        createSymbolTableEntry(GLOBAL_TABLE, procedure, binaryLength, FUNCTION, INT_T, 0);

        emitJFormat(OP_JAL, 0);

        type = INT_T; //assume default return type 'int'

    } else {
        type = getType(entry);

        if (getData(entry) == 0) {
            // CASE 2: function call, no definition, but declared.
            setData(entry, binaryLength);

            emitJFormat(OP_JAL, 0);
        } else if (getOpcode(loadMemory(getData(entry))) == OP_JAL) {
            // CASE 3: function call, no declaration
            emitJFormat(OP_JAL, getData(entry) / 4);

            setData(entry, binaryLength - 8);
        } else
            // CASE 4: function defined, use the address
            emitJFormat(OP_JAL, getData(entry) / 4);
    }

    return type;
}

void help_procedure_prologue(int localVariables) {
    // allocate space for return address
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4);

    // save return address
    emitIFormat(OP_SW, REG_SP, REG_RA, 0);

    // allocate space for caller's frame pointer
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4);

    // save caller's frame pointer
    emitIFormat(OP_SW, REG_SP, REG_FP, 0);

    // set callee's frame pointer
    emitIFormat(OP_ADDIU, REG_SP, REG_FP, 0);

    // allocate space for callee's local variables
    if (localVariables != 0)
        emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4 * localVariables);
}

void help_procedure_epilogue(int parameters) {
    // deallocate space for callee's frame pointer and local variables
    emitIFormat(OP_ADDIU, REG_FP, REG_SP, 0);

    // restore caller's frame pointer
    emitIFormat(OP_LW, REG_SP, REG_FP, 0);

    // deallocate space for caller's frame pointer
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    // restore return address
    emitIFormat(OP_LW, REG_SP, REG_RA, 0);

    // deallocate space for return address and parameters
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, (parameters + 1) * 4);

    // return
    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

int gr_call(int *procedure) {
    int *entry;
    int numberOfTemporaries;
    int type;

    // assert: n = allocatedTemporaries

    entry = getSymbolTableEntry(procedure, FUNCTION, global_symbol_table);

    numberOfTemporaries = allocatedTemporaries;

    save_temporaries();

    // assert: allocatedTemporaries == 0

    if (isExpression()) {
        gr_expression();

        // TODO: check if types/number of parameters is correct
        // push first parameter onto stack
        emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4);
        emitIFormat(OP_SW, REG_SP, currentTemporary(), 0);

        tfree(1);

        while (symbol == SYM_COMMA) {
            getSymbol();

            gr_expression();

            // push more parameters onto stack
            emitIFormat(OP_ADDIU, REG_SP, REG_SP, -4);
            emitIFormat(OP_SW, REG_SP, currentTemporary(), 0);

            tfree(1);
        }

        if (symbol == SYM_RPARENTHESIS) {
            getSymbol();
            
            type = help_call_codegen(entry, procedure);
        } else {
            syntaxErrorSymbol(SYM_RPARENTHESIS);

            type = INT_T;
        }
    } else if (symbol == SYM_RPARENTHESIS) {
        getSymbol();

        type = help_call_codegen(entry, procedure);
    } else {
        syntaxErrorSymbol(SYM_RPARENTHESIS);

        type = INT_T;
    }

    // assert: allocatedTemporaries == 0

    restore_temporaries(numberOfTemporaries);

    // assert: allocatedTemporaries == n
    return type;
}

int gr_factor() {
    int hasCast;
    int cast;
    int type;

    int *variableOrProcedureName;

    // assert: n = allocatedTemporaries

    hasCast = 0;

    type = INT_T;

    while (lookForFactor()) {
        syntaxErrorUnexpected();

        if (symbol == SYM_EOF)
            exit(-1);
        else
            getSymbol();
    }

    // optional cast: [ cast ]
    if (symbol == SYM_LPARENTHESIS) {
        getSymbol();

        // cast: "(" "int" [ "*" ] ")"
        if (symbol == SYM_INT) {
            hasCast = 1;

            cast = gr_type();

            if (symbol == SYM_RPARENTHESIS)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_RPARENTHESIS);

        // not a cast: "(" expression ")"
        } else {
            type = gr_expression();

            if (symbol == SYM_RPARENTHESIS)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_RPARENTHESIS);

            // assert: allocatedTemporaries == n + 1

            return type;
        }
    }

    // dereference?
    if (symbol == SYM_ASTERISK) {
        getSymbol();

        // ["*"] identifier
        if (symbol == SYM_IDENTIFIER) {
            type = load_variable(identifier);

            getSymbol();

        // * "(" expression ")"
        } else if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            type = gr_expression();

            if (symbol == SYM_RPARENTHESIS)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        } else
            syntaxErrorUnexpected();

        if (type != INTSTAR_T)
            typeWarning(INTSTAR_T, type);

        // dereference
        emitIFormat(OP_LW, currentTemporary(), currentTemporary(), 0);

        type = INT_T;

    // identifier?
    } else if (symbol == SYM_IDENTIFIER) {
        variableOrProcedureName = identifier;

        getSymbol();

        if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            // function call: identifier "(" ... ")"
            type = gr_call(variableOrProcedureName);

            talloc();

            emitIFormat(OP_ADDIU, REG_V0, currentTemporary(), 0);
        } else
            // variable access: identifier
            type = load_variable(variableOrProcedureName);

    // integer?
    } else if (symbol == SYM_INTEGER) {
        load_integer();

        getSymbol();

        type = INT_T;

    // character?
    } else if (symbol == SYM_CHARACTER) {
        talloc();

        emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), constant);

        getSymbol();
    
        type = INT_T;
        
    // string?
    } else if (symbol == SYM_STRING) {
        load_string();

        getSymbol();

        type = INTSTAR_T;

    //  "(" expression ")"
    } else if (symbol == SYM_LPARENTHESIS) {
        getSymbol();

        type = gr_expression();

        if (symbol == SYM_RPARENTHESIS)
            getSymbol();
        else
            syntaxErrorSymbol(SYM_RPARENTHESIS);
    } else
        syntaxErrorUnexpected();

    // assert: allocatedTemporaries == n + 1

    if (hasCast)
        return cast;
    else
        return type;
}

int gr_term() {
    int ltype;
    int operatorSymbol;
    int rtype;

    // assert: n = allocatedTemporaries

    ltype = gr_factor();

    // assert: allocatedTemporaries == n + 1

    // * / or % ?
    while (isStarOrDivOrModulo()) {
        operatorSymbol = symbol;

        getSymbol();

        rtype = gr_factor();

        // assert: allocatedTemporaries == n + 2
        
        if (ltype != rtype)
            typeWarning(ltype, rtype);

        if (operatorSymbol == SYM_ASTERISK) {
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), 0, FCT_MULTU);
            emitRFormat(OP_SPECIAL, 0, 0, previousTemporary(), FCT_MFLO);

        } else if (operatorSymbol == SYM_DIV) {
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), 0, FCT_DIVU);
            emitRFormat(OP_SPECIAL, 0, 0, previousTemporary(), FCT_MFLO);

        } else if (operatorSymbol == SYM_MOD) {
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), 0, FCT_DIVU);
            emitRFormat(OP_SPECIAL, 0, 0, previousTemporary(), FCT_MFHI);
        }

        tfree(1);
    }

    // assert: allocatedTemporaries == n + 1

    return ltype;
}

int gr_simpleExpression() {
    int sign;
    int ltype;
    int operatorSymbol;
    int rtype;

    // assert: n = allocatedTemporaries

    // optional: -
    if (symbol == SYM_MINUS) {
        sign = 1;

        mayBeINTMINConstant = 1;
        isINTMINConstant    = 0;

        getSymbol();

        mayBeINTMINConstant = 0;

        if (isINTMINConstant) {
            isINTMINConstant = 0;
            
            // avoids 0-INT_MIN overflow when bootstrapping
            // even though 0-INT_MIN == INT_MIN
            sign = 0;
        }
    } else
        sign = 0;

    ltype = gr_term();

    // assert: allocatedTemporaries == n + 1

    if (sign == 1) {
        if (ltype != INT_T) {
            typeWarning(INT_T, ltype);

            ltype = INT_T;
        }

        emitRFormat(OP_SPECIAL, REG_ZR, currentTemporary(), currentTemporary(), FCT_SUBU);
    }

    // + or -?
    while (isPlusOrMinus()) {
        operatorSymbol = symbol;

        getSymbol();

        rtype = gr_term();

        // assert: allocatedTemporaries == n + 2

        if (operatorSymbol == SYM_PLUS) {
            if (ltype == INTSTAR_T) {
                if (rtype == INT_T)
                    // pointer arithmetic: factor of 2^2 of integer operand
                    emitLeftShiftBy(2);
            } else if (rtype == INTSTAR_T)
                typeWarning(ltype, rtype);

            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_ADDU);

        } else if (operatorSymbol == SYM_MINUS) {
            if (ltype != rtype)
                typeWarning(ltype, rtype);

            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_SUBU);
        }

        tfree(1);
    }

    // assert: allocatedTemporaries == n + 1

    return ltype;
}

int gr_expression() {
    int ltype;
    int operatorSymbol;
    int rtype;

    // assert: n = allocatedTemporaries

    ltype = gr_simpleExpression();

    // assert: allocatedTemporaries == n + 1

    //optional: ==, !=, <, >, <=, >= simpleExpression
    if (isComparison()) {
        operatorSymbol = symbol;

        getSymbol();

        rtype = gr_simpleExpression();

        // assert: allocatedTemporaries == n + 2

        if (ltype != rtype)
            typeWarning(ltype, rtype);

        if (operatorSymbol == SYM_EQUALITY) {
            // subtract, if result = 0 then 1, else 0
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_SUBU);

            tfree(1);

            emitIFormat(OP_BEQ, REG_ZR, currentTemporary(), 4);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 0);
            emitIFormat(OP_BEQ, REG_ZR, currentTemporary(), 2);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 1);

        } else if (operatorSymbol == SYM_NOTEQ) {
            // subtract, if result = 0 then 0, else 1
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_SUBU);

            tfree(1);

            emitIFormat(OP_BNE, REG_ZR, currentTemporary(), 4);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 0);
            emitIFormat(OP_BEQ, REG_ZR, currentTemporary(), 2);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 1);

        } else if (operatorSymbol == SYM_LT) {
            // set to 1 if a < b, else 0
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_SLT);

            tfree(1);

        } else if (operatorSymbol == SYM_GT) {
            // set to 1 if b < a, else 0
            emitRFormat(OP_SPECIAL, currentTemporary(), previousTemporary(), previousTemporary(), FCT_SLT);

            tfree(1);

        } else if (operatorSymbol == SYM_LEQ) {
            // if b < a set 0, else 1
            emitRFormat(OP_SPECIAL, currentTemporary(), previousTemporary(), previousTemporary(), FCT_SLT);

            tfree(1);

            emitIFormat(OP_BNE, REG_ZR, currentTemporary(), 4);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 1);
            emitIFormat(OP_BEQ, REG_ZR, REG_ZR, 2);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 0);

        } else if (operatorSymbol == SYM_GEQ) {
            // if a < b set 0, else 1
            emitRFormat(OP_SPECIAL, previousTemporary(), currentTemporary(), previousTemporary(), FCT_SLT);

            tfree(1);

            emitIFormat(OP_BNE, REG_ZR, currentTemporary(), 4);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 1);
            emitIFormat(OP_BEQ, REG_ZR, REG_ZR, 2);
            emitIFormat(OP_ADDIU, REG_ZR, currentTemporary(), 0);
        }
    }
    
    // assert: allocatedTemporaries == n + 1

    return ltype;
}

void gr_while() {
    int brBackToWhile;
    int brForwardToEnd;

    // assert: allocatedTemporaries == 0

    brBackToWhile = binaryLength;

    brForwardToEnd = 0;

    // while ( expression )
    if (symbol == SYM_WHILE) {
        getSymbol();

        if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            gr_expression();

            // do not know where to branch, fixup later
            brForwardToEnd = binaryLength;

            emitIFormat(OP_BEQ, REG_ZR, currentTemporary(), 0);

            tfree(1);

            if (symbol == SYM_RPARENTHESIS) {
                getSymbol();

                // zero or more statements: { statement }
                if (symbol == SYM_LBRACE) {
                    getSymbol();

                    while (isNotRbraceOrEOF())
                        gr_statement();

                    if (symbol == SYM_RBRACE)
                        getSymbol();
                    else {
                        syntaxErrorSymbol(SYM_RBRACE);

                        exit(-1);
                    }
                }
                // only one statement without {}
                else
                    gr_statement();
            } else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        } else
            syntaxErrorSymbol(SYM_LPARENTHESIS);
    } else
        syntaxErrorSymbol(SYM_WHILE);

    // unconditional branch to beginning of while
    emitIFormat(OP_BEQ, 0, 0, (brBackToWhile - binaryLength - 4) / 4);

    if (brForwardToEnd != 0)
        // first instruction after loop comes here
        // now we have our address for the conditional jump from above
        fixup_relative(brForwardToEnd);

    // assert: allocatedTemporaries == 0
}

void gr_if() {
    int brForwardToElseOrEnd;
    int brForwardToEnd;

    // assert: allocatedTemporaries == 0

    // if ( expression )
    if (symbol == SYM_IF) {
        getSymbol();

        if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            gr_expression();

            // if the "if" case is not true, we jump to "else" (if provided)
            brForwardToElseOrEnd = binaryLength;

            emitIFormat(OP_BEQ, REG_ZR, currentTemporary(), 0);

            tfree(1);

            if (symbol == SYM_RPARENTHESIS) {
                getSymbol();

                // zero or more statements: { statement }
                if (symbol == SYM_LBRACE) {
                    getSymbol();

                    while (isNotRbraceOrEOF())
                        gr_statement();

                    if (symbol == SYM_RBRACE)
                        getSymbol();
                    else {
                        syntaxErrorSymbol(SYM_RBRACE);

                        exit(-1);
                    }
                }
                // only one statement without {}
                else
                    gr_statement();

                //optional: else
                if (symbol == SYM_ELSE) {
                    getSymbol();

                    // if the "if" case was true, we jump to the end
                    brForwardToEnd = binaryLength;
                    emitIFormat(OP_BEQ, 0, 0, 0);

                    // if the "if" case was not true, we jump here
                    fixup_relative(brForwardToElseOrEnd);

                    // zero or more statements: { statement }
                    if (symbol == SYM_LBRACE) {
                        getSymbol();

                        while (isNotRbraceOrEOF())
                            gr_statement();

                        if (symbol == SYM_RBRACE)
                            getSymbol();
                        else {
                            syntaxErrorSymbol(SYM_RBRACE);

                            exit(-1);
                        }

                    // only one statement without {}
                    } else
                        gr_statement();

                    // if the "if" case was true, we jump here
                    fixup_relative(brForwardToEnd);
                } else
                    // if the "if" case was not true, we jump here
                    fixup_relative(brForwardToElseOrEnd);
            } else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        } else
            syntaxErrorSymbol(SYM_LPARENTHESIS);
    } else
        syntaxErrorSymbol(SYM_IF);

    // assert: allocatedTemporaries == 0
}

void gr_return(int returnType) {
    int type;

    // assert: allocatedTemporaries == 0

    if (symbol == SYM_RETURN)
        getSymbol();
    else
        syntaxErrorSymbol(SYM_RETURN);

    // optional: expression
    if (symbol != SYM_SEMICOLON) {
        type = gr_expression();

        if (returnType == VOID_T)
            typeWarning(type, returnType);
        else if (type != returnType)
            typeWarning(returnType, type);

        // save value of expression in return register
        emitRFormat(OP_SPECIAL, REG_ZR, currentTemporary(), REG_V0, FCT_ADDU);
        
        tfree(1);
    }

    // unconditional branch to procedure epilogue
    // maintain fixup chain for later fixup
    emitJFormat(OP_J, returnBranches / 4);

    // new head of fixup chain
    // offest is 8 rather than 4 bytes because of delay slot NOP
    returnBranches = binaryLength - 8;

    // assert: allocatedTemporaries == 0
}

void gr_statement() {
    int ltype;
    int rtype;
    int *variableOrProcedureName;
    int *entry;

    // assert: allocatedTemporaries == 0;

    while (lookForStatement()) {
        syntaxErrorUnexpected();

        if (symbol == SYM_EOF)
            exit(-1);
        else
            getSymbol();
    }

    // ["*"]
    if (symbol == SYM_ASTERISK) {
        getSymbol();

        // "*" identifier
        if (symbol == SYM_IDENTIFIER) {
            ltype = load_variable(identifier);

            if (ltype != INTSTAR_T)
                typeWarning(INTSTAR_T, ltype);

            getSymbol();

            // "*" identifier "="
            if (symbol == SYM_ASSIGN) {
                getSymbol();

                rtype = gr_expression();

                if (rtype != INT_T)
                    typeWarning(INT_T, rtype);

                emitIFormat(OP_SW, previousTemporary(), currentTemporary(), 0);

                tfree(2);
            } else
                syntaxErrorSymbol(SYM_ASSIGN);

            if (symbol == SYM_SEMICOLON)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_SEMICOLON);

        // "*" "(" expression ")"
        } else if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            ltype = gr_expression();

            if (ltype != INTSTAR_T)
                typeWarning(INTSTAR_T, ltype);

            if (symbol == SYM_RPARENTHESIS) {
                getSymbol();

                // "*" "(" expression ")" "="
                if (symbol == SYM_ASSIGN) {
                    getSymbol();

                    rtype = gr_expression();

                    if (rtype != INT_T)
                        typeWarning(INT_T, rtype);

                    emitIFormat(OP_SW, previousTemporary(), currentTemporary(), 0);

                    tfree(2);
                } else
                    syntaxErrorSymbol(SYM_ASSIGN);

                if (symbol == SYM_SEMICOLON)
                    getSymbol();
                else
                    syntaxErrorSymbol(SYM_SEMICOLON);
            } else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        } else
            syntaxErrorSymbol(SYM_LPARENTHESIS);
    }
    // identifier "=" expression | call
    else if (symbol == SYM_IDENTIFIER) {
        variableOrProcedureName = identifier;

        getSymbol();

        // call
        if (symbol == SYM_LPARENTHESIS) {
            getSymbol();

            gr_call(variableOrProcedureName);

            if (symbol == SYM_SEMICOLON)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_SEMICOLON);

        // identifier = expression
        } else if (symbol == SYM_ASSIGN) {
            entry = getVariable(variableOrProcedureName);

            ltype = getType(entry);

            getSymbol();

            rtype = gr_expression();

            if (ltype != rtype)
                typeWarning(ltype, rtype);

            emitIFormat(OP_SW, getRegister(entry), currentTemporary(), getData(entry));

            tfree(1);

            if (symbol == SYM_SEMICOLON)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_SEMICOLON);
        } else
            syntaxErrorUnexpected();
    }
    // while statement?
    else if (symbol == SYM_WHILE) {
        gr_while();
    }
    // if statement?
    else if (symbol == SYM_IF) {
        gr_if();
    }
    // return statement?
    else if (symbol == SYM_RETURN) {
        entry = getSymbolTableEntry(currentProcedureName, FUNCTION, global_symbol_table);

        gr_return(getType(entry));

        if (symbol == SYM_SEMICOLON)
            getSymbol();
        else
            syntaxErrorSymbol(SYM_SEMICOLON);
    }
}

int gr_type() {
    int type;

    type = INT_T;
    
    if (symbol == SYM_INT) {
        getSymbol();

        if (symbol == SYM_ASTERISK) {
            type = INTSTAR_T;

            getSymbol();
        }
    } else
        syntaxErrorSymbol(SYM_INT);

    return type;
}

void gr_variable(int offset) {
    int type;

    type = gr_type();

    if (symbol == SYM_IDENTIFIER) {
        createSymbolTableEntry(LOCAL_TABLE, identifier, offset, VARIABLE, type, 0);

        getSymbol();
    } else {
        syntaxErrorSymbol(SYM_IDENTIFIER);

        createSymbolTableEntry(LOCAL_TABLE, (int*) "missing variable name", offset, VARIABLE, type, 0);
    }
}

void gr_initialization(int *name, int offset, int type) {
    int hasCast;
    int cast;
    int sign;

    initialValue = 0;

    hasCast = 0;

    if (symbol == SYM_SEMICOLON)
        getSymbol();
    else if (symbol == SYM_ASSIGN) {
        getSymbol();

        // optional cast: [ cast ]
        if (symbol == SYM_LPARENTHESIS) {
            hasCast = 1;

            getSymbol();

            cast = gr_type();

            if (symbol == SYM_RPARENTHESIS)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        }

        // optional: -
        if (symbol == SYM_MINUS) {
            sign = 1;

            mayBeINTMINConstant = 1;
            isINTMINConstant    = 0;

            getSymbol();

            mayBeINTMINConstant = 0;

            if (isINTMINConstant) {
                isINTMINConstant = 0;
            
                // avoids 0-INT_MIN overflow when bootstrapping
                // even though 0-INT_MIN == INT_MIN
                sign = 0;
            }
        } else
            sign = 0;

        if (isConstant()) {
            initialValue = constant;

            getSymbol();

            if (sign == 1)
                initialValue = -initialValue;
        } else
            syntaxErrorUnexpected();

        if (symbol == SYM_SEMICOLON)
            getSymbol();
        else
            syntaxErrorSymbol(SYM_SEMICOLON);
    } else
        syntaxErrorUnexpected();

    if (hasCast) {
        if (type != cast)
            typeWarning(type, cast);
    } else if (type != INT_T)
        typeWarning(type, INT_T);

    createSymbolTableEntry(GLOBAL_TABLE, name, offset, VARIABLE, type, initialValue);
}

void gr_procedure(int *procedure, int returnType) {
    int numberOfParameters;
    int parameters;
    int localVariables;
    int functionStart;
    int *entry;

    currentProcedureName = procedure;

    numberOfParameters = 0;

    // ( variable , variable ) ;
    if (symbol == SYM_LPARENTHESIS) {
        getSymbol();

        if (symbol != SYM_RPARENTHESIS) {
            gr_variable(0);

            numberOfParameters = 1;

            while (symbol == SYM_COMMA) {
                getSymbol();

                gr_variable(0);

                numberOfParameters = numberOfParameters + 1;
            }

            entry = local_symbol_table;

            parameters = 0;

            while (parameters < numberOfParameters) {
                // 8 bytes offset to skip frame pointer and link
                setData(entry, parameters * 4 + 8);

                parameters = parameters + 1;
                entry      = getNext(entry);
            }

            if (symbol == SYM_RPARENTHESIS)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_RPARENTHESIS);
        } else
            getSymbol();
    } else
        syntaxErrorSymbol(SYM_LPARENTHESIS);

    if (symbol == SYM_SEMICOLON) {
        entry = getSymbolTableEntry(currentProcedureName, FUNCTION, global_symbol_table);

        if (entry == (int*) 0)
            createSymbolTableEntry(GLOBAL_TABLE, currentProcedureName, 0, FUNCTION, returnType, 0);

        getSymbol();

    // ( variable, variable ) { variable; variable; statement }
    } else if (symbol == SYM_LBRACE) {
        functionStart = binaryLength;
        
        getSymbol();

        entry = getSymbolTableEntry(currentProcedureName, FUNCTION, global_symbol_table);

        if (entry == (int*) 0)
            createSymbolTableEntry(GLOBAL_TABLE, currentProcedureName, binaryLength, FUNCTION, returnType, 0);
        else {
            if (getData(entry) != 0) {
                if (getOpcode(loadMemory(getData(entry))) == OP_JAL)
                    fixlink_absolute(getData(entry), functionStart);
                else {
                    printLineNumber((int*) "error");

                    print((int*) "multiple definitions of ");

                    print(currentProcedureName);

                    println();
                }
            }

            setData(entry, functionStart);

            if (getType(entry) != returnType)
                typeWarning(getType(entry), returnType);

            setType(entry, returnType);
        }

        localVariables = 0;

        while (symbol == SYM_INT) {
            localVariables = localVariables + 1;

            gr_variable(-4 * localVariables);

            if (symbol == SYM_SEMICOLON)
                getSymbol();
            else
                syntaxErrorSymbol(SYM_SEMICOLON);
        }

        help_procedure_prologue(localVariables);

        // create a fixup chain for return statements
        returnBranches = 0;

        while (isNotRbraceOrEOF())
            gr_statement();

        if (symbol == SYM_RBRACE)
            getSymbol();
        else {
            syntaxErrorSymbol(SYM_RBRACE);

            exit(-1);
        }

        fixlink_absolute(returnBranches, binaryLength);

        returnBranches = 0;

        help_procedure_epilogue(numberOfParameters);

    } else
        syntaxErrorUnexpected();

    local_symbol_table = (int*) 0;

    // assert: allocatedTemporaries == 0
}

void gr_cstar() {
    int type;
    int *variableOrProcedureName;

    while (symbol != SYM_EOF) {
        while (lookForType()) {
            syntaxErrorUnexpected();

            if (symbol == SYM_EOF)
                exit(-1);
            else
                getSymbol();
        }

        // void identifier procedure
        if (symbol == SYM_VOID) {
            type = VOID_T;

            getSymbol();

            if (symbol == SYM_IDENTIFIER) {
                variableOrProcedureName = identifier;

                getSymbol();

                gr_procedure(variableOrProcedureName, type);
            } else
                syntaxErrorSymbol(SYM_IDENTIFIER);
        } else {
            type = gr_type();

            if (symbol == SYM_IDENTIFIER) {
                variableOrProcedureName = identifier;

                getSymbol();

                // type identifier "(" procedure declaration or definition
                if (symbol == SYM_LPARENTHESIS)
                    gr_procedure(variableOrProcedureName, type);
                else {
                    allocatedMemory = allocatedMemory + 4;

                    // type identifier ";" global variable declaration
                    if (symbol == SYM_SEMICOLON) {
                        getSymbol();

                        createSymbolTableEntry(GLOBAL_TABLE, variableOrProcedureName, -allocatedMemory, VARIABLE, type, 0);

                    // type identifier "=" global variable definition
                    } else
                        gr_initialization(variableOrProcedureName, -allocatedMemory, type);
                }
            } else
                syntaxErrorSymbol(SYM_IDENTIFIER);
        }
    }
}

// -----------------------------------------------------------------
// ------------------------ MACHINE CODE LIBRARY -------------------
// -----------------------------------------------------------------

void emitLeftShiftBy(int b) {
    // assert: 0 <= b < 15

    // load multiplication factor less than 2^15 to avoid sign extension
    emitIFormat(OP_ADDIU, REG_ZR, nextTemporary(), twoToThePowerOf(b));
    emitRFormat(OP_SPECIAL, currentTemporary(), nextTemporary(), 0, FCT_MULTU);
    emitRFormat(OP_SPECIAL, 0, 0, currentTemporary(), FCT_MFLO);
}

void emitMainEntry() {
    // instruction at address zero cannot be fixed up
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP);

    createSymbolTableEntry(GLOBAL_TABLE, (int*) "main", binaryLength, FUNCTION, INT_T, 0);

    mainJumpAddress = binaryLength;

    emitJFormat(OP_JAL, 0);
}

// -----------------------------------------------------------------
// ----------------------------- MAIN ------------------------------
// -----------------------------------------------------------------

int main_compiler() {
    initScanner();
    initParser();

    // memory in bytes and executable file name "out"
    initMemory(maxBinaryLength, (int*) "out");

    getSymbol();

    // jump to main
    emitMainEntry();

    // library:
    // exit must be first to exit main
    // if exit call in main is missing
    emitExit();
    emitRead();
    emitWrite();
    emitOpen();
    emitMalloc();
    emitGetchar();
    emitPutchar();
    emitYield();
    emitLock();
    emitUnlock();

    // parser
    gr_cstar();

    if (getInstrIndex(loadMemory(mainJumpAddress)) != 0)
        emitBinary();
    else {
        print((int*) "cstarc: main function missing");
        println();
    }

    exit(0);
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// -------------------     I N T E R F A C E     -------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- REGISTER ---------------------------
// -----------------------------------------------------------------

void printRegister(int reg) {
    print((int*) *(REGISTERS + reg));
}

// -----------------------------------------------------------------
// ---------------------------- ENCODER ----------------------------
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+-----+-----+------+
// |opcode|  rs |  rt |  rd |00000|fction|
// +------+-----+-----+-----+-----+------+
//    6      5     5     5     5     6
int encodeRFormat(int opcode, int rs, int rt, int rd, int function) {
    // assert: 0 <= opcode < 2^6
    // assert: 0 <= rs < 2^5
    // assert: 0 <= rt < 2^5
    // assert: 0 <= rd < 2^5
    // assert: 0 <= function < 2^6
    return leftShift(leftShift(leftShift(leftShift(opcode, 5) + rs, 5) + rt, 5) + rd, 11) + function;
}

// -----------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+----------------+
// |opcode|  rs |  rt |    immediate   |
// +------+-----+-----+----------------+
//    6      5     5          16
int encodeIFormat(int opcode, int rs, int rt, int immediate) {
    // assert: 0 <= opcode < 2^6
    // assert: 0 <= rs < 2^5
    // assert: 0 <= rt < 2^5
    // assert: -2^15 <= immediate < 2^15
    if (immediate < 0)
        // convert from 32-bit to 16-bit two's complement
        immediate = immediate + twoToThePowerOf(16);

    return leftShift(leftShift(leftShift(opcode, 5) + rs, 5) + rt, 16) + immediate;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+--------------------------+
// |opcode|       instr_index        |
// +------+--------------------------+
//    6                26
int encodeJFormat(int opcode, int instr_index) {
    // assert: 0 <= opcode < 2^6
    // assert: 0 <= immediate < 2^26
    return leftShift(opcode, 26) + instr_index;
}

int getOpcode(int instruction) {
    return rightShift(instruction, 26);
}

int getRS(int instruction) {
    return rightShift(leftShift(instruction, 6), 27);
}

int getRT(int instruction) {
    return rightShift(leftShift(instruction, 11), 27);
}

int getRD(int instruction) {
    return rightShift(leftShift(instruction, 16), 27);
}

int getFunction(int instruction) {
    return rightShift(leftShift(instruction, 26), 26);
}

int getImmediate(int instruction) {
    return rightShift(leftShift(instruction, 16), 16);
}

int getInstrIndex(int instruction) {
    return rightShift(leftShift(instruction, 6), 6);
}

int signExtend(int immediate) {
    // sign-extend from 16-bit to 32-bit two's complement
    if (immediate < twoToThePowerOf(15))
        return immediate;
    else
        return immediate - twoToThePowerOf(16);
}

// -----------------------------------------------------------------
// ---------------------------- DECODER ----------------------------
// -----------------------------------------------------------------

void printOpcode(int opcode) {
    print((int*) *(OPCODES + opcode));
}

void printFunction(int function) {
    print((int*) *(FUNCTIONS + function));
}

void decode() {
    opcode = getOpcode(ir);

    if (opcode == 0)
        decodeRFormat();
    else if (opcode == OP_JAL)
        decodeJFormat();
    else if (opcode == OP_J)
        decodeJFormat();
    else
        decodeIFormat();
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+-----+-----+------+
// |opcode|  rs |  rt |  rd |00000|fction|
// +------+-----+-----+-----+-----+------+
//    6      5     5     5     5     6
void decodeRFormat() {
    rs          = getRS(ir);
    rt          = getRT(ir);
    rd          = getRD(ir);
    immediate   = 0;
    function    = getFunction(ir);
    instr_index = 0;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+----------------+
// |opcode|  rs |  rt |    immediate   |
// +------+-----+-----+----------------+
//    6      5     5          16
void decodeIFormat() {
    rs          = getRS(ir);
    rt          = getRT(ir);
    rd          = 0;
    immediate   = getImmediate(ir);
    function    = 0;
    instr_index = 0;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+--------------------------+
// |opcode|       instr_index        |
// +------+--------------------------+
//    6                26
void decodeJFormat() {
    rs          = 0;
    rt          = 0;
    rd          = 0;
    immediate   = 0;
    function    = 0;
    instr_index = getInstrIndex(ir);
}

// -----------------------------------------------------------------
// ---------------------------- MEMORY -----------------------------
// -----------------------------------------------------------------

int tlb(int vaddr) {
	int addr;
    if (vaddr % 4 != 0)
        exception_handler(EXCEPTION_ADDRESSERROR);
	addr = vaddr/4;
	if(isEmulating){
		if(memoryStartAddress > ((int)memory + addr)){ // addressed memory is below memory start address
  			print((int*)"tlb first: ");exception_handler(EXCEPTION_SEGMENTATIONFAULT);
		}
		if(memoryStartAddress + memorySize < ((int)memory + addr)){ // addressed memory is greater than memory
			print((int*)"tlb second: ");exception_handler(EXCEPTION_SEGMENTATIONFAULT);
		}
//		println();
//		print((int*)"currSegmentPos ");
////		print(itoa(currSegmentPos, string_buffer, 10, 0));println();
//		print((int*)"memory ");
//		print(itoa((int)memory, string_buffer, 10, 0));println();
		
		if(currSegmentPos != (int)memory){
	        print((int*)"tlb third: ");exception_handler(EXCEPTION_ADDRESSERROR);
		}
		if(currSegmentSize < addr){
	        print((int*)"tlb fourth: ");exception_handler(EXCEPTION_HEAPOVERFLOW);
		}
		if(addr < 0){
	       print((int*)"tlb fifth: "); exception_handler(EXCEPTION_ADDRESSERROR);
		}
	}

    // physical memory is word-addressed for lack of byte-sized data type
    return addr;
}

int loadMemory(int vaddr) {
	
    return *(memory + tlb(vaddr));
}

void storeMemory(int vaddr, int data) {
    *(memory + tlb(vaddr)) = data;
}

// -----------------------------------------------------------------
// ---------------------------- BINARY -----------------------------
// -----------------------------------------------------------------

void emitInstruction(int instruction) {
    if (binaryLength >= maxBinaryLength) {
        syntaxErrorMessage((int*) "exceeded maximum binary length");
        exit(-1);
    } else {
        storeMemory(binaryLength, instruction);
        
        binaryLength = binaryLength + 4;
    }
}

void emitRFormat(int opcode, int rs, int rt, int rd, int function) {
    emitInstruction(encodeRFormat(opcode, rs, rt, rd, function));

    if (opcode == OP_SPECIAL) {
        if (function == FCT_JR)
            emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // delay slot
        else if (function == FCT_MFLO) {
            // In MIPS I-III two instructions after MFLO/MFHI
            // must not modify the LO/HI registers
            emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // pipeline delay
            emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // pipeline delay
        } else if (function == FCT_MFHI) {
            emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // pipeline delay
            emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // pipeline delay
        }
    }
}

void emitIFormat(int opcode, int rs, int rt, int immediate) {
    emitInstruction(encodeIFormat(opcode, rs, rt, immediate));

    if (opcode == OP_BEQ)
        emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // delay slot
    else if (opcode == OP_BNE)
        emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // delay slot
}

void emitJFormat(int opcode, int instr_index) {
    emitInstruction(encodeJFormat(opcode, instr_index));

    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_NOP); // delay slot
}

void fixup_relative(int fromAddress) {
    int instruction;

    instruction = loadMemory(fromAddress);

    storeMemory(fromAddress,
        encodeIFormat(getOpcode(instruction),
            getRS(instruction),
            getRT(instruction),
            (binaryLength - fromAddress - 4) / 4));
}

void fixup_absolute(int fromAddress, int toAddress) {
    storeMemory(fromAddress,
        encodeJFormat(getOpcode(loadMemory(fromAddress)), toAddress / 4));
}

void fixlink_absolute(int fromAddress, int toAddress) {
    int previousAddress;

    while (fromAddress != 0) {
        previousAddress = getInstrIndex(loadMemory(fromAddress)) * 4;
        fixup_absolute(fromAddress, toAddress);
        fromAddress = previousAddress;
    }
}

int copyStringToMemory(int *s, int a) {
    int l;
    int w;

    l = stringLength(s) + 1;

    w = a + l;

    if (l % 4 != 0)
        w = w + 4 - l % 4;

    while (a < w) {
        storeMemory(a, *s);

        s = s + 1;
        a = a + 4;
    }

    return w;
}

void emitBinary() {
    int *entry;
    int fd;

    entry = global_symbol_table;

    // allocate space for global variables and copy strings
    while ((int) entry != 0) {
        if (getClass(entry) == VARIABLE) {
            storeMemory(binaryLength, getValue(entry));

            binaryLength = binaryLength + 4;
        } else if (getClass(entry) == STRING)
            binaryLength = copyStringToMemory(getString(entry), binaryLength);

        entry = getNext(entry);
    }

    // assert: file with name binaryName exists prior to execution of compiler
    fd = open(binaryName, 1); // 1 = O_WRONLY

    if (fd < 0) {
        syntaxErrorMessage((int*) "output file not found");
        exit(-1);
    }

    // The mipster_syscall 4004 writes the code array into a file.
    // The syscall uses the "write" system call of the OS and compiler.
    // The write system call of our Linux uses little endian byte ordering.
    write(fd, memory, binaryLength);
}

void loadBinary() {
    int fd;
    int numberOfReadBytes;

    fd = open(binaryName, 0); // 0 = O_RDONLY

    if (fd < 0)
        exit(-1);

    numberOfReadBytes = 4;

    while (numberOfReadBytes == 4) {
        numberOfReadBytes = read(fd, memory + tlb(binaryLength), 4);

        if (debug_load) {
            print(binaryName);
            print((int*) ": ");
            print(itoa(binaryLength, string_buffer, 16, 8));
            print((int*) ": ");
            print(itoa(loadMemory(binaryLength), string_buffer, 16, 8));
            println();
        }

        if (numberOfReadBytes == 4)
            binaryLength = binaryLength + 4;
    }
}

// -----------------------------------------------------------------
// --------------------------- SYSCALLS ----------------------------
// -----------------------------------------------------------------

void emitExit() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "exit", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);

    emitIFormat(OP_LW, REG_SP, REG_A0, 0); // exit code
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_EXIT);
    emitRFormat(0, 0, 0, 0, FCT_SYSCALL);
}

void syscall_exit() {
    int exitCode;

    exitCode = *(registers+REG_A0);

    *(registers+REG_V0) = exitCode;
	println();
    print(binaryName);
    print((int*) ": exiting with error code ");
    print(itoa(exitCode, string_buffer, 10, 0));
    println();

    exit(0);
}

void emitRead() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "read", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);

    emitIFormat(OP_LW, REG_SP, REG_A2, 0); // count
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_LW, REG_SP, REG_A1, 0); // *buffer
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_LW, REG_SP, REG_A0, 0); // fd
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_READ);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void syscall_read() {
    int count;
    int vaddr;
    int fd;
    int *buffer;
    int size;

    count = *(registers+REG_A2);
    vaddr = *(registers+REG_A1);
    fd    = *(registers+REG_A0);

    buffer = memory + tlb(vaddr);

    size = read(fd, buffer, count);

    *(registers+REG_V0) = size;

    if (debug_read) {
        print(binaryName);
        print((int*) ": read ");
        print(itoa(size, string_buffer, 10, 0));
        print((int*) " bytes from file with descriptor ");
        print(itoa(fd, string_buffer, 10, 0));
        print((int*) " into buffer at address ");
        print(itoa((int) buffer, string_buffer, 16, 8));
        println();
    }
}

void emitWrite() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "write", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);

    emitIFormat(OP_LW, REG_SP, REG_A2, 0); // size
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_LW, REG_SP, REG_A1, 0); // *buffer
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_LW, REG_SP, REG_A0, 0); // fd
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_WRITE);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void syscall_write() {
    int size;
    int vaddr;
    int fd;
    int *buffer;
	size  = *(registers+REG_A2);
	vaddr = *(registers+REG_A1);
	fd    = *(registers+REG_A0);

	buffer = memory + tlb(vaddr);

	size = write(fd, buffer, size);

	*(registers+REG_V0) = size;

	if (debug_write) {
		print(binaryName);
		print((int*) ": wrote ");
		print(itoa(size, string_buffer, 10, 0));
		print((int*) " bytes from buffer at address ");
		print(itoa((int) buffer, string_buffer, 16, 8));
		print((int*) " into file with descriptor ");
		print(itoa(fd, string_buffer, 10, 0));
		println();
	}
	
}

void emitOpen() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "open", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);

    emitIFormat(OP_LW, REG_SP, REG_A1, 0); // flags
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_LW, REG_SP, REG_A0, 0); // filename
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_OPEN);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void syscall_open() {
    int flags;
    int vaddr;
    int *filename;
    int fd;

    flags = *(registers+REG_A1);
    vaddr = *(registers+REG_A0);

    filename = memory + tlb(vaddr);

    fd = open(filename, flags);

    *(registers+REG_V0) = fd;

    if (debug_open) {
        print(binaryName);
        print((int*) ": opened file ");
        printString(filename);
        print((int*) " with flags ");
        print(itoa(flags, string_buffer, 10, 0));
        print((int*) " returning file descriptor ");
        print(itoa(fd, string_buffer, 10, 0));
        println();
    }
}

void emitMalloc() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "malloc", binaryLength, FUNCTION, INTSTAR_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);

    // load argument for malloc (size)
    emitIFormat(OP_LW, REG_SP, REG_A0, 0);

    // remove the argument from the stack
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    // load the correct syscall number and invoke syscall
    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_MALLOC);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    // jump back to caller, return value is in REG_V0
    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void syscall_malloc() {
    int size;
    int bump;

    size = *(registers+REG_A0);

    if (size % 4 != 0)
        size = size + 4 - size % 4;

    bump = *(registers+REG_K1);

    if (bump + size >= *(registers+REG_SP))
        exception_handler(EXCEPTION_HEAPOVERFLOW);

    *(registers+REG_K1) = bump + size;
    *(registers+REG_V0) = bump;

    if (debug_malloc) {
        print(binaryName);
        print((int*) ": malloc ");
        print(itoa(size, string_buffer, 10, 0));
        print((int*) " bytes returning address ");
        print(itoa(bump, string_buffer, 16, 8));
        println();
    }
}

void emitGetchar() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "getchar", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A0, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_GETCHAR);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void syscall_getchar() {
    int c;

    c = getchar();

    *(registers+REG_V0) = c;

    if (debug_getchar) {
        print(binaryName);
        print((int*) ": getchar ");
        printCharacter(c);
        println();
    }
}

void emitPutchar() {
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "putchar", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 4); // write one integer, 4 bytes

    emitIFormat(OP_ADDIU, REG_SP, REG_A1, 0); // pointer to character
    emitIFormat(OP_ADDIU, REG_SP, REG_SP, 4);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A0, 1); // stdout file descriptor

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_WRITE);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);
}

void emitYield(){
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "yield", binaryLength, FUNCTION, INT_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A0, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_YIELD);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);	
}

void syscall_yield() {
//print((int*)"lock = ");
//print(itoa(lock, string_buffer, 10 , 0));println();
	switchProcess(0);
}

void emitUnlock(){
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "unlock", binaryLength, FUNCTION, VOID_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A0, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_UNLOCK);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);	

}

void syscall_unlock(){
	if(*(currProcess+2) == lockID){
		lock = 0;
	}
}

void emitLock(){
    createSymbolTableEntry(GLOBAL_TABLE, (int*) "lock", binaryLength, FUNCTION, VOID_T, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_A3, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A2, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A1, 0);
    emitIFormat(OP_ADDIU, REG_ZR, REG_A0, 0);

    emitIFormat(OP_ADDIU, REG_ZR, REG_V0, SYSCALL_LOCK);
    emitRFormat(OP_SPECIAL, 0, 0, 0, FCT_SYSCALL);

    emitRFormat(OP_SPECIAL, REG_RA, 0, 0, FCT_JR);	
}

void syscall_lock(){
//	print((int*)"currProcess:");
//	print(itoa(*(currProcess+2), string_buffer, 10, 0));println();
//	print((int*)"blockedQueue:");println();
//	printList(blockedQueue,0);
//	print((int*)"readyQueue:");println();
//	printList(readyQueue,0);
//	print((int*)"lock: ");
//	print(itoa(lock, string_buffer, 10, 0));println();
//	print((int*)"lockID: ");
//	print(itoa(lockID, string_buffer, 10, 0));println();
	if(lock == 0){
		lock = 1;
		lockID = *(currProcess+2);
	} else if(lock == 1){
		if(lockID != *(currProcess+2)){
//			appendListElement(currProcess, blockedQueue);
			switchProcess(0);
		} 
	}
//	printList(blockedQueue, 0);
}


// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------    Operating System   ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// a process has following structure:
//
// 0 +-----+
//   |prev | pointer to previous element in list, 0 if non-existent
// 1 +-----+  
//   |next | pointer to next element in list, 0 if non-existent
// 2 +-----+  
//   |key  | unique identifier of list element
// 3 +-----+  
//   | pc  | program counter
// 4 +-----+  
//   |reg  | pointer to registers
// 5 +-----+  
//   |mem  | pointer to  entry in segmentation table
// 6 +-----+ 

// print segmentation table entry
void printSegmentationTableEntryVerbose(int *element){
	int *prev;
	int *next;
	prev = (int*)*element;
	next = (int*)*(element+1);

	println();
	print((int*)"pre ");
	if(prev != (int*)0)
		print(itoa(*(prev+2), string_buffer, 10, 0));
	println();

	print((int*)"next ");
	if(next != (int*)0)
		print(itoa(*(next+2), string_buffer, 10, 0));
	println();

	print((int*)"key ");
	if(element != (int*)0)
		print(itoa(*(element+2), string_buffer, 10, 0));
	println();

	print((int*)"segPos ");
	if(element != (int*)0)
		print(itoa(*(element+3), string_buffer, 10, 0));
	println();

	print((int*)"segSize ");
	if(element != (int*)0)
		print(itoa(*(element+4), string_buffer, 10, 0));
	println();
	println();

}
// print segmentation table entry
void printSegmentationTableEntry(int *element){
	print((int*)"key ");
	if(element != (int*)0)
		print(itoa(*(element+2), string_buffer, 10, 0));
	print((int*)", segSize ");
	if(element != (int*)0)
		print(itoa(*(element+4), string_buffer, 10, 0));
	println();
}

void printProcess(int *element){
	int *prev;
	int *next;
	int *segEntry;
	prev = (int*)*element;
	next = (int*)*(element+1);
	segEntry = (int*)*(element+5);

	println();
	print((int*)"pre ");
	if(prev != (int*)0)
		print(itoa(*(prev+2), string_buffer, 10, 0));
	println();

	print((int*)"next ");
	if(next != (int*)0)
		print(itoa(*(next+2), string_buffer, 10, 0));
	println();

	print((int*)"key ");
	if(element != (int*)0)
		print(itoa(*(element+2), string_buffer, 10, 0));
	println();

	print((int*)"pc ");
	if(element != (int*)0)
		print(itoa(*(element+3), string_buffer, 10, 0));
	println();
	
	print((int*)"segPos ");
	if(segEntry != (int*)0)
		print(itoa((int)*(segEntry+3), string_buffer, 10, 0));
	println();

	print((int*)"segSize ");
	if(segEntry != (int*)0)
		print(itoa((int)*(segEntry+4), string_buffer, 10, 0));
	println();
	println();

}
// print list element 
void printListElementVerbose(int *element){
	int *prev;
	int *next;
	prev = (int*)*element;
	next = (int*)*(element+1);

	print((int*)"pre ");
	if(prev != (int*)0)
		print(itoa(*(prev+2), string_buffer, 10, 0));
	println();

	print((int*)"next ");
	if(next != (int*)0)
		print(itoa(*(next+2), string_buffer, 10, 0));
	println();

	print((int*)"key ");
	if(element != (int*)0)
		print(itoa(*(element+2), string_buffer, 10, 0));
	println();

	print((int*)"pc ");
	if(element != (int*)0)
		print(itoa(*(element+3), string_buffer, 10, 0));
	println();
	
}
void printListElement(int *element){
	print((int*)"key ");
	if(element != (int*)0)
		print(itoa(*(element+2), string_buffer, 10, 0));
	println();
}

//initialize head and tail
//@return: initialized borders
int* initList(){
	int *list;
	int *head;
	int *tail;
	// borders are head and tail of the list
	// head and tail point to its allocated memory that contain the address of head and tail
	list = malloc (2*4);
	head = malloc(4);
	tail = malloc(4);
	*head = 0;
	*tail = 0;
	*list = (int)head;
	*(list+1) = (int)tail;
	return list;
}

int* getSegmentationTableEntry(int key, int *list){
	int *entry;
	entry = findElementByKey(key, list);
	return entry;
}

// create list element
// key starts at 0 and increases per process
// @return: new list element
int* createProcess(int key){
	int *newElement;
	int *segEntry;
	newElement = malloc (6*4);
	*(newElement+0) = 0;	// prev
	*(newElement+1) = 0;	// next
	*(newElement+2) = key;	// unique key
	*(newElement+3) = 0;	// pc
	*(newElement+4) = (int)malloc(32*4);	// registers
	segEntry = getSegmentationTableEntry(key, segmentationTable);
	*(newElement+5) = (int)segEntry;	// pointer to entry in segmentation table
	
	return newElement;
}


// @return: pointer to first element of list
int* pollListHead(int *list){
	return (int*)*list;
}

// @return: pointer to last element of list
int* pollListTail(int *list){
	return (int*)*(list+1);
}

// @return: 1 if list is empty
// 			0 otherwise
int isListEmpty(int *list){
	int *pToHead;
	pToHead = pollListHead(list);
	if(*pToHead == 0)
		return 1;
	return 0;
}


// add element at the end of the list
void appendListElement(int *newElement, int *list){
	int *pToHead;
	int *pToTail;
	int *tail;
	pToHead = pollListHead(list);
	pToTail = pollListTail(list);
	if(isListEmpty(list)){	// if list is empty
		*pToHead = (int)newElement;
	} else {	// otherwise
		*newElement = *pToTail;
		tail = (int*)*pToTail;
		*(tail+1) = (int)newElement;
	}
	*pToTail = (int)newElement;
	*(newElement+1) = 0;
}

void insertListElementAtBeginning(int *newElement, int *list){
	int *pToHead;
	int *pToTail;
	int *head;
	pToHead = pollListHead(list);
	pToTail = pollListTail(list);

	if(isListEmpty(list)){	// if list is empty
		*pToTail = (int)newElement;
	} else {	// otherwise
		*(newElement+1) = *pToHead;
		head = (int*)*pToHead;
		*head = (int)newElement;
	}
	*pToHead = (int)newElement;

}

// print the list
// if type == 0: print processes
// if type == 1: print segmentation table 
void printListVerbose(int *list, int type){
	int *pToHead;
	int *pToTail;
	int *curr;
	pToHead = pollListHead(list);
	pToTail = pollListTail(list);

	print((int*)"print list start");
	println();
	if(isListEmpty(list)==0){	// if list not empty
		curr = (int*)*pToHead;
		while((int)curr != 0){
			if(type == 0)
				printListElementVerbose(curr);
			else if(type == 1)
				printSegmentationTableEntryVerbose(curr);
			else if(type ==2)
				printProcess(curr);
			curr = (int*)*(curr+1);
		}
	} else {
		println();
	}

	println();
	print((int*)"print list end");

}
// print the list
// if type == 0: print listelement
// if type == 1: print segmentation table 
// if type == 2: print whole process (listelement+segmentation table entry)
void printList(int *list, int type){
	int *pToHead;
	int *pToTail;
	int *curr;
	pToHead = pollListHead(list);
	pToTail = pollListTail(list);

	print((int*)"print list start");
	println();
	if(isListEmpty(list)==0){	// if list not empty
		curr = (int*)*pToHead;
		while((int)curr != 0){
			if(type == 0)
				printListElement(curr);
			else if(type == 1)
				printSegmentationTableEntry(curr);
			else if(type ==2)
				printProcess(curr);
			curr = (int*)*(curr+1);
		}
	} 

	print((int*)"print list end");
	println();

}

// remove first element of the list
// @return: pointer to the element
//		    0 if list is empty
int* removeFirst(int *list){
	int *pToHead;
	int *pToTail;
	int *curr;
	int *next;
	int *head;
	int *tail;

	pToHead = pollListHead(list);
	pToTail = pollListTail(list);
	
	if(isListEmpty(list)){ // list is empty
		return (int*)0;
	} else if(*pToHead == *pToTail){ // list contains only 1 element
		curr = (int*)*pToHead;
		head = malloc(4);
		tail = malloc(4);
		*head = 0;
		*tail = 0;
		*list = (int)head;
		*(list+1) = (int)tail;
	} else {	// otherwise
		curr = (int*)*pToHead;
		next = (int*)*(curr+1);
		*(curr+1) = 0;
		*next = 0;
		*pToHead = (int)next;
	}
	return curr;
}

// does only sort the key attributes at the moment
//TODO reset pointer
void sortList(int *borders){
	int *pToHead;
	int *pToTail;
	int *curr;
	int *next;
	int unsorted;
	int changes;
	int tmp;

	pToHead = pollListHead(borders);
	pToTail = pollListTail(borders);
	
	unsorted = 1;
	changes = 0;
	
	while(unsorted){
		next = (int*)*pToHead;
		changes = 0;
		while(next != (int*)*pToTail){
			curr = next;
			next = (int*)*(curr+1);
			
			if(*(curr+2) > *(next+2)){
				tmp = *(curr+2);
				*(curr+2) = *(next+2);
				*(next+2) = tmp;
				changes = 1;
			}
		}
		if(changes == 0){
			unsorted = 0;
		}
	}
}

// @return: list element with key "key"
//			0 if not in list
int* findElementByKey(int key, int *list){
	int *pToHead;
	int *pToTail;
	int *curr;
	pToHead = pollListHead(list);
	pToTail = pollListTail(list);

	if(isListEmpty(list) == 0){
		curr = (int*)*pToHead;
		while(curr != (int*)*pToTail){
			if(*(curr+2) == key){
				return curr;
			}
			curr = (int*)*(curr+1);
		}
		if(*(curr+2) == key){
			return curr;
		}
	}
	return (int*)0;
}

// save pc and registers of current process
void saveProcessState(){
	*(currProcess+3) = pc;
	*(currProcess+4) = (int)registers;
}

// set pc, registers and memory segment of current process
void setProcessState(){
	currSegment = (int*)*(currProcess + 5);
	registers = (int*)*(currProcess + 4);
	pc = *(currProcess + 3);
	memory = (int*)*(currSegment + 3);
	currSegmentPos = (int)memory;
	currSegmentSize = *(currSegment+4);
}

int* createSegmentationTableEntry(int key, int segmentSize){
	int *newSegmentationTableEntry;
	currSegmentSize = segmentSize;
	usedMemorySize = usedMemorySize + segmentSize;
	if(usedMemorySize > memorySize){
		exception_handler(EXCEPTION_SEGMENTATIONFAULT);
	}
	newSegmentationTableEntry = malloc(5*4);
	*(newSegmentationTableEntry+0) = 0;	// prev
	*(newSegmentationTableEntry+1) = 0;	// next
	*(newSegmentationTableEntry+2) = key;	// unique key
	*(newSegmentationTableEntry+3) = (int)memoryBumpPointer;	// memory segment
	*(newSegmentationTableEntry+4) = segmentSize;		// segmentSize
	memoryBumpPointer = memoryBumpPointer + segmentSize;

	return newSegmentationTableEntry;
}

void switchProcess(int finished){//finished = 1, not finished = 0
	if(finished == 1){
		lock = 0;
		if(isListEmpty(blockedQueue)==0){
			currProcess = removeFirst(blockedQueue);
			lock = 1;
			lockID = *(currProcess+2);
			setProcessState();
		} else if (isListEmpty(readyQueue) == 0){
			currProcess = removeFirst(readyQueue);
			setProcessState();
		} else {
			continueExecuting();
		}
	} else if (finished == 0){
		saveProcessState();
		if(lock==1){
			if(lockID != *(currProcess+2)){
				appendListElement(currProcess, blockedQueue);
			} else 
				appendListElement(currProcess, readyQueue);
		} else
			appendListElement(currProcess, readyQueue);

		if(lock == 0){
			if(isListEmpty(blockedQueue)==0) {
				currProcess = removeFirst(blockedQueue);
				lock = 1;
				lockID = *(currProcess+2);
				setProcessState();
			} else {
				currProcess = removeFirst(readyQueue);
				setProcessState();
			}
		} else if (lock == 1) {
			currProcess = removeFirst(readyQueue);
			setProcessState();
		}
	}
}
void testDoubleLinkedList(){
	int *borders;
	int *head;
	int *newElement;
	int *find;
	print((int*)"testDoubleLinkedList");
	borders = initList();
	segmentationTable = initList();

	newElement = createProcess(65);
	appendListElement(newElement, borders);

	// expected output: 0,65,0 	
	printList(borders, 0);

	newElement = createProcess(66);
	appendListElement(newElement, borders);

	// expected output: 0,65,66		65,66,0
	printList(borders, 0);

	newElement = pollListHead(borders);
	removeFirst(borders);

	// expected output: 0,66,0 	
	printList(borders, 0);
	
	removeFirst(borders);

	// expected output:  	
	printList(borders, 0);
	
	printListElement((int*)*newElement);
	appendListElement((int*)*newElement, borders);

	// expected output: 0,66,0 	
	printList(borders, 0);
	
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     E M U L A T O R   ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ------------------------- INSTRUCTIONS --------------------------
// -----------------------------------------------------------------

void fct_syscall() {
    if (debug_disassemble) {
        printFunction(function);
        println();
    }

    if (*(registers+REG_V0) == SYSCALL_EXIT) {
        syscall_exit();
    } else if (*(registers+REG_V0) == SYSCALL_READ) {
        syscall_read();
    } else if (*(registers+REG_V0) == SYSCALL_WRITE) {
        syscall_write();
    } else if (*(registers+REG_V0) == SYSCALL_OPEN) {
        syscall_open();
    } else if (*(registers+REG_V0) == SYSCALL_MALLOC) {
        syscall_malloc();
    } else if (*(registers+REG_V0) == SYSCALL_GETCHAR) {
        syscall_getchar();
    } else if (*(registers+REG_V0) == SYSCALL_YIELD) {
        syscall_yield();
    } else if (*(registers+REG_V0) == SYSCALL_LOCK) {
        syscall_lock();
    } else if (*(registers+REG_V0) == SYSCALL_UNLOCK) {
        syscall_unlock();
    } else {
        exception_handler(EXCEPTION_UNKNOWNSYSCALL);
    }

    pc = pc + 4;
}

void fct_nop() {
    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        println();
    }
}

void op_jal() {
    *(registers+REG_RA) = pc + 8;

    pc = instr_index * 4;

    // TODO: execute delay slot

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        print(itoa(instr_index, string_buffer, 16, 8));
        println();
    }
}

void op_j() {
    pc = instr_index * 4;

    // TODO: execute delay slot

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        print(itoa(instr_index, string_buffer, 16, 8));
        println();
    }
}

void op_beq() {
    pc = pc + 4;

    if (*(registers+rs) == *(registers+rt)) {
        pc = pc + signExtend(immediate) * 4;

        // TODO: execute delay slot
    }

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        println();
    }
}

void op_bne() {
    pc = pc + 4;

    if (*(registers+rs) != *(registers+rt)) {
        pc = pc + signExtend(immediate) * 4;

        // TODO: execute delay slot
    }

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        println();
    }
}

void op_addiu() {
    *(registers+rt) = *(registers+rs) + signExtend(immediate);

    // TODO: check for overflow

    pc = pc + 4;

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rt);
        putchar(',');
        printRegister(rs);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        println();
    }
}

void fct_jr() {
    pc = *(registers+rs);

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rs);
        println();
    }
}

void op_lui() {
    *(registers+rt) = leftShift(immediate, 16);

    pc = pc + 4;

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rt);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        println();
    }
}

void fct_mfhi() {
    *(registers+rd) = reg_hi;

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rd);
        println();
    }
}

void fct_mflo() {
    *(registers+rd) = reg_lo;

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rd);
        println();
    }
}

void fct_multu() {
    // TODO: 64-bit resolution currently not supported
    reg_lo = *(registers+rs) * *(registers+rt);

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

void fct_divu() {
    reg_lo = *(registers+rs) / *(registers+rt);
    reg_hi = *(registers+rs) % *(registers+rt);

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

void fct_addu() {
    *(registers+rd) = *(registers+rs) + *(registers+rt);

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rd);
        putchar(',');
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

void fct_subu() {
    *(registers+rd) = *(registers+rs) - *(registers+rt);

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rd);
        putchar(',');
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

void op_lw() {
    int vaddr;

    vaddr = *(registers+rs) + signExtend(immediate);

    *(registers+rt) = loadMemory(vaddr);

    pc = pc + 4;

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rt);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        putchar('(');
        printRegister(rs);
        putchar(')');
        println();
    }
}

void fct_slt() {
    if (*(registers+rs) < *(registers+rt))
        *(registers+rd) = 1;
    else
        *(registers+rd) = 0;

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rd);
        putchar(',');
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

void op_sw() {
    int vaddr;

    vaddr = *(registers+rs) + signExtend(immediate);

    storeMemory(vaddr, *(registers+rt));

    pc = pc + 4;

    if (debug_disassemble) {
        printOpcode(opcode);
        print((int*) " ");
        printRegister(rt);
        putchar(',');
        print(itoa(signExtend(immediate), string_buffer, 10, 0));
        putchar('(');
        printRegister(rs);
        putchar(')');
        println();
    }
}

void fct_teq() {
    if (*(registers+rs) == *(registers+rt))
        exception_handler(EXCEPTION_SIGNAL);

    pc = pc + 4;

    if (debug_disassemble) {
        printFunction(function);
        print((int*) " ");
        printRegister(rs);
        putchar(',');
        printRegister(rt);
        println();
    }
}

// -----------------------------------------------------------------
// -------------------------- INTERPRETER --------------------------
// -----------------------------------------------------------------

void printException(int enumber) {
    print((int*) *(EXCEPTIONS + enumber));
}

void exception_handler(int enumber) {
    print(binaryName);
    print((int*) ": exception: ");
    printException(enumber);
    println();

    exit(enumber);
}

void pre_debug() {
    if (debug_disassemble) {
        print(binaryName);
        print((int*) ": $pc=");
        print(itoa(pc, string_buffer, 16, 8));
        print((int*) ": ");
    }
}

void post_debug() {
    int i;
    if (debug_registers) {
        i = 0;

        while (i < 32) {
            if (*(registers+i) != 0) {
                print(binaryName);
                print((int*) ": ");
                printRegister(i);
                putchar(CHAR_EQUAL);
                print(itoa(*(registers+i), string_buffer, 16, 8));
                println();
            }
            i = i + 1;
        }
        println();
    }
}

void fetch() {
    ir = loadMemory(pc);
}

void execute() {
    if (opcode == OP_SPECIAL) {
        if (function == FCT_NOP) {
            fct_nop();
        } else if (function == FCT_ADDU) {
            fct_addu();
        } else if (function == FCT_SUBU) {
            fct_subu();
        } else if (function == FCT_MULTU) {
            fct_multu();
        } else if (function == FCT_DIVU) {
            fct_divu();
        } else if (function == FCT_MFHI) {
            fct_mfhi();
        } else if (function == FCT_MFLO) {
            fct_mflo();
        } else if (function == FCT_SLT) {
            fct_slt();
        } else if (function == FCT_JR) {
            fct_jr();
        } else if (function == FCT_SYSCALL) {
            fct_syscall();
        } else if (function == FCT_TEQ) {
            fct_teq();
        } else {
            exception_handler(EXCEPTION_UNKNOWNINSTRUCTION);
        }
    } else if (opcode == OP_ADDIU) {
        op_addiu();
    } else if (opcode == OP_LW) {
        op_lw();
    } else if (opcode == OP_SW) {
        op_sw();
    } else if (opcode == OP_BEQ) {
        op_beq();
    } else if (opcode == OP_BNE) {
        op_bne();
    } else if (opcode == OP_JAL) {
        op_jal();
    } else if (opcode == OP_J) {
        op_j();
    } else {
        exception_handler(EXCEPTION_UNKNOWNINSTRUCTION);
    }
}

void run() {
	int *listHead;
	currProcess = removeFirst(readyQueue);
	setProcessState();
 	
 	while (1) {
	    fetch();
	    if (*(registers+REG_V0) == SYSCALL_EXIT) { // if SYSCALL_EXIT is next instruction, lock = 0, otherwise unlock was not called
			switchProcess(1);
       	} else
       		continueExecuting();
	}

}

void continueExecuting(){
	decode();
	pre_debug();
	execute();
	post_debug();
}
void parse_args(int argc, int *argv) {
    // assert: ./selfie -m size executable {-m size executable}

    // memory size in bytes and executable file name
    initMemory(atoi((int*) *(argv+2)) * 1024 * 1024, (int*) *(argv+3));

    print(binaryName);
    print((int*) ": memory size ");
    print(itoa(memorySize / 1024 / 1024, string_buffer, 10, 0));
    print((int*) "MB");
    println();
}

void up_push(int value) {
    int vaddr;
    // allocate space for one value on the stack
    *(registers+REG_SP) = *(registers+REG_SP) - 4;

    // compute address
    vaddr = *(registers+REG_SP);
	
    // store value
    storeMemory(vaddr, value);
}

int up_malloc(int size) {
    *(registers+REG_A0) = size;

    syscall_malloc();

    return *(registers+REG_V0);
}

void up_copyArguments(int argc, int *argv) {
    int vaddr;
    up_push(argc);

    vaddr = up_malloc(argc * 4);

    up_push(vaddr);

    while (argc > 0) {
        storeMemory(vaddr, up_malloc(stringLength((int*) *argv) + 1));

        copyStringToMemory((int*) *argv, loadMemory(vaddr));

        vaddr = vaddr + 4;

        argv = argv + 1;
        argc = argc - 1;
    }
}

int main_emulator(int argc, int *argv) {
	int counter;
	int segmentSize;
	int vaddr;
	isEmulating = 1;
	segmentSize = 1024*1024;
	counter = 0;
	counterProcesses = 2;
	
	readyQueue = initList();
    segmentationTable = initList();
	blockedQueue = initList();
    initInterpreter();
    parse_args(argc, argv);
	
//	loadBinary();
	while(counter < counterProcesses){

		currSegment = createSegmentationTableEntry(counter, segmentSize);
		appendListElement(currSegment, segmentationTable);
		currProcess = createProcess(counter);
		appendListElement(currProcess, readyQueue);

		setProcessState();

	loadBinary();

		*(registers+REG_GP) = binaryLength;
		*(registers+REG_K1) = *(registers+REG_GP);
	    *(registers+REG_SP) = *(currSegment+4)-4;
	    
		binaryLength = 0;
		up_copyArguments(argc-3, argv+3);
		counter = counter + 1;
	}
    run();

    exit(0);
}

// -----------------------------------------------------------------
// ----------------------------- MAIN ------------------------------
// -----------------------------------------------------------------

int main(int argc, int *argv) {
    int *firstParameter;
    initLibrary();

    initRegister();
    initDecoder();
   
     if (argc > 1) {
        firstParameter = (int*) *(argv+1);

        if (getCharacter(firstParameter, 0) == '-') {
            if (getCharacter(firstParameter, 1) == 'c')
                main_compiler();
            else if (getCharacter(firstParameter, 1) == 'm') {
                if (argc > 3)
                    main_emulator(argc, (int*) argv);
                else
                    exit(-1);
            } else if(getCharacter(firstParameter, 1) == 'l'){
            	testDoubleLinkedList();
            }
            else {
                exit(-1);
            }
        } else {
            exit(-1);
        }
    } else
        // default: compiler
        main_compiler();
}
