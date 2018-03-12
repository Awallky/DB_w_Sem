# a simple makefile
EXEDIR = exe
H=./../include/header.h
C=header.c

all: $(EXEDIR)/Load $(EXEDIR)/Print $(EXEDIR)/Query $(EXEDIR)/Change $(EXEDIR)/Clean

$(EXEDIR)/Load: load.c $(H)
	gcc -g -o $(EXEDIR)/Load load.c $(C)
$(EXEDIR)/Print: print.c $(H)
	gcc -g -o $(EXEDIR)/Print print.c $(C)
$(EXEDIR)/Query: query.c $(H)
	gcc -g -o $(EXEDIR)/Query query.c $(C)
$(EXEDIR)/Change: change.c $(H)
	gcc -g -o $(EXEDIR)/Change change.c $(C)
$(EXEDIR)/Clean: clean.c $(H)
	gcc -g -o $(EXEDIR)/Clean clean.c $(C)
clean: 
	rm -f $(EXEDIR)/*  *~ core
