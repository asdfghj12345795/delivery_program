#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

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
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
		//Initialize corresponding memory values using NULL to initialize one storage box by susing NULL.
		// date of byte can be removed by NULL
		deliverySystem[x][y].building = NULL; 
		deliverySystem[x][y].room = NULL;		
		deliverySystem[x][y].cnt = NULL;
		deliverySystem[x][y].passwd = NULL;
		deliverySystem[x][y].context = NULL;		
		
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
		if(try_passwd == deliverySystem[x][y].passwd) 
		{
			return 0;
		 } 
		 
		 // if the password is not matching, notice user that It is wong password and return.
		else 
		{
			printf("It's wrong password. Please check it.\n");
			return -1;			
		 } 
	
}


// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {

	// open the entered file as 'fopen'
	
	// copy the contents of the delivery system as a function of "w" to open files
	
	// if (fp=NULL) = Fail- --> No content
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	
	//allocate memory using dynamic memory allocation to deluverySytem structure
	
	// use 'if' to confirm allocated memory is NULL
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	// clear memory of deliverySystem allocated to dynamic memory
	
	//free (pointor for dynamic memory)
	
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
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
		deliverySystem[x][y].building = nBuilding;
		deliverySystem[x][y].room = nRoom ;
		deliverySystem[x][y].passwd = passwd;
		deliverySystem[x][y].context = msg;
		deliverySystem[x][y].cnt ++;
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
		char try_passwd[PASSWD_LEN+1];
	
		printf("Please enter your password.\n"); //Enter Password
		scanf("%s", &try_passwd);
		
		 //Compare the entered password with the specific locker password
		if (try_passwd ! = deliverySystem[x][y].passwd) 
		{
			printf("It's wrong password. Please check it.\n");
			return -1;	// failed ot extract.		
		 }
		 
		else
		{	
			//check the context of the storage
			printf("THE context of package is %s", deliverySystem[x][y].context); 
			// after extract the context, initialize the storage (x,y)
			initStorage(x,y); 
			
			return 0;
		  }  
	 
	
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	int x; //the variable about the row 
	int y; //the varialbe about the colunm
	 
	//Check from all structured arrangement to the last parcel shelf
	
	// there are a total of 6 columns, so the number of the last column number 5.
	for(y=0;y<6;y++)
	{
		// there are a total of 4 rows, so the number of the last column number 3.
		for(x=0;x<4;x++)
		{
	 		//Determine if the number of the building entered and the room number match the building number and the room number of the structure. 
			if(deliverSystem.[x][y]building == nBuilding && deliverySystem.room[x][y] ==nRoom)
			{		
				//print the all the cells (x,y) which has user package
				printf("The user(live in room %d, building %d)'s package is in (%d,%d)\n", nRoom, nBuildig, x,y);
				
				//return the number of packages the the storage system have
				return (deliverySystem.building[x][y].cnt);
			}
		}
	}
	
	
}
