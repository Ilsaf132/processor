HEADERS = FuncStack.h Check.h Constants.h Stack.h

a.out: StackMain.o Check.o FuncStack.o
	g++ StackMain.o FuncStack.o Check.o 

StackMain.o: StackMain.cpp $(HEADERS)
	g++ -c StackMain.cpp

FuncStack.o: FuncStack.cpp $(HEADERS)
	g++ -c FuncStack.cpp

Check.o: Check.cpp $(HEADERS)
	g++ -c Check.cpp
