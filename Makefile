### 
## Makefile skeleton
## INFO0030: Projet 1
## 
### 

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes

# Files
EXEC=pnm
LIBS=liblfsr/lfsr.a  libpnm/libpnm.a

## Rules

all: $(EXEC)

$(EXEC): $(LIBS) main.c
	$(CC) -o $(EXEC) main.c -lpnm -llfsr -L liblfsr -L libpnm


liblfsr/lfsr.a:
	cd liblfsr; make

libpnm/libpnm.a:
	cd libpnm; make

clean:
	cd liblfsr; make clean
	cd libpnm; make clean

