/*---------------------------------------------------------------
   Vanstapel Herman
   ex02\cli.c

 Le client dit bonjour en utilisant un structure  et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "structure.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
 int rc ;
 int Desc ;
 int tm ; 
 
 u_long  IpSocket , IpServer;
 u_short PortSocket, PortServer ; 
 
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 struct RequeteHV UneRequete ;

 memset(&sthis,0,sizeof(struct sockaddr_in)) ;
 memset(&sos,0,sizeof(struct sockaddr_in)) ; 
 memset(&sor,0,sizeof(struct sockaddr_in)) ; 
 
 if (argc!=5)
  
{
    printf("cli client portc serveur ports\n") ;
    exit(1) ;
 }
 
 /* Récupération IP & port   */
 IpSocket= inet_addr(argv[1]);
 PortSocket = atoi(argv[2]);
 
 IpServer = inet_addr(argv[3]) ;
 PortServer = atoi(argv[4]);
 
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ; 
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 
  sos.sin_family = AF_INET ;
  sos.sin_addr.s_addr= IpServer ;
  sos.sin_port = htons(PortServer) ;

 
 UneRequete.Type = Question ; 
 // strncpy(UneRequete.Marque , "Avec une structure: Bonjour" , sizeof(UneRequete.Marque)) ;
 // UneRequete.Reference = ;
 UneRequete.NumeroFacture = 0;
 UneRequete.Date = 0;
 UneRequete.Reference = 5;
 UneRequete.Prix = 0;
 strcpy(UneRequete.Marque, "");
 strcpy(UneRequete.Modele, "");
 strcpy(UneRequete.NomClient, "Paul");

 
 rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sos ) ;

 if ( rc == -1 )
    die("SendDatagram") ;
 else
   fprintf(stderr,"Envoi de %d bytes\n",rc ) ;
 
 memset(&UneRequete,0,sizeof(struct RequeteHV)) ;
 tm = sizeof(struct RequeteHV) ;
 struct RequeteHV UneRequete1;
  rc = ReceiveDatagram( Desc, &UneRequete1,tm, &sor ) ;
 if ( rc == -1 )
    die("ReceiveDatagram") ;
 else
   fprintf(stderr,"bytes recus:%d:%s\n",rc,UneRequete1.Marque ) ;
 printf("<TypeRequete %d\n", UneRequete1.Type);
 printf("Reference %d\n", UneRequete1.Reference);
 printf("NumeroFacture %d\n", UneRequete1.NumeroFacture);
 printf("Date %d\n", UneRequete1.Date);
 printf("Reference %d\n", UneRequete1.Reference);
 printf("Prix %d\n", UneRequete1.Prix);
 printf("Marque %s\n", UneRequete1.Marque);
 printf("Modele %s\n", UneRequete1.Modele);
 printf("Client %s\n", UneRequete1.NomClient);
 close(Desc) ;
}
