GCC=g++
MPICC=mpiCC
OUTPUT=Parallel Parallel2 
CFLAGS=-std=c99
SRC=./src
BIN=./bin
OBJ=./obj


$(OBJ)/ShortestPathV1.o:
	$(MPICC) $(CFLAGS) -c $(SRC)/ShortestPathV1.cpp -o $@

$(OBJ)/ShortestPathV2.o:
	$(MPICC) $(CFLAGS) -c $(SRC)/ShortestPathV2.cpp -o $@


$(OBJ)/Timer.o: $(SRC)/Timer.c
	$(GCC) $(CFLAGS) -c $(SRC)/Timer.c -o $@

Parallel: $(OBJ)/Timer.o $(OBJ)/ShortestPathV1.o
	$(MPICC) $(CFLAGS) $(OBJ)/ShortestPathV1.o $(OBJ)/Timer.o -o $(BIN)/$@
	
Parallel2: $(OBJ)/Timer.o $(OBJ)/ShortestPathV2.o
	$(MPICC) $(CFLAGS) $(OBJ)/ShortestPathV2.o $(OBJ)/Timer.o -o $(BIN)/$@

clean:
	rm -rf $(OBJ)/*.o $(BIN)/*
	
all:$(OUTPUT)