
// enum TypeRequete {
//        Question = 1 ,
//        Reponse  = 2 
//      } ;

// struct Requete
// {
//   enum TypeRequete Type ;
//   char Message[40] ; 
// } ; 

#include <stdio.h>


enum TypeRequete {
     Question = 1 ,
     Achat = 2 ,
     Livraison= 3 ,
     OK = 4,
     Fail = 5
 };
struct RequeteHV
{
     enum TypeRequete Type ;
     int Numero ; // Contient le numéro de la requete
     int NumeroFacture ;
     int Date ;
     int Reference ; // la référence du film
     int Quantite ; ;
     int Prix ;
     char Marque[30] ;
     char Modele[30] ;
     char NomClient[80] ;
} ;

void AfficheRequeteHV(FILE *fp, struct RequeteHV R )
{
    fprintf(fp,">TypeRequete %d \n", R.Type ) ;
    fprintf(fp," Reference %d \n", R.Numero ) ;
    fprintf(fp," NumeroFacture %d \n", R.NumeroFacture ) ;
    fprintf(fp," Date %d \n", R.Date ) ;
    fprintf(fp," Reference %d \n", R.Reference ) ;
    fprintf(fp," Places %d \n", R.Quantite ) ;
    fprintf(fp," Prix %d \n", R.Prix ) ;
    fprintf(fp,"Marque %s \n", R.Marque ) ;
    fprintf(fp,"Modele %s \n", R.Modele ) ;
    fprintf(fp," Client %s \n\n", R.NomClient ) ;
    return ;
}