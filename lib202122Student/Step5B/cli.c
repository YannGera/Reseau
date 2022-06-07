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
#include <signal.h>
#include <errno.h>

void die(char *s)
{
    perror(s);
    exit(1);
}
static void handlerSIGALRM(int sig) {}

int main(int argc, char *argv[])
{
 int rc, ret;
 int Desc ;
 int tm ; 
 int compteur = 0;
 unsigned int time_interval;
 
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
  int choix;
  int Ref;
  
  struct sigaction sigAct;
  sigemptyset(&sigAct.sa_mask);
  sigAct.sa_handler = handlerSIGALRM;
  sigAct.sa_flags = 0;
  sigaction(SIGALRM, &sigAct, NULL);

 while(1)
 {
   printf("--------------------\n");
   printf("1) Demander une référence\n");
   printf("2) Acheter un véhicule\n");
   printf("3) Quitter\n");
   printf("--------------------\n");
   printf("Choix : ");
   scanf("%d",&choix);
   switch(choix)
   {
      case 1:  printf("Référence : ");
               scanf("%d",&Ref);
               UneRequete.Type = Question;
               UneRequete.Reference = Ref;
               UneRequete.Numero = compteur;

               rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sos );
               if ( rc == -1 )
                  die("SendDatagram") ;
               else
                  fprintf(stderr,"Envoi de %d bytes\n",rc ) ;
               while(1)
               {
                  memset(&UneRequete,0,sizeof(struct RequeteHV)) ;
                  tm = sizeof(struct RequeteHV) ;
                  struct RequeteHV UneRequete1;
                  rc = ReceiveDatagram( Desc, &UneRequete1,tm, &sor ) ;
                  if(UneRequete1.Numero != compteur)
                  {
                     printf("Doublon!\n");
                  }
                  else
                  {
                     fprintf(stderr,"bytes recus:%d:%s\n",rc,UneRequete1.Marque ) ;
                     printf("<TypeRequete %d\n", UneRequete1.Type);
                     printf("Reference %d\n", UneRequete1.Reference);
                     printf("Reference %d\n", UneRequete1.Reference);
                     printf("Marque %s\n", UneRequete1.Marque);
                     printf("Modele %s\n", UneRequete1.Modele);
                     printf("Client %s\n", UneRequete1.NomClient);
                     break;
                  }
               }
               compteur++;
               break;

      case 2:
            UneRequete.Numero = compteur;
            UneRequete.Type = Achat;
            printf("NomClient : ");
            fflush(stdin);
            char client[80];
            scanf("%s", &client);
            printf("Reference : ");
            fflush(stdin);
            int ref;
            scanf("%d", &ref);
            printf("Quantite : ");
            fflush(stdin);
            int qte;
            scanf("%d", &qte);


            redo:
            UneRequete.Type = Achat;
            UneRequete.Numero = compteur;
            strcpy(UneRequete.NomClient,client);
            UneRequete.Reference = ref;
            UneRequete.Quantite = qte;
            time_interval = 9;
            ret = alarm(time_interval);
            
            rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sos ) ;

            if ( rc == -1 )
               die("SendDatagram");
            else
              fprintf(stderr,"Envoi de %d bytes\n",rc );

            while(1)
            {
               memset(&UneRequete,0,sizeof(struct RequeteHV));
               tm = sizeof(struct RequeteHV);

               rc = ReceiveDatagram( Desc, &UneRequete,tm, &sor );
               if ( rc <= 0 )
               {
                  goto redo;
                  //die("ReceiveDatagram") ;
               }
               if(UneRequete.Numero != compteur)
               {
                  printf("C'est un doublon");
               }
               else
               {
                  alarm(0);
                  if(UneRequete.Type == OK)
                  {
                     printf("Achat réussi\n");
                     break;
                  }
                    
                  else
                  {
                     printf("echec\n");
                     break;
                  }
                    
               }
                 
            }
            compteur++;
            //close(Desc);
            break;



      case 3:  close(Desc);
               exit(0);
               break;
      default: printf("Choix invalide\n");
   }


   // rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteHV) ,&sos ) ;

   // if ( rc == -1 )
   //  die("SendDatagram") ;
   // else
   // fprintf(stderr,"Envoi de %d bytes\n",rc ) ;

   // memset(&UneRequete,0,sizeof(struct RequeteHV)) ;
   // tm = sizeof(struct RequeteHV) ;
   // struct RequeteHV UneRequete1;
   // rc = ReceiveDatagram( Desc, &UneRequete1,tm, &sor ) ;
   // if ( rc == -1 )
   //  die("ReceiveDatagram") ;
   // else
   // fprintf(stderr,"bytes recus:%d:%s\n",rc,UneRequete1.Marque ) ;
   // printf("<TypeRequete %d\n", UneRequete1.Type);
   // printf("Reference %d\n", UneRequete1.Reference);
   // printf("NumeroFacture %d\n", UneRequete1.NumeroFacture);
   // printf("Date %d\n", UneRequete1.Date);
   // printf("Reference %d\n", UneRequete1.Reference);
   // printf("Prix %d\n", UneRequete1.Prix);
   // printf("Marque %s\n", UneRequete1.Marque);
   // printf("Modele %s\n", UneRequete1.Modele);
   // printf("Client %s\n", UneRequete1.NomClient);
 }
 
 close(Desc) ;
}