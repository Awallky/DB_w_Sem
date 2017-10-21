# a simple makefile
EXEDIR = exe

all: $(EXEDIR)/Load $(EXEDIR)/Print $(EXEDIR)/Query $(EXEDIR)/Change $(EXEDIR)/Clean

$(EXEDIR)/Load: load.c ./../include/header.h
	gcc -g -o $(EXEDIR)/Load load.c header.c
$(EXEDIR)/Print: print.c ./../include/header.h
	gcc -g -o $(EXEDIR)/Print print.c header.c
$(EXEDIR)/Query: query.c ./../include/header.h
	gcc -g -o $(EXEDIR)/Query query.c header.c
$(EXEDIR)/Change: change.c ./../include/header.h
	gcc -g -o $(EXEDIR)/Change change.c header.c
$(EXEDIR)/Clean: clean.c ./../include/header.h
	gcc -g -o $(EXEDIR)/Clean clean.c header.c
clean: 
	rm -f $(EXEDIR)/*  *~ core
