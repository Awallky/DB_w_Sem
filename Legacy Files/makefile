/* a simple makefile */

all: create change

create: create.c header.h
        gcc -o create create.c header.c
change: change.c header.h
        gcc -o change change.c header.c
clean:
        rm -f create  change  *~ core
