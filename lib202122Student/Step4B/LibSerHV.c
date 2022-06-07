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

int ReservationYG(char* NomFichier, int ref, int qtt)
{
    struct VehiculeHV UnRecord;
    printf("ref de base = %d\n",ref );
    FILE *ssortie;
    int res=0;
    int ret=0;

    ssortie = fopen(NomFichier,"r+") ;
    if ( ssortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0);
     ssortie = fopen(NomFichier,"w+");
    }
    else
        fprintf(stderr,"Ouverture reussie \n") ;

    fseek(ssortie,0,SEEK_SET);
    fread(&UnRecord, sizeof(struct VehiculeHV),1,ssortie);
    while(!feof(ssortie) && ret == 0)
    {
        if(ref == UnRecord.Reference)
        {
            printf("Reference trouvee ! : %d\n",ref );
            ret=1;
        }
        else
        {
            fread(&UnRecord,sizeof(struct VehiculeHV),1,ssortie);
        }
    }
    if(ret)
    {
        printf("Trouve pour ref %d : %s , %s , %d , %d\n", UnRecord.Reference, UnRecord.Marque, UnRecord.Modele, UnRecord.Quantite, UnRecord.Puissance );
        if(UnRecord.Quantite >= qtt)
        {
            UnRecord.Quantite = UnRecord.Quantite - qtt;
            printf("qtt : %d \n",UnRecord.Quantite );
            fseek(ssortie, -sizeof(struct VehiculeHV), SEEK_CUR);
            fwrite(&UnRecord, sizeof(struct VehiculeHV),1,ssortie);
        }
        else
        {
            fclose(ssortie);
            return -1;
        }
    }
    else
    {
        printf("Reference non trouvee \n");
        fclose(ssortie);
        return -1;
    }

    fclose(ssortie);
    return 0;

}

int FacturationYG(char NomFichier[80], char NomClient[60],int date,int Quantite,int Reference)
{
    struct FactureHV tmp;
    struct FactureHV tmp2;
    int nbFact=0;

    FILE* p;

    p = fopen(NomFichier, "r+");
    if(p == NULL)
    {
        printf("Erreur de fopen\n");
        p = fopen(NomFichier, "w+");
    }

    while(!feof(p))
    {
        fread(&tmp2,sizeof(struct FactureHV), 1, p);
        nbFact++;
    }

    fseek(p,0,SEEK_END);
    tmp.NumeroFacturation = nbFact+1;
    strcpy(tmp.NomClient, NomClient);
    tmp.Quantite = Quantite;
    tmp.Reference = Reference;
    tmp.DateFacturation = date;
    fwrite(&tmp,sizeof(struct FactureHV), 1, p);
    fclose(p);
}






















