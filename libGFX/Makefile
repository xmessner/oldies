# Librairie Grafik 10/01
#.EXPORT_ALL_VARIABLES:

CC = gcc
LIB = ./lib/
INC = ./include/
INSTLIB = /usr/local/lib/
INSTINC = /usr/local/include/
OBJ = ./obj/
SRC = ./src/
BIN = ./bin/
TEST = ./test/

LIBARGS = -shared -O2
#CCARGS = -lvgagl -lvga -lMXsvga
CCARGS = -lpthread -lSDL -lGFX

#all: clean libMXsvga install
all: libGFX

libGFX:
	$(CC) $(SRC)fileio.c -I$(INC) -c -o $(OBJ)fileio.o && \
	$(CC) $(SRC)primitives.c -I$(INC) -c -o $(OBJ)primitives.o && \
	$(CC) $(SRC)effets.c -I$(INC) -c -o $(OBJ)effets.o && \
	$(CC) $(SRC)kernel.c -I$(INC) -c -o $(OBJ)kernel.o && \
	$(CC) $(SRC)fps.c -I$(INC) -c -o $(OBJ)fps.o && \
	$(CC) $(OBJ)*.o $(LIBARGS) -o $(LIB)libGFX.so

clean:
	rm $(OBJ)*.o $(LIB)*.so
	
install:
	cp $(LIB)libGFX.so $(INSTLIB)
	cp $(INC)*.h $(INSTINC)
	ldconfig

test:
	$(CC) $(TEST)test.c $(CCARGS) -o $(BIN)test
	
	
