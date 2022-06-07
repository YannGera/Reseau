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
#include <signal.h>


void die(char *s)
{
    perror(s);
    exit(1);
}

void handlerSIGSTP(int sig)
{
  printf("Pause de 30 secondes du serveur \n");
  sleep(30);
}


int main(int argc,char *argv[])
{
 int rc ;
 int Desc ;
 int exist = 0;
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 FILE* p;
 u_long  IpSocket ;
 u_short PortSocket ;
 
 int tm ;
 struct RequeteHV UneRequete ;
 struct FactureHV facture;

 signal(SIGTSTP, handlerSIGSTP);

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
    printf("Reference %d\n", UneRequete.Reference);

    /*FILE *LogSer;

    LogSer = fopen("LogSer","w");
    if ( LogSer == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
    fseek(LogSer, 0, SEEK_END ) ;
    fwrite(&UneRequete,sizeof(UneRequete),1,LogSer);
    fclose(LogSer);*/

    switch(UneRequete.Type)
    {
      case Question:
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
         break;

      case Achat:


         p = fopen("FactureHV","r");
         if(p == NULL)
         {
            printf("Erreur ouverture FactureHV\n");
            break;
         }

         while(!feof(p))
         {
            fread(&facture,sizeof(struct FactureHV),1,p);
            if(facture.DateFacturation == UneRequete.Date && strcmp(facture.NomClient, UneRequete.NomClient)==0)
            {
               exist = 1;
            }
         }
         fclose(p);
         if(exist == 0)
         {
            res = ReservationYG("VehiculesHV", UneRequete.Reference, UneRequete.Quantite);
            if(res==0)
            {
               FacturationYG("FactureHV", UneRequete.NomClient, UneRequete.Date, UneRequete.Quantite, UneRequete.Reference);
               UneRequete.Type = OK;
            }
            else
            {
               UneRequete.Type = Fail;
            }

            rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sor ) ;
            if ( rc == -1 )
               die("SendDatagram:") ;
            else
               fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
            break;
         }
         else
         {
            printf("Déjà existant \n");
            break;
         }
         
      default: printf("Erreur requete\n"); break;

    }


   rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sor ) ;
   if ( rc == -1 )
      die("SendDatagram:") ;
   else
     fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
    /* attention l'enum peut être codé en short */
    /* reponse avec psos */
    
 }
 
 
 close(Desc) ;
}

