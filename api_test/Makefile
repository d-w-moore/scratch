
default: all
all: main lib

CC=/opt/irods-externals/clang3.7-0/bin/clang++

BOOST_INC=-I/opt/irods-externals/boost1.59.0-0/include

clean:
	rm *.o *.so dyn_ex

main:
	${CC} -g -std=c++11 ${BOOST_INC} -fPIC -c main.cpp -o main.o
	${CC} -g -std=c++11 -Wl,-E -o dyn_ex main.o -ldl

lib:
	${CC} -g -std=c++11 ${BOOST_INC} -fPIC -c dynamic_loader_example.cpp -o dynamic_loader_example.o
	${CC} -g -std=c++11 -shared -o dynamic_loader_example.so dynamic_loader_example.o

