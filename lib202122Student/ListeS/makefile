# ListeS

LIBS=
all:	main	dnslib.o

dnslib.o:	../dnslib/dnslib.h	../dnslib/dnslib.c
	echo "Compilation de ListeSimple"
	cc -c ../dnslib/dnslib.c	

main:	main.c	dnslib.o
	echo "Compilation de client"
	cc  -o main	main.c dnslib.o	$(LIBS)

