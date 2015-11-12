
default: all
all: main lib

CC=/opt/irods-externals/clang3.7-0/bin/clang++

AVROINC = -I/opt/irods-externals/avro1.7.7-0/include
BOOSTINC = -I/opt/irods-externals/boost1.59.0-0/include
PYTHONINC = -I/usr/include/python2.7

BOOSTLIB = -Wl,-rpath,/opt/irods-externals/boost1.59.0-0/lib/ \
		   -L/opt/irods-externals/boost1.59.0-0/lib/ \
	       -lboost_python \
	       -lboost_filesystem \
	       -lboost_system \
	       -lboost_iostreams
AVROLIB = -Wl,-rpath,/opt/irods-externals/avro1.7.7-0/lib/ \
          -L/opt/irods-externals/avro1.7.7-0/lib/ \
		  -lavrocpp

INC= ${BOOSTINC} ${PYTHONINC} ${AVROINC}
LIBS= ${AVROLIB} ${BOOSTLIB} -ldl -lpython2.7 -lz

clean:
	rm *.o *.so dyn_ex

main:
	${CC} -g -std=c++11 ${INC} -fPIC -c main.cpp -o main.o
	${CC} -g -std=c++11 -Wl,-E -o dyn_ex main.o ${LIBS}

lib:
	${CC} -g -std=c++11 ${INC} -fPIC -c dynamic_loader_example.cpp -o dynamic_loader_example.o
	${CC} -g -std=c++11 -shared -o dynamic_loader_example.so dynamic_loader_example.o

