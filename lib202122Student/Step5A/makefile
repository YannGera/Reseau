vanstapelsalefilsdeputedemerde: admin cli ser 

LibSerHV.o: LibSerHV.c LibSerHV.h data.h
	echo "compilation de LibSerHV"
	cc -c LibSerHV.c
admin: data.h admin.c LibSerHV.o
	echo "Compilation de admin"
	cc -o admin admin.c LibSerHV.o
udplib.o: ../udplib/udplib.h ../udplib/udplib.c
	echo "Compilation de udplib.o"
	cc -c ../udplib/udplib.c
cli: cli.c requetehv.h data.h udplib.o
	echo "Compilation de client"
	cc -o cli cli.c udplib.o $(LIBS)
ser: ser.c requetehv.h data.h udplib.o LibSerHV.o
	echo "Compilation de serveur"
	cc -o ser ser.c udplib.o LibSerHV.o $(LIBS)