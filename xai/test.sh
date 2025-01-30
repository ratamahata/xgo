#Test script, to verify correctness of C++ routines

#clean previous build
rm -f *.o
rm -f testxo

#compile

g++  -std=c++11 -o TNode.o -c TNode.cpp
g++  -std=c++11 -o Cursor.o -c Cursor.cpp
g++  -o Relator.o -c Relator.cpp
g++  -o SimplyNumbers.o -c SimplyNumbers.cpp
g++  -o Expander.o -c Expander.cpp
g++  -o Evaluator.o -c Evaluator.cpp
g++  -o Logger.o -c Logger.cpp
g++  -o Hashtable.o -c Hashtable.cpp
g++  -o Builder.o -c Builder.cpp
g++  -o Grower.o -c Grower.cpp
g++  -o GameBoard.o -c GameBoard.cpp

#build test utility:
mv Test.txt Test.cpp
g++  -o testxo Test.cpp GameBoard.o Grower.o Builder.o Hashtable.o Logger.o Evaluator.o Expander.o SimplyNumbers.o Relator.o TNode.o Cursor.o
mv Test.cpp Test.txt

#run test, upon success should print success
./testxo

#clean
rm -f *.o
rm -f testxo
