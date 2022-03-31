### 
## Makefile skeleton
## INFO0030: Projet 1
## 
### 

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes
LD=gcc
LDFLAGS=

# Files
EXEC=pnm
MODULES=main.c lib/pnm.c
OBJECTS=main.o pnm.o

## Rules

all: $(EXEC)

pnm: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

pnm.o: lib/pnm.c
	$(CC) -c lib/pnm.c -o pnm.o $(CFLAGS)

clean:
	rm -f *.o $(EXEC) *~

