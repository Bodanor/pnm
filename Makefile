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

all: pnm basic_cypher advanced_cypher

pnm: $(LIBS) pnm.c
	@echo "[+] Making pnm"
	$(CC) -o pnm pnm.c -lpnm -llfsr -L liblfsr -L libpnm

basic_cypher: $(LIBS) basic_cypher.c
	@echo "[+] Making basic_cypher"
	$(CC) -o basic_cypher basic_cypher.c -lpnm -llfsr -L liblfsr -L libpnm

advanced_cypher: $(LIBS) advanced_cypher.c
	@echo "[+] Making advanced_cypher"
	$(CC) -o advanced_cypher advanced_cypher.c -lpnm -llfsr -L liblfsr -L libpnm

liblfsr/lfsr.a:
	cd liblfsr; make

libpnm/libpnm.a:
	cd libpnm; make

clean:
	cd liblfsr; make clean
	cd libpnm; make clean
	rm -rf pnm basic_cypher advanced_cypher

