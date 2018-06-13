#ifndef TERMINAL_H
#define TERMINAL_H


#define MAX 1024
#define PARAM 10
#define NBBASE 12
char Base[NBBASE][MAX]={"cp","rm","mv","cat","ln","echo","ls","mkdir","rmdir","cd","df","touch"};

char* supprimer_espace (char* cmd); //formater l'entré du terminal 
int verif_cmd (char par[][MAX],int indice);// verifier si la case indice est une commande dans la base ou pas
int sep_opt_arg (char par[][MAX],char opt[][MAX],char argu[][MAX],int nbparam); // separer les argument des options
int verif_redirection (char par[][MAX],int nbparam,char red[]);  //formatage des arguments en cas de redirection
/************* Les commandes qu'on va exécuter *************/
void my_touch(char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_cd (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_cp (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_rmdir (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_rm (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_mv (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_cat (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt,char red[]);
void my_ln (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_echo (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt,char red[]);
void my_ls (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_mkdir (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);
void my_df (char par [],char argu[][MAX],char opt[][MAX],int i,int nbopt);

/********************** Fin des commandes ******************/
void moteur_cmd (char par[],char argu[][MAX],char opt[][MAX],char red[],int i,int verifcm,int nbopt);//La ou seront exécutés toutes les commandes 
void analyse_cmd (char* cmd);






#endif