#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "blocks.h"
#include "terminal.h"
 // a definir dans un .h
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
			//printf("commande %s detecte en cour de construction ",par[indice] );
			indicateur=i;
		}

	}
	if (indicateur==-1)
	{
		printf("%s : commande introuvable \n",par[indice]);
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
//Les commandes 

void my_touch(char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//une création simple de fichier sans options 
	int err=0 ;
	int k ;
	int descriptor;
	char file[20]="";
	if (nbopt>-1)
		err=-1 ;
	if(err!=-1)
	for (k=0;k<=i;k++)
	{
		if (argu[0][0]!='/')
		{	
			strcpy(file,cwd_name) ;
			strcat(file,argu[k]);
			descriptor=mycreat(file);
		}
		else
		{
			descriptor=mycreat(argu[k]);	
		}

	}
}

void my_pwd (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//pwd ne prend aucun argument
	if (nbopt>-1)
		printf("pwd ne prend aucune option");
	else
		printf("%s ",cwd_name);

}

void my_cd (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	// Cd ne posséde que deux options -L et -P 
	//Elle ne peut avoir qu'un seul argument 
	// Synopsis cd [-L|-P] [dir]
	/* ***************************
	
	**************************** */ 
	char cheminp[MAX_FILE_NAME_LENGTH*MAX_LEVEL]="/";
	char arg[MAX_LEVEL][MAX_FILE_NAME_LENGTH];
	char file [20] = "";
	int j,u ;
			
	struct inode *cur = (struct inode *) malloc(sizeof(struct inode));
	int err=0; 
	if (i >= 3 )
		{
			printf("Syntaxe de la commande cd invalide ! cd [-L|-P] [dir] \n");
			err=-1;	
		} 
	if (nbopt>-1 )
		if (strcmp(opt[0],"-L")!=0 && strcmp(opt[0],"-P")!=0)
			{
				printf(" Options invalides ! cd [-L|-P] [dir]  \n" );
				err=-1;
			}
	if (err!=-1)
	{	
		if (strcmp(argu[0],"..")==0)
		{
			if (strcmp(cwd_name,"/")!=0)
			{
				j=find_rep(cwd_name,arg);
				for (u=0;u<j-1;u++)
					{
						strcat(cheminp,arg[u]);
						strcat(cheminp,"/");
					}
				printf("%s \n",cheminp);
				strcpy(cwd_name,cheminp);
				cur=find(cheminp);
				cwd_inode=cur->num ;
			}
		}
		else 
		if (strcmp(argu[0],"")==0 || i ==0)
		{
			//On revient au repertoire initiale
			cur=find("/");
			cwd_inode=cur->num ;
			strcpy(cwd_name,cur->name);
		} 
		else
		{
			if (argu[0][0]!='/')
			{
				strcat(file,cwd_name);
				strcat(file,argu[0]);
				if(find(file)==NULL)
					printf("Chemin introuvable");
				else
				{
					cur=find(file);
					cwd_inode=cur->num ;
					strcpy(cwd_name,file) ;		
				}	
			}
			else
			{
				if(find(argu[0])==NULL)
					printf("Chemin introuvable");
				else
				{
					cur=find(argu[0]);
					cwd_inode=cur->num ;
					strcpy(cwd_name,argu[0]) ;
				}	
			}
		}
	}
}
void my_cp (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	// pour le cp on va juste implémenter les options 
	//-f pour effacer les fichiers/dossier destinations
	//-i interoger l'utilisateur avant la copie
	int err=0; int k ; 
	char choix='\0' ;
	char file [20];
	char cr[20];
	int descriptor ;

	if(nbopt>-1) //exemple cp -i fich1 fich2 on doit voir c'est quoi l'option
	for (k=0;k<=nbopt;k++)
	{
			if (strcmp(opt[k],"-f")==0 )
				{
					//Effacer les fichiers cibles
					//Envisager l'ajout d'une variable bool pour traiter aprés
				}
			else if (strcmp(opt[k],"-i")==0 )
					{
						while (choix != 'Y' && choix != 'y' && choix != 'N' && choix != 'n')
						{
							printf("Voulez vous vraiment écraser le(s) fichier(s) destinations ? Y/N \n" );
							scanf("%c",&choix) ;
						}
					}
			else 
				err=-1 ; //En cas ou d'autres options non valide fourni 
	}
	if (err!=-1)
	{
		if (choix =='Y' || choix== 'y' || choix =='\0')
		{
			if (argu[0][0]!='/')
			{
				strcpy(file,cwd_name);
				strcat(file,argu[0]);
				if (find(file)==NULL)
					printf("Fichier inexistant ");
				else 
				{
					descriptor=mycreat(file);
					my_read(descriptor,cr,20) ; //Lecture depuis le premier fichier
					strcpy(file,cwd_name);
					strcat(file,argu[1]);
					descriptor=mycreat(file);
					my_write(descriptor,cr,20);//Insertion du contenu du premier fichier vers le deuxiéme

				}
			}
			else
			{
				if (find(argu[0])==NULL)
					printf("Fichier inexistant ");
				else 
				{
					descriptor=mycreat(argu[0]);
					my_read(descriptor,cr,20) ; //Lecture depuis le premier fichier
					descriptor=mycreat(argu[1]);
					my_write(descriptor,cr,20);//Insertion du contenu du premier fichier vers le deuxiéme

				}
			}
		}
	}
}

void my_rmdir (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	int err=0; int k ; 
	char choix ='\0' ;
	char file [20] ;

	if(nbopt>-1) 
	
		for (k=0;k<=nbopt;k++)
		{
			if (strcmp(opt[k],"-f")==0 )
				{
					
				}
			else if (strcmp(opt[k],"-i")==0 )
					{
						while (choix != 'Y' && choix != 'y' && choix != 'N' && choix != 'n')
						{
							printf("Voulez vous vraiment supprimer le(s) dossier(s) destinations ? Y/N \n" );
							scanf("%c",&choix) ;
						}
					}
			else 
				err=-1 ; // En cas ou d'autres options non valide fourni 
		}
	if (err!=-1)
	{
		if (choix =='Y' || choix =='y' || choix =='\0')
		{
			strcpy(file,cwd_name);
			strcat(file,argu[0]);
			//strcat(file,"/");
			if (find(file)!=NULL)
				my_rmdir_dir(file);
			else 
				printf("Dossier inexistant\n");
		}
	}
}

void my_rm (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//rm ne supprime que les fichiers par les repertoires
	//en cas ou l'option -d fourni on renvoie sur rmdir 
	//rm ne peut avoir que 3 options -d -f -i 
	int err=0; int k ; // K a remplacer avec nbopt
	char choix ='\0' ;
	char file [20] ;
	if(nbopt>-1) //exemple rm -i -f fich1  on doit voir c'est quoi l'option
	for (k=0;k<=nbopt;k++)
	{
			if (strcmp(opt[k],"-f")==0 )
				{
					//Si le fichier n'existe pas ignorer !
					//Envisager l'ajout d'une variable bool pour traiter aprés
				}
			else if (strcmp(opt[k],"-i")==0 )
					{
						while (choix != 'Y' && choix != 'y' && choix != 'N' && choix != 'n')
						{
							printf("Voulez vous vraiment supprimer le(s) fichier(s) destinations ? Y/N \n" );
							scanf("%c",&choix) ;
						}
						
					}
			else if (strcmp(opt[k],"-d")==0 )
					{
						my_rmdir(par,argu,opt,i,-1); // On renvoie vers rmdir 
					}
			
			else 
				err=-1 ; // En cas ou d'autres options non valide fourni 
	}
	if (err!=-1)
	{
		if (choix =='Y' || choix =='y' || choix=='\0')
		{
			if (argu[0][0]!='/')
			{
				strcpy(file,cwd_name);
				strcat(file,argu[0]);
				if (find(file)==NULL)
					printf("Fichier non trouvé \n");
				else
					my_rm_file(file) ;
			}
			else
			{
				if (find(argu[0])==NULL)
					printf("Fichier non trouvé \n");
				else
					my_rm_file(argu[0]) ;	
			}
			
		}
	}
}

void my_mv (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	int err=0; int k ; 
	char choix ='\0' ;
	char file [20] ;
	char file2 [20] ;
	int descriptor=-1 ;
	char cr[20];
	if(nbopt>-1) 
	for (k=0;k<=nbopt;k++)
	{
			if (strcmp(opt[k],"-f")==0 )
				{
					//Forcer l'ecrasement des fichiers destinations sans confirmation
					//Envisager l'ajout d'une variable bool pour traiter aprés
				}
			else if (strcmp(opt[k],"-i")==0 )
					{
						while (choix != 'Y' && choix != 'y' && choix != 'N' && choix != 'n')
						{
							printf("Voulez vous vraiment depalcer le(s) fichier(s) ? Y/N \n" );
							scanf("%c",&choix) ;
						}
					}
			else 
				err=-1 ; // En cas ou d'autres options non valide fourni 
	}
	if (err!=-1)
	{
		if (choix =='Y' || choix== 'y' || choix =='\0')
		{
			strcpy(file,"/");
			strcat(file,argu[0]);
			if (find(file)==NULL)
				printf("Fichier inexistant \n");
			else 
			{
				descriptor=mycreat(file);
				my_read(descriptor,cr,20) ; //Lecture depuis le premier fichier
				my_rm_file(file); //suppression du premier fichier
				strcpy(file,"/");
				strcat(file,argu[1]);
				descriptor=mycreat(file);
				my_write(descriptor,cr,20);//Insertion du contenu du premier fichier vers le deuxiéme

			}
		}
	}
}
	
void my_cat (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt,char red[])
{
	// cat dans notre cas n'aura qu'une seule option qui est -n pour numéroter les lignes
	int err=0; int k ;
	int descriptor ;
	char file [20] ="";
	char file2 [20]="" ;
	
	char cr [20] ;

	if(nbopt>-1) 
	for (k=0;k<=nbopt;k++)
	{
			if (strcmp(opt[k],"-n")==0 )
				{
					//Numéroter les lignes à la sortie standard ou à la redirection
				}
			else 
				err=-1 ; // En cas ou d'autres options non valide fourni 
	} 
	if (err!=-1)
	{
		if (strcmp(red,""))
		{
			//en cas de redirection on cree le nouveau fichier et on lit le contenu
			//du premier fichier et le renvoyer vers le deuxieme en changeant de descripteur
			if (argu[0][0]!='/')
			{	
				strcpy(file,cwd_name);
				strcat(file,argu[0]);
				strcpy(file2,cwd_name);
				strcat(file2,red);
				descriptor=mycreat(file);
				my_read(descriptor,cr,20) ;
				descriptor=mycreat(file2);
				my_write(descriptor,cr,20);
			}
			else
			{
				descriptor=mycreat(argu[0]);
				my_read(descriptor,cr,20) ;
				descriptor=mycreat(red);
				my_write(descriptor,cr,20);	
			}
		}
		else
		{
			if (argu[0][0]!='/')
			{	
				strcpy(file,cwd_name);
				strcat(file,argu[0]);
				if (find(file)==NULL)
					printf("Fichier inexistant \n");
				else 
				{
					descriptor=mycreat(file);
					my_read(descriptor,cr,20) ;
					printf("%s ",cr);
						//Creation du fichier s'il n'existe pas 
				}		//Affecter le contenu de argu[k] dans le fichier 
			}
			else
			{
				if (find(argu[0])==NULL)
					printf("Fichier inexistant \n");
				else 
				{
					descriptor=mycreat(argu[0]);
					my_read(descriptor,cr,20) ;
					printf("%s ",cr);
						//Creation du fichier s'il n'existe pas 
				}	
			}
		}

	}
}

void my_ln (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//Creation de lien entre fichiers
	//find prend l'inode Est ce que repertoire ou non si repertoire recuperer dentry 


}

void my_echo (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt,char red[])
{
	//echo ne va pas prendre d'options 
	struct inode *cur ;
	int err=0 ;
	int k ;
	int descriptor ;
	char file [20] ="" ;
	char rc2 [20]="" ;
	if (nbopt>-1) err=-1 ;
	if (err!=-1)
	{	
		if (strcmp(red,""))
		{
			if (argu[0][0]!='/')
			{
				strcpy(file,cwd_name);
				strcat(file,red);
				descriptor=mycreat(file);
				my_write(descriptor,argu[0],20);
				//Creation du fichier s'il n'existe pas 
				//Affecter le contenu de argu[k] dans le fichier 
			}
			else
			{
				descriptor=mycreat(red);
				my_write(descriptor,argu[0],20);			
			}
		}
		else 
			for (k=0;k<=i;k++) 
				printf("%s ",argu[k]); //On va juste afficher ce qui se trouve apres echo 
	}
}

void my_ls (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//On va prendre que l'option -l 
	int err=0; int k ; 
	int l=-1;
	char type[2];
	char file [20]="";
	struct inode *cur = (struct inode *) malloc(sizeof(struct inode));

	if(nbopt>-1) 
	for (k=0;k<=nbopt;k++)
	{
			if (strcmp(opt[k],"-l")==0 )
				{
					l=1;
					//Affichage des dossiers qu'on a avec les droits d'accés argu[0] ..  
				}
			else 
				err=-1 ; // En cas ou d'autres options non valide fourni  
	}
	if (err!=-1)
	{
		if (l!=-1)
		{
			printf("\n"); 
			if (find(argu[0])!=NULL)
			{	
				if (strcmp(argu[0],"")==0)
					my_ls_dir(cwd_name);

				if(argu[0][strlen(argu[0]-1)]=='/')
				{
					if (argu[0][0]!='/')
					{
						strcat(file,cwd_name);
						strcat(file,argu[0]);
						my_ls_dir(file);	
					}
					else
					{
						my_ls_dir(argu[0]);	
					}	
					
				}
				else
				{	
					if(find(argu[0])==NULL)
						printf("Fichier inexistant");
					else
					{
						cur=find(argu[0]);
						if(cur->type==1) 
							strcpy(type,"-");
						else 
							strcpy(type,"d"); 
						printf("%s%s root root %d %s",type,cur->mode,cur->size,cur->name);
					}
				}
			}
		}
		//parcourir les dossiers avec argu[0] .. et les supprimer 
	}
}

void my_mkdir (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//mkdir ne va pas contenir des options
	int err=0; int k ; 
	char file [20] ;
	if(nbopt>-1) err=-1 ;
	if (err!=-1)
	{
		if (argu[0][0]!='/')
			{
				strcpy(file,cwd_name);
				strcat(file,argu[0]);
				my_mkdir_dir(file);
			}
		else
			my_mkdir_dir(argu[0]);
		
	}
}



void my_df (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt)
{
	//TODO structure super block read en 0 free indoe free //block*1024(espace dispo) 
	//struct super_block *sbim = (struct super_block *)malloc(sizeof(struct super_block));  
	struct super_block *sbim = (struct super_block *) calloc(BLOCKSIZE, 1); 
	read_block(0,sbim);
	printf("Espace disponible : %d \n",sbim->free_blk * 1024 );
	printf("Nombre de block disponible %d \n",sbim->free_blk) ;
	printf("Nombre d'inode disponible : %d \n",sbim->free_inode) ;
	
}

//FIN commande	
	


void moteur_cmd (char par[],char argu[][MAX],char opt[][MAX],char red[],int i,int verifcm,int nbopt)
{

	//{"cp","rm","mv","cat","ln","echo","ls","mkdir","rmdir","cd","df"}; // a definir dans un .h

	if (i!=-1 && verifcm!=-1)
	{
		//printf("syntaxe valide commande prete a l'execution\n");
		if (strcmp(par,"touch")==0)
		{
			my_touch(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"pwd")==0)
		{
			printf("[ISTY]:~$ ");
			my_pwd(par,argu,opt,i,nbopt);
			printf("\n");	
	
		}
		if (strcmp(par,"cd")==0)
		{
			my_cd(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"cp")==0)
		{
			my_cp(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"rm")==0)
		{
			my_rm(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"mv")==0)
		{
			my_mv(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"cat")==0)
		{
			if (strcmp(red,"")==0)
				printf("[ISTY]:~$ ");
			my_cat(par,argu,opt,i,nbopt,red);
			if (strcmp(red,"")==0)
				printf("\n");	

		}
		if (strcmp(par,"ln")==0)
		{
			my_ln(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"echo")==0)
		{
			if (strcmp(red,"")==0)
				printf("[ISTY]:~$ ");
			my_echo(par,argu,opt,i,nbopt,red);
			if (strcmp(red,"")==0) 
				printf("\n");	
		}
		if (strcmp(par,"ls")==0)
		{
			printf("[ISTY]:~$ ");
			my_ls(par,argu,opt,i,nbopt);
			printf("\n");	
		}
		if (strcmp(par,"mkdir")==0)
		{
			my_mkdir(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"rmdir")==0)
		{
			my_rmdir(par,argu,opt,i,nbopt);	
		}
		if (strcmp(par,"df")==0)
		{
			printf("[ISTY]:~$ ");
			my_df(par,argu,opt,i,nbopt);	
		}
		
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
	/*if (red[0]!='\0')
	{
		printf("redirection detecté vers le fichier : %s \n",red );
	}*/
	nbopt=sep_opt_arg(par,opt,argu,i);
	/*if (nbopt>-1)
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
	}*/
	moteur_cmd (par[0],argu,opt,red,i,verifcm,nbopt);
	//printf("[ISTY]:~$ "); //continuation console

}

// primitives

int main(int argc, char *argv[], char *envp[])
{
	format_disk();
	struct inode *cur = (struct inode *) malloc(sizeof(struct inode));
	char c = '\0';
	char tmp[MAX];
	tmp[0]='\0';
	int k;
	cur=find("/");
	cwd_inode=cur->num;
	strcpy(cwd_name,"/");
	
	printf("\n[ISTY]:~$ ");
	//Affichage du terminal
	while(c != EOF) {
		c = getchar();
		switch(c) {
			case '\n': // analyser et exécuter. 
				   //printf("[ISTY]:~$ ");
				   if (tmp[0]!='\0')
				   	{
				   		analyse_cmd(tmp);
						printf("[ISTY]:~$ ");
						tmp[0]='\0';
				   		break;
					}
					else
						{
							printf("[ISTY]:~$ ");
							tmp[0]='\0';
				   			break;
				   		}
				   
			default: strncat(tmp, &c, 1);
				 break;
		}
	}
	printf("\n");
	return 0;
}


