#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX 1024
#define PARAM 10
#define NBBASE 11
char Base[NBBASE][MAX]={"cp","rm","mv","cat","ln","echo","ls","mkdir","rmdir","cd","df"}; // a definir dans un .h
// formatage et verification syntaxe commande
char* supprimer_espace (char* cmd) //formater l'entré du terminal 
{
	int i=0;
	int j=0;
	int k;
	int indicateur=0;
	char aux,aux2;
	if (cmd[0]==' ') // si la chaine commence par un ou des espaces (il faut enlever tout les espaces avant la commandes dans ce cas)
	{
		i++;
		while (cmd[i]!='\0' && i<MAX && cmd[i]==' ')
		{
			i++;
		}
		while (cmd[j]!='\0' && j<MAX)
		{
			cmd[j]=cmd[j+i];
			j++;
		}
	}
	i=0;
	j=0;
	while (cmd[i]!='\0' && i<MAX) // suppression des espaces de la chaine ( rendre les espaces uniques)
	{
		if (cmd[i]==' ' && cmd[i+1]==' ')
		{
			i++;
			j=i;
			k=i;
			while (cmd[i]!='\0' && i<MAX && cmd[i]==' ')
			{
				i++;
			}
			i=i-j;
			while (cmd[j]!='\0' && j<MAX)
			{
				cmd[j]=cmd[j+i];
				j++;
			}
			i=k;
			j=0;
		}
		
		i++;
	}
	i=1;
	j=0;
	
	while (cmd[i]!='\0' && i<MAX) // traitement des cas de redirection (rajouter des espaces si non existants)
	{
		if (cmd[i]=='>')
		{
			if (cmd[i+1]!=' ')// pas d'espace a droite de la redirection
			{
				j=i+1;
				aux=cmd[j];
				cmd[j]=' ';
				j++;
				while (j<MAX)
				{
					aux2=cmd[j];
					cmd[j]=aux;
					aux=aux2;
					j++;
				}
			}
		}
		i++;
	}
	i=1;
	k=0;
	while (cmd[i]!='\0' && i<MAX) // traitement des cas de redirection (rajouter des espaces si non existants)
	{
		if (cmd[i]=='>')
		{
			if (cmd[i-1]!=' ')// pas d'espace a gauche de la redirection
			{
				k=i;
				aux=cmd[k];
				cmd[k]=' ';
				k++;
				while (k<MAX)
				{
					aux2=cmd[k];
					cmd[k]=aux;
					aux=aux2;
					k++;
				}
			}
		}
		i++;
	}

	
	return cmd;
}
int verif_cmd (char par[][MAX],int indice)// verifier si la case indice est une commande dans la base ou pas
{
	int i,indicateur=-1;
	for (i=0;i<NBBASE;i++)
	{
		if (strcmp(par[indice],Base[i])==0)
		{
			printf("commande %s detecte en cour de construction ",par[indice] );
			indicateur=i;
		}

	}
	if (indicateur==-1)
	{
		printf("%s : commande introuvable ",par[indice]);
		return -1;
	}
	return indicateur;// commande en question
}

int sep_opt_arg (char par[][MAX],char opt[][MAX],char argu[][MAX],int nbparam) // separer les argument des options
{
	int i,indicateur_opt=-1,indicateur_arg=0;
	for (i=1;i<nbparam+1;i++)
	{
		if (par[i][0]=='-')
		{
			indicateur_opt++;
			strcpy(opt[indicateur_opt],par[i]);			
		}
		else
		{
			strcpy(argu[indicateur_arg],par[i]);
			indicateur_arg++;
		}
	} 
	return indicateur_opt; //nombre d'options
}
int verif_redirection (char par[][MAX],int nbparam,char red[])
{
	int i=1;
	for (i=1;i<(nbparam+1)-2;i++)
	{
		if (strcmp(par[i],">")==0) //redirection au milieu de la commande => erreur
		{
			printf("Erreur syntaxe \">\" \n");
			return -1; // redirection non valide
		}
	}
	if (strcmp(par[i],">")==0)
	{
		strcpy(red,par[i+1]);
		par[i][0]='\0';
		par[i+1][0]='\0';
		return i-1; // redirection valide renvoyer le nouveau nombre de parametres
	}
	return nbparam;
}
void moteur_cmd (char par[],char argu[][MAX],char opt[][MAX],char red[],int i,int verifcm)
{
	if (i!=-1 && verifcm!=-1)
	{
		printf("syntaxe valide commande prete a l'execution\n");
	}
	else
	{
		printf("syntaxe invalide\n");
	}
}

void analyse_cmd (char* cmd)
{
	int occ=-1;
	char par[PARAM][MAX];// nom de la commande 
	char opt[PARAM][MAX];
	char argu[PARAM][MAX];
	char aux[MAX];
	char red[MAX]; // fichier de redirection
	red[0]='\0';
	int i=0;
	int indicateur=0;
	int verifcm=0;
	int nbopt=0;
	char save_cmd[MAX];
	supprimer_espace(cmd); // elever les espaces inutiles
	strcpy(save_cmd,cmd);
	while (i<PARAM+1 && indicateur == 0)
	{	
		occ = strchr(cmd,' ') - cmd;// recherche d'espace 
		strcpy (aux,cmd);
		if (occ >= 1 && occ < MAX) // commande avec espace 
		{
			// extraction du premier mot de la commande
			aux[occ]='\0'; // isoler la partie avant l'espace
			strcpy(par[i],aux); // sauvegarder le i eme parametre
			aux[0]='\0'; //effacer aux pour la prochaine iteration 
			
			occ++;// incrementation occ afin de copier aprés l'espace 
			strncpy(cmd, cmd + occ, MAX - occ); // enlever la partie extraite de cmd
			occ=-1;
			i++;
		}
		else 
		{
			indicateur=1;
			strcpy(par[i],cmd);
		}
	}
	if (i==0)
	{
		strcpy (par[0],save_cmd);
	}
	verifcm=verif_cmd(par,0);
	i=verif_redirection(par,i,red);
	if (red[0]!='\0')
	{
		printf("redirection detecté vers le fichier : %s \n",red );
	}
	nbopt=sep_opt_arg(par,opt,argu,i);
	if (nbopt>-1)
	for (int j=0 ;j<nbopt+1;j++)
	{
		printf("opt %d : %s\n",j,opt[j]);
	}
	if (nbopt == -1)
	{
		for(int j=0;j<i;j++)
		{
			printf("arg %d : %s \n",j,argu[j]);
		}

	}
	else 
	{
		for(int j=0;j<(i-(nbopt+1));j++)
		{
			printf("arg %d : %s \n",j,argu[j]);
		}
	}
	moteur_cmd (par[0],argu,opt,red,i,verifcm);
	printf("\n[ISTY]:~$ "); //continuation console

}
// primitives

int main(int argc, char *argv[], char *envp[])
{
	
	char c = '\0';
	char tmp[MAX];
	tmp[0]='\0';
	printf("\n[ISTY]:~$ ");
	//Affichage du terminal
	while(c != EOF) {
		c = getchar();
		switch(c) {
			case '\n': // analyser et exécuter. 
				   printf("[ISTY]:~$ ");
				   analyse_cmd(tmp);
				   tmp[0]='\0';
				   break;
			default: strncat(tmp, &c, 1);
				 break;
		}
	}
	printf("\n");
	return 0;
}


