#include "LibSerHV.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}


int RechercheHV(char* NomFichier,int Reference ,struct VehiculeHV *UnRecord)
{
    FILE *sortie ;
    char Tampon[80] ;
    int  Numero ;
    int  nbr ;

    sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
    if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
    else
    fprintf(stderr,"Ouverture reussie \n") ;

    //AfficheEnteteVehiculeHV () ;
    nbr = fread(UnRecord,sizeof(struct VehiculeHV),1,sortie);
    while ( UnRecord->Reference!=Reference && !feof(sortie) )
    {
        nbr = fread(UnRecord,sizeof(struct VehiculeHV),1,sortie) ;
    }

    
    if(!feof(sortie))
    {
        fclose(sortie) ;
        return 1;
    }
    fclose(sortie) ;
    return 0;
}