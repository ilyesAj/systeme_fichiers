#include "blocks.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int block_number=-1;
int f=-1; //fichier 
void open_disk ()
{
	struct stat st;
	
	
	f = open ("./vdisk", O_RDWR);
	fstat (f, &st);
	block_number = st.st_size / 1024;
	printf(" ouverture du disque, nombre de block  : %d\n",block_number);
	

}

int read_block(int number, void * block ) // plusieur type de blocks (super block , inode block données ...)
{
	if (number < 0 &&  number >= block_number)
	{
		printf ("debordement du nombre de block");
		return -1;
	}
	lseek(f,number * BLOCKSIZE ,SEEK_SET); // lseek pointe sur un point dans un fichier 
	// SEEK_SET ensemble de recherche dans le fichier 
	read(f,block,BLOCKSIZE); // lecture 
	return 0;	
}
int write_block(int number, void * block)
{
	if (number < 0 &&  number >= block_number)
	{
		printf ("debordement du nombre de block");
		return -1;
	}
	lseek(f,number * BLOCKSIZE ,SEEK_SET);
	write(f,block,BLOCKSIZE); // ecriture 
	fsync(f); // synchonisation du fihchier ( disque dur)
	return 0;
}
int write_inode(int number , void * inode)
{
	if (number == 0 ||( number > INODE_TABLE_START && number >= block_number ) )
	{
		printf ("debordement du nombre d'inode");
		return -1;
	}
	lseek(f, INODE_TABLE_START * BLOCKSIZE + number * 256 ,SEEK_SET); // taille inode 256 bytes (bytes en ENG)
	write(f,inode,256);
	fsync(f);
	return 0;
}
int read_inode(int number , void * inode)
{
	if (number == 0 ||( number > INODE_TABLE_START && number >= block_number ) )
	{
		printf ("debordement du nombre d'inode");
		return -1;
	}
	lseek(f,INODE_TABLE_START * BLOCKSIZE + number * 256,SEEK_SET);
	read(f,inode,256);
	return 0;
}
void format_disk ()
{
	open_disk(); // ouverture du disque dur virtuel
	struct super_block *sb = (struct super_block *) calloc(BLOCKSIZE, 1); // allocation du superblock
	char* test = (char*) calloc (BLOCKSIZE,1); // bloc a lire
	struct inode *root = (struct inode *) malloc(sizeof(struct inode)); // allocation inode root
	struct dir *root_block = (struct dir *) malloc(sizeof(struct dir)); // allocation bloque de root repertoire
	int i; // compteur 
	read_block(0,test);
	if (* (int *) test == 2012) // test si le disque est deja formaté 
	{
		printf("disque dur deja formaté \n");
	}
	else
	{	
		sb->magic_number = 2012;       //ecrire le nombre magique 
		// ce nombre magique caracterise notre disque dur
		sb->block_num = block_number; // contient le nombre de block total dans le disque
		sb->inode_num = MAX_FILE_NUM;
		sb->free_blk = 99883;    // total is 99884, minus 1 for root dir block
		sb->free_inode = 2499;   // total is 2500, minus 1 for root dir inode
		write_block(0, sb); // ecriture du superblock
		// superblock ecrit [X]
		for (i = 0 ; i < MAX_OPEN_FILES ; i++)
		{
			fd[i] = -1;
		}
		// initialisation des descripteurs
		*test = 0x80 ; // @ de depart des blocks 
		write_block(BLOCK_MAP_START,test);
		*test = 0xc0; // saut du premier inode puis @ de depart des inodes
		write_block(INODE_MAP_START,test);
		// carte des inodes et bloque initialisé 
		root->type = DIR_TYPE; // type dossier
		root->num = 1;                // le deuxieme inode est l'inode root,a mettre dans le premier bloque de données (bloque 16 dans le disque,le nombre d'inode 1
		root->size = 1;               // 1 KB  pour bloque
		root->uid = 0; // id user 
		root->gid = 0; // id groupe
		strcpy(root->mode, "rwxr-xr-x"); // droit du repertoire root
		strcpy(root->name, "/"); // nom du repertoire root 
		root->blocks[0] = BLOCK_TABLE_START;     // ou est stocké le repertoire root dans le disque 
		for(i = 1; i < 10; i++)
			root->blocks[i] = -1;    // les autres bloques sont a null on pas depassé qté de données d'un bloque de données
		for(i = 0; i < 30; i++)
			root->ind_blocks[i] = -1; // meme pour les bloque indirecte puisque les autres bloques son vides
		write_inode(1, root);
		printf("inode root ecrit \n");
		// inode root ecrit [X]
		// dentry soit disant la colle entre les repertoires et fichier du disque
		// definition vers quel inodes pointes les chemins
		root_block->dentry[0].inode = 1; // inode du root 
		root_block->dentry[0].type = DIR_TYPE; // type deja definit
		root_block->dentry[0].length = 32; // taille 
		strcpy(root_block->dentry[0].name, "."); // . est le chemin courant du fichier ( lui meme)

		root_block->dentry[1].inode = 1;
		root_block->dentry[1].type = DIR_TYPE;
		root_block->dentry[1].length = 32;
		strcpy(root_block->dentry[1].name, "..");  // .. chemin des parent dans le cas du root (lui meme)
		write_block(BLOCK_TABLE_START, root_block); // ecriture du bloque de root 
		printf("bloque données root ecrit \n");
		free(test);
		free(sb);
		free(root);
		free(root_block);
		printf("disque dur formaté ,systeme de fichier initialisé !\n");

	}
}

int main(int argc, char const *argv[])
{
	format_disk();

	close(f);
	return 0;
}