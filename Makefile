PATH_CPP = .\Cpp_files
PATH_HEADER = .\Headers\
HEADERS_NAME = FuncStack.h Check.h Constants.h Stack.h Error.h ProcessorConstants.h
HEADERS = $(HEADERS_NAME:%.h=$(PATH_HEADER)%.h)

flags = -IHeaders -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

assemble: assembler.exe
	@.\assembler.exe

assembler.exe:Compiler.o
	@g++ Compiler.o -o assembler.exe

Compiler.o: $(PATH_CPP)\Compiler.cpp $(HEADERS) 
	@g++ -c $(flags) $(PATH_CPP)\Compiler.cpp 

process: processor.exe
	@.\processor.exe

processor.exe: Check.o FuncStack.o ProcessorMain.o
	@g++ Check.o FuncStack.o ProcessorMain.o -o processor.exe

Check.o: $(PATH_CPP)\Check.cpp $(HEADERS)
	@g++ -c $(flags) $(PATH_CPP)\Check.cpp

FuncStack.o: $(PATH_CPP)\FuncStack.cpp $(HEADERS)
	@g++ -c $(flags) $(PATH_CPP)\FuncStack.cpp

ProcessorMain.o: $(PATH_CPP)\ProcessorMain.cpp $(HEADERS)
	@g++ -c $(flags) $(PATH_CPP)\ProcessorMain.cpp

