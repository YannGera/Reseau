
# cphex\makefile

LIBS=
all:	admin	LibSerHV.o


LibSerHV.o:	LibSerHV.c	LibSerHV.h	data.h	
	echo "compilation de LibSerHV"
	gcc -c LibSerHV.c

admin:	data.h	admin.c	LibSerHV.o			
	echo "Compilation de admin"
	gcc -o admin	admin.c	LibSerHV.o 

clean:
	@rm -f LibSerHV.o core

clobber:	clean
	@rm -f tags admin