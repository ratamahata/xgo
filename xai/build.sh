#clean previous build
rm *.o
rm libxai.so
rm libxai_wrap.so
rm xai.go
rm xai_wrap.c

#4. Compile the 32-bit C/C++ Code
#Compile the C/C++ code explicitly as 32-bit using the -m32 flag:

g++ -m32 -std=c++11 -o TNode.o -c TNode.cpp
g++ -m32 -std=c++11 -o Cursor.o -c Cursor.cpp
g++ -m32 -o Relator.o -c Relator.cpp
g++ -m32 -o SimplyNumbers.o -c SimplyNumbers.cpp
g++ -m32 -o Expander.o -c Expander.cpp
g++ -m32 -o Evaluator.o -c Evaluator.cpp
g++ -m32 -o Logger.o -c Logger.cpp
g++ -m32 -o Hashtable.o -c Hashtable.cpp
g++ -m32 -o Builder.o -c Builder.cpp
g++ -m32 -o GameBoard.o -c GameBoard.cpp

#static library:
#ar rcs libxai.a GameBoard.o Builder.o Hashtable.o Logger.o Evaluator.o Expander.o SimplyNumbers.o Relator.o TNode.o Cursor.o

#or, shared library:
g++ -m32 -shared -o libxai.so GameBoard.o Builder.o Hashtable.o Logger.o Evaluator.o Expander.o SimplyNumbers.o Relator.o TNode.o Cursor.o

#5. Generate Go Bindings with SWIG
#Run SWIG to generate the Go wrapper code for your interface file. 
#Include the -go option for Go support and the Go module name.

swig -go -cgo -intgosize 64 -c++ -o xai_wrap.c xai.swigcxx

#6. **Compile the Wrapper Code as 32-bit**
#Compile the generated wrapper file (`add_wrap.c`) with the `-m32` flag to ensure 32-bit compatibility:

g++ -m32 -o xai_wrap.o -c xai_wrap.c

#Link the compiled wrapper with the earlier compiled 32-bit code:
#- If using a static library:

g++ -shared -m32 -o libxai_wrap.so xai_wrap.o -L. -lxai

#- If using an object file:

#g++ -m32 -o libxai_wrap.so xai_wrap.o GameBoard.o

