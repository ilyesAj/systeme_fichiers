#ifndef BLOKCS_H
#define BLOKCS_H

#define MAX_FILE_NAME_LENGTH 50
#define MAX_FILE_NUM 10000   //Nombre maximal d'inodes
#define MAX_OPEN_FILES 10
#define BLOCK_MAP_START 1    // block_map coute 13 blocs
#define INODE_MAP_START 14   // inode_map coute 2 blocs
#define INODE_TABLE_START 16      //premier inode bloc
#define BLOCK_TABLE_START 2516    //inode table coute 2500 blocs 
#define BLOCKSIZE 1024

#define FILE_TYPE 1
#define DIR_TYPE 2

#define MAX_LEVEL 10     //nbre max de sous dossier

#define EMPTY_BLOCK 102399
int fd[MAX_OPEN_FILES];        // file description
char cwd_name[100];  
char cwd_pname[100];         // dossier courant
int cwd_inode;      
int cwd_pinode;      // inode du dossier courant 

// super block
struct super_block
{
	int magic_number;
	int block_num;          //nb total de blocs
	int inode_num;			//nb total d'inodes
	int free_blk;			//nb blocs vide
	int free_inode;			//nb inode vide
};

struct ind_block
{
	int blocks[256];  //bloc indirecte forme de table de bloc directe
};

// each inode has 256 Bytes
struct inode {
	int type;                       //1:fichier et 2:dossier 
	int num;                        //num inode
	int size;                        //taille fichier
	int uid;						//id utilisateur
	int gid;						//id group
	char mode[11];					//droits
	char name[MAX_FILE_NAME_LENGTH];   //nom fichier
	int blocks[10];                //bloc directe
	struct ind_block ind_blocks[30];            //bloc indirecte
	char unused[15];
};

//structure dossier et direntry expliqué au rapport
struct dirEntry
{
	int inode;          // inode dir entry 
	int type;           // type dir entry 
	int length;         // nom fichier
	char name[20];      // nom du sous dossier 
};


struct dir
{
	struct dirEntry dentry[32];   
};

//bitmap in block
struct bmap {
	unsigned char map[BLOCKSIZE];
};

void open_disk ();
int read_block(int number, void * block ) ;// plusieurs type de blocks (super block , inode block données ...)
int write_block(int number, void * block);
int write_inode(int number , void * inode);
int read_inode(int number , void * inode);
void format_disk ();
struct inode* find(const char* path);
void seek_block (int* i, int* j,int nbck);
void seek_inode (int* i, int* j,int nbck);
int get_block();
int get_inode() ;
int mycreat(const char* path);
void my_mkdir_dir(const char* path);
void my_close(const char* path);
int my_open(const char* path);
int my_read (int fdd, void * buf ,int nbtes);
int my_write (int fdd, char * buf ,int nbtes);
void my_rm_file(const char* path);
void my_rmdir_dir (const char* path);
int find_rep(const char* path,char arg[][MAX_FILE_NAME_LENGTH]);
void my_ls_dir (const char* path);


#endif
