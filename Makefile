ESC=\033
RED='${ESC}[38;5;198m'
GREEN='${ESC}[38;5;82m'
YELLOW='${ESC}[38;5;226m'
PURPLE='${ESC}[38;5;128m'
ORANGE='${ESC}[38;5;214m'
NC='\033[0m' # No Color

all:
	@make -s compiler
	@make -s vm

compiler:

	@echo -${YELLOW}Generating yacc output files...${NC};
	@make -s bison
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Compiling yacc output files...${NC};
	@make -s parser
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating flex output files...${NC};
	@make -s flex
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Compiling flex output files...${NC};
	@make -s lexer
	@echo -e ${GREEN}OK${NC}
	
	@echo -${YELLOW}Generating symtable dependancies...${NC};
	@make -s symtable
	@echo -e ${GREEN}OK${NC}
	
	@echo -${YELLOW}Generating quad dependancies...${NC};
	@make -s quads
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating binary code dependancies...${NC};
	@make -s binary
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating Alpha Compiler executable...${NC};
	@make -s alpha_comp
	@echo -e ${GREEN}OK${NC}

	@echo -e ${PURPLE}Alpha Compiler built successfully :\)${NC}

symtable: src/symboltable/symtable.c src/symboltable/Variable.c src/symboltable/Function.c
	gcc -g src/symboltable/Variable.c -o bin/symboltable/Variable.o -c
	gcc -g src/symboltable/Function.c -o bin/symboltable/Function.o -c
	gcc -g src/symboltable/symtable.c -o bin/symboltable/symtable.o -c

quads: src/icode/quads.c
	gcc -g src/icode/quads.c -o bin/icode/quads.o -c

bison: src/parser/parser.y
	bison --yacc --defines --output=src/parser/parser.c src/parser/parser.y 

debug-bison: src/parser/parser.y
	bison --yacc --defines --debug --verbose --output=src/parser/parser.c src/parser/parser.y 

parser: src/parser/parser.c
	gcc -g src/parser/parser.c -o bin/parser/parser.o -c

flex: src/lexer/scanner.l
	flex --outfile=src/lexer/scanner.c src/lexer/scanner.l

lexer: src/lexer/scanner.c
	gcc -g src/lexer/scanner.c -o bin/lexer/scanner.o -c
	
alpha_comp:	
	gcc -g bin/parser/*.o bin/lexer/*.o bin/symboltable/*.o bin/icode/*.o  bin/targetcode/*.o -o alphac

vm:
	@echo -${YELLOW}Generating libfuncs dependancies...${NC};
	@make -s lib
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating execute function dependancies...${NC};
	@make -s exec
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating execution enviroment dependancies...${NC};
	@make -s exec_env
	@echo -e ${GREEN}OK${NC}

	@echo -${YELLOW}Generating Virtual Machine executable...${NC};
	@make -s alpha_vm
	@echo -e ${GREEN}OK${NC}

	@echo -e ${ORANGE}Alpha VM built successfully :\) ${NC}

lib: src/libfuncs/libfuncs.c
	gcc -g src/libfuncs/libfuncs.c -o bin/libfuncs/libfuncs.o -c -lm

exec: src/instructions/execute_assign.c src/instructions/execute_arithmetic.c src/instructions/execute_logical.c src/instructions/execute_funcs.c src/instructions/execute_jump.c src/instructions/execute_table.c src/instructions/execute_nop.c
	gcc -g src/instructions/execute_assign.c -o bin/instructions/execute_assign.o -c
	gcc -g src/instructions/execute_arithmetic.c -o bin/instructions/execute_arithmetic.o -c
	gcc -g src/instructions/execute_logical.c -o bin/instructions/execute_logical.o -c
	gcc -g src/instructions/execute_funcs.c -o bin/instructions/execute_funcs.o -c
	gcc -g src/instructions/execute_jump.c -o bin/instructions/execute_jump.o -c
	gcc -g src/instructions/execute_table.c -o bin/instructions/execute_table.o -c
	gcc -g src/instructions/execute_nop.c -o bin/instructions/execute_nop.o -c

binary:	src/targetcode/alpha_bc.c
	gcc -g src/targetcode/alpha_bc.c -o bin/targetcode/alpha_bc.o -c

exec_env: src/avm/execution_enviroment.c src/avm/vm.c 
	gcc -g src/avm/execution_enviroment.c -o bin/avm/execution_enviroment.o -c
	gcc -g src/avm/vm.c -o bin/avm/vm.o -c

alpha_vm:
	gcc -g bin/instructions/*.o bin/libfuncs/libfuncs.o bin/avm/execution_enviroment.o bin/avm/vm.o -o alpha -lm


clean_all:
	@echo -${ORANGE}Cleaning all files...${NC};

	@make -s clean_compiler
	@make -s clean_vm
	@rm -f output/*.out;
	@rm -f output/*.aic
	@rm -f output/*.atc
	@rm -f *.abc

	@echo ${PURPLE}All files deleted${NC};

clean_compiler:
	@echo -${YELLOW}Cleaning compiler files...${NC};
	@rm -f bin/lexer/*.o;
	@rm -f bin/parser/*.o;
	@rm -f bin/avm/*.o;
	@rm -f bin/icode/*.o;
	@rm -f bin/symboltable/*.o;
	@rm -f bin/targetcode/*.o;
	@rm -f src/parser/parser.c;
	@rm -f src/parser/parser.h;
	@rm -f src/lexer/scanner.c;
	@rm -f parser.output
	@rm -f alpha.out
	@rm -f alphac;
	@echo ${GREEN}OK${NC};

clean_vm:
	@echo -${YELLOW}Cleaning vm files...${NC};
	@rm -f bin/instructions/*.o;
	@rm -f bin/libfuncs/*.o;
	@rm -f bin/avm/*.o;
	@rm -f alpha;
	@echo ${GREEN}OK${NC};