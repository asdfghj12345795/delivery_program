#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define N_BUILDING				10
#define N_FLOOR					10
#define N_ROOM					10

#define MAX_MSG_SIZE			100
#define PASSWD_LEN				4

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
		//Initialize corresponding memory values using NULL to initialize one storage box by susing NULL.
		// date of byte can be removed by 0
		deliverySystem[x][y].building = 0; 
		deliverySystem[x][y].room = 0;		
		deliverySystem[x][y].cnt = 0;
		
		//Initialization of string is achieved using '\0'
		deliverySystem[x][y].passwd[PASSWD_LEN+1] = '\0';
		deliverySystem[x][y].context = '\0';		
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
		
		char try_passwd[PASSWD_LEN+1]; // password that the user typed
		
		// Enter Password for only user.
		printf("Please enter your password.\n"); 
		scanf("%s", &try_passwd);
		
	
		 //Compare the entered password with the specific locker password
		 //strcmp(strind1,string2) ~ compare the string 1 and string 2
		 //strcmp = if the two of strings are same, print 0
		 //			if not, don't print 0
		if (strcmp(deliverySystem[x][y].passwd,try_passwd)!=0) 
		{
			return -1;	// failed 		
		 }
		 
		// if the try_passwd is masterpassword, the storage should open
		//strcmp = if the two of strings are same, print 0
		//			if not, don't print 0
		else if(strcmp(masterPassword[PASSWD_LEN+1],try_passwd[PASSWD_LEN+1])==0)
		{
			return 0; //success
		}

		//the situation, tyr_passwd and passwd are the same
		else
		{	
			return 0; // success
					
		  }  
		
	
}


// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
	FILE* fp;
	int i_row, j_column;
	
	// open the entered file as 'fopen'
	// copy the contents of the delivery system as a function of "w"(function of write) to open files
	fp =fopen(filepath,"w");
	
	// if (fp=NULL), Fail to open
	if(fp==NULL)
	{
		printf("Can't work well for open the file.\n");
		return -1;
	}
	
	//Enter current storage() status in txt file 
	//Repeat to verify that content is ingested throughout the storage.	
	//systemSize[0] : row , systemSize[1] : column 
	
	//repeat for 'row'
	for(i_row=0;i_row<systemSize[0];i_row++)
	{
		//repeat for 'column'
		for(j_column=0;j_column<systemSize[1];j_column++)
		{
			//If deliverySystem.cnt is greater than 0, input the value stored deliverySystem in the filepath
			if(deliverySystem[i_row][j_column].cnt>0)
			{
				//print the storage system status and setting value 
				// order of saving >>> row, column, building, room, password, context
				fprintf(fp,"%d %d %d %d %s %s", i_row, j_column, deliverySystem[i_row][j_column].building,
												deliverySystem[i_row][j_column].room, deliverySystem[i_row][j_column].passwd,
												deliverySystem[i_row][j_column].context);
			}
		}
	}	
	
	fclose(fp);
	
	return 0;
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	// i = row / j = column
	int i,j;  // to use the circular 
	int inputrow,inputcolumn; // accordinate the package location
	FILE* fp;
	char c;
	char context[1000];
	int number_str; //Variables that store the length of a string in context
		
	
	//file open 
	fp = fopen(filepath,"r");
	
	//systemSize[0]  : row, systemSize[1] : column
	//read row, column number and masterpassword from filepath
	// masterpassword create by the storage.txt
	fscanf(fp, "%d %d %s", &systemSize[0], &systemSize[1], masterPassword);
	
	//allocate memory using dynamic memory allocation to deluverySytem structure
	
	deliverySystem = (storage_t**)malloc(systemSize[1]*sizeof(storage_t*));
		for(i=0;i<systemSize[1];i++)
		{
			deliverySystem[i]=(storage_t*)malloc(systemSize[0]* sizeof(storage_t));		
			
			if(deliverySystem[i]==NULL)
			{
				printf("Dynamic memory allocation error");
			
				//failed to creat te system.
				return -1;
			}
	
		}

	// i = row / j = column
	// use 'if' to confirm allocated memory is NULL
	// this member variable have to 0 in the first time
	for(i=0;i<systemSize[0];i++)
		{
			for(j=0;j<systemSize[1];j++)
			{
				deliverySystem[i][j].cnt = 0;
			}
		}
	

	// put the package accroding to storage.txt file (first of all have to read it)
	while( (c =fgetc(fp))!= EOF)
	{
		// put the package accroding to storage.txt file (first of all have to read it)
		//read the file text in order of building number, room number, password, context,
		//The first string is received by the context variable.
		fscanf(fp, "%d %d %s %s", &deliverySystem[inputrow][inputcolumn].building, &deliverySystem[inputrow][inputcolumn].room,
								  deliverySystem[inputrow][inputcolumn].passwd, context);
		
		//To figure out what is the path of context
		
		number_str = strlen(context);
		
		//Allocate as many memory as the number of courier strings at the context of aray
		//Set context memory according to the size of the string.
		//context is defined char and have array.
		deliverySystem[inputrow][inputcolumn].context= (char*)malloc(number_str* sizeof(char));
	 
		//Save text string for deliverySystem.context // use 'strcpy'
		strcpy(deliverySystem[inputrow][inputcolumn].context, context);
		
		// and add 1 to the package cnt(count in the cell) 
		deliverySystem[inputrow][inputcolumn].cnt++;
	}
	
	fclose(fp);
	//successfully create.
	return 0;
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	
	// clear memory of deliverySystem allocated to dynamic memory
	// free (pointor for dynamic memory)
	//systemSize[1] is the column 
	// delete the memory of dynamic
	for(i=0;i<systemSize[1];i++)
		free(deliverySystem[i]);
	free(deliverySystem);

	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %d\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%d|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%d,%d\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char passwd[PASSWD_LEN+1], char msg[MAX_MSG_SIZE+1]) {
		
		deliverySystem[x][y].building = nBuilding;
		deliverySystem[x][y].room = nRoom ;
		
		//Use the 'strcat' to insert the delivery string.
		//strcat = (target, Source); >>> Save source string to target string
		strcat(deliverySystem[x][y].passwd,passwd);
		strcat(deliverySystem[x][y].context,msg);
		
		deliverySystem[x][y].cnt ++;
		storedCnt++;
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
		//Check the password using the inputPasswd (int x, int y) function
		inputPasswd(x,y);
		
		//Only when the correct password has been entered through the function,
		// the extraStorage function below will function (because it is return 0 only in that case)
		//Therefore, have to do is take out the contents of the storage.	
	
		printf("THE context of package is %s", deliverySystem[x][y].context); 
		
		// after extract the context, initialize the storage (x,y)
		initStorage(x,y); 
	
	return 0;
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	int cor_column; //coordinate of column
	int cor_row; //coordinate of row
	 
	//Check from all structured arrangement to the last parcel shelf
	
	// the imformation of row is stored in systemSize[0]
	for(cor_row=0;cor_row<systemSize[0];cor_row++)
	{
		// the imformation of column is stored in systemSize[1]
		for(cor_column=0;cor_column<systemSize[1];cor_column++)
		{
	 		//Determine if the number of the building entered and the room number match the building number and the room number of the structure. 
			if(deliverySystem[cor_row][cor_column].building == nBuilding && deliverySystem[cor_row][cor_column].room ==nRoom)
			{		
				//print the all the cells (x,y) which has user package
				printf("The user(live in room %d, building %d)'s package is in (%d,%d)\n", nRoom, nBuilding, cor_row,cor_column);
				
				//return the number of packages the the storage system have
				return (deliverySystem[cor_row][cor_column].cnt);
			}
		}
	}
	
	
}
