/*--------------------------------------
  Herman Vanstapel
  
  ex02\ser.c 
  
  Un serveur recevant une structure 
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "structure.h"
#include "LibSerHV.h"


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc,char *argv[])
{
 int rc ;
 int Desc ;
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 
 u_long  IpSocket ;
 u_short PortSocket ;
 
 int tm ;
 struct RequeteHV UneRequete ;

 memset(&sthis,0,sizeof(struct sockaddr_in)) ;
 memset(&sos,0,sizeof(struct sockaddr_in)) ; 
 memset(&sor,0,sizeof(struct sockaddr_in)) ; 
 
 printf("Ceci est le serveur\n") ;
 if ( argc!=3)
 {
  printf("ser ser port cli\n") ;
  exit(1) ;
 }
 
 /* Récupération IP & port   */
 IpSocket= inet_addr(argv[1]);
 PortSocket = atoi(argv[2]);
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ;
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 while(1)
 {
    tm = sizeof(struct RequeteHV) ;
    rc = ReceiveDatagram( Desc,&UneRequete ,tm, &sor ) ;
    if ( rc == -1 )
    die("ReceiveDatagram") ;
    else
    fprintf(stderr,"bytes recus:%d\n",rc) ;
    printf("Received packet from %s:%d\n", inet_ntoa(sor.sin_addr), ntohs(sor.sin_port));
    // printf("<TypeRequete %d\n", UneRequete.Type);
    printf("Reference %d\n", UneRequete.Reference);
    // printf("NumeroFacture %d\n", UneRequete.NumeroFacture);
    // printf("Date %d\n", UneRequete.Date);
    // printf("Reference %d\n", UneRequete.Reference);
    // printf("Prix %d\n", UneRequete.Prix);
    // printf("Marque %s\n", UneRequete.Marque);
    // printf("Modele %s\n", UneRequete.Modele);
    // printf("Client %s\n", UneRequete.NomClient);

    // int temp = UneRequete.Type;

    FILE *LogSer;

    LogSer = fopen("LogSer","w");
    if ( LogSer == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
    fseek(LogSer, 0, SEEK_END ) ;
    fwrite(&UneRequete,sizeof(UneRequete),1,LogSer);
    fclose(LogSer);

    // printf("Lecture \n");
    // LogSer = fopen("LogSer","r");
    // if ( LogSer == NULL )
    //    {
    //     fprintf(stderr,"Echec Ouverture\n") ;
    //     exit(0) ;
    //    }
    //    int tmp;
    //    fread(&tmp,sizeof(int),1,LogSer);
    //    printf("Lecture = %d \n",tmp);
    //    fclose(LogSer);
    /* attention l'enum peut être codé en short */
    /* reponse avec psos */
    printf("\n\n") ;
    printf("Recherche Véhicule \n");
    struct VehiculeHV   UnRecord ;
    int res;
    res=RechercheHV("VehiculesHV",UneRequete.Reference,&UnRecord);
    printf("Recherche Véhicule terminée\n");
    if(res)
    {
    char Tampon[80] ;
    printf("Ref : %d\n",UnRecord.Reference ) ;  
    printf("Marque : %s\n",UnRecord.Marque ) ;    
    printf("Modele : %s\n",UnRecord.Modele ) ;    
    printf("Quantite : %d\n",UnRecord.Quantite ) ; 
    printf("Puissance : %d\n",UnRecord.Puissance ) ;
    printf("\n") ;
    }
    else
    {
    printf("non\n");
    }
    // AfficheVehiculeHV(&UnRecord);

    UneRequete.Type = 1 ;

    strcpy(UneRequete.Marque,UnRecord.Marque);
    strcpy(UneRequete.Modele,UnRecord.Modele);

    rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sor ) ;
    if ( rc == -1 )
    die("SendDatagram:") ;
    else
    fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
 }
 
 
 close(Desc) ;
}
