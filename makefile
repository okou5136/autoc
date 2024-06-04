CC = gcc 
CFLAG = 
SOURCES = ./src/autoc.c
DESTINATION = ./exe/autoc

all: comp run clean

comp:
    $(CC) $(SOURCES) -o $(DESTINATION)
run:
    $(DESTINATION)
clean:
    rm $(DESTINATION)


