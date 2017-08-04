/*********************************************************************
 ** Author: Eli Goodwin
 ** Program: goodwiel.adventure
 ** Class: CS 344
 ** Date: 2017.07.23
 ** Description: Creates a simple program for an adventure style game
 **     that demonstrates threading and reading files from unknown
 **     directories. 
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>



//max size of char array
#define MAXCHARSIZE 25
#define FILEPATHNAME 75

//max number of random rooms
#define MAXRANDOMROOMS 7
//number of rooms
#define MAXROOMS 10
#define BUFFERSIZE 256;

//the room types
static char* START_ROOM = "START_ROOM";
static char* MID_ROOM = "MID_ROOM";
static char* END_ROOM = "END_ROOM";

/*********************************************************************
 ** structure: room
 ** Description: stores room information for generating a room graph.
 **     roomName: the name of the room in question
 **     roomType: type of room
 **     roomUsed: flag for the if the room was already used to generate
 **         a random room
 **     roomConnections: adjacnecy list for rooms that are connected to
 **         this room
 **     connectionCount: stores the number of rooms connected to this
 **         room
 *********************************************************************/
struct room{
    char roomName[MAXCHARSIZE];
    char roomType[MAXCHARSIZE];
    int roomUsed;
    char connections[MAXRANDOMROOMS][BUFFERSIZE];
    int connectionCount;
};


/*********************************************************************
 ** Function: writeFile
 ** Description: writes room data to individual files for storage
 ** Parameters: array of pointers to rooms
 ** Pre-Conditions: rooms must form a graph
 ** Post-Conditions: None
 ** Return: None
 *********************************************************************/
void writeFile(struct room theRoom[]);


/*********************************************************************
 ** Function: getRandomRoom
 ** Description: grabs a random room
 ** Parameters: an array of room pointers
 ** Pre-Conditions: room array must not be empty
 ** Post-Conditions: None
 ** Return: a single pointer to a randomly selected room
 *********************************************************************/
struct room* getRandomRoom(struct room theRooms[]);


/*********************************************************************
 ** Function: isListFul
 ** Description: tests if the adjacency list is for the room is "full"
 ** Parameters: a pointer to a room struct
 ** Pre-Conditions: room pointer must not be null
 ** Post-Conditions: None
 ** Return: if the rooms list has 3 or more entries
 *********************************************************************/
int isListFull(struct room* theRoom);


/*********************************************************************
 ** Function: isGraphFull
 ** Description: tests if the graph is full or not
 ** Parameters: an array of room pointers
 ** Pre-Conditions: room pointers must not be NULL
 ** Post-Conditions: None
 ** Return: if the all rooms have 3 or more connections or not
 *********************************************************************/
int isGraphFull(struct room theRoom[]);


/*********************************************************************
 ** Function: connectRoom
 ** Description: connects two rooms together
 ** Parameters: roomA and roomB are pointers to room structs
 ** Pre-Conditions: rooms must not be NULL
 ** Post-Conditions: None
 ** Return: None
 *********************************************************************/
void connectRoom(struct room* roomA, struct room* roomB);


/*********************************************************************
 ** Function: canAddConnection
 ** Description: tests if the randomly selected room has free space
 **     in it's adjacency list
 ** Parameters: theRoom is the room struct in question
 ** Pre-Conditions:
 ** Post-Conditions: None
 ** Return: int 1 == true (avialble space), int 0 == false (no space)
 *********************************************************************/
int canAddConnection(struct room* theRoom);


/*********************************************************************
 ** Function: andRandomConnection
 ** Description: creates a non sparse graph of rooms with nonrepeated
 **     connections between the same rooms
 ** Parameters: struct room** theRooms an array of rooms
 ** Pre-Conditions: rooms must not be null and have vailid names
 **     assigned to them
 ** Post-Conditions: None
 ** Return: None
 *********************************************************************/
void addRandomConnection(struct room rooms[]);


/*********************************************************************
 ** Function: isSameRoom
 ** Description: tests if a randomly selected room A is the same as
 **     room B
 ** Parameters: struct room* roomA, struct room* roomB
 ** Pre-Conditions: both rooms must not be null and have valid names
 ** Post-Conditions: None
 ** Return: int 1 = the rooms are the same, int = 0 the rooms are
 **     different
 *********************************************************************/
int isSameRoom(struct room* roomA, struct room* roomB);


/*********************************************************************
 ** Function: printRanomdRooms
 ** Description: prints each room and its associated adjacency list
 ** Parameters: struct room** theRooms
 ** Pre-Conditions: theRooms must be properly initialized and filled
 **     data
 ** Post-Conditions: None
 ** Return: none
 *********************************************************************/
void printRandomRooms(struct room theRooms[]);



/*********************************************************************
 ** Function: printRooms
 ** Description: prints each room and its associated adjacency list
 ** Parameters: struct room** theRooms
 ** Pre-Conditions: theRooms must be properly initialized and filled
 **     data
 ** Post-Conditions: None
 ** Return: none
 *********************************************************************/
void printRooms(struct room theRooms[]);


/*********************************************************************
 ** Function: roomAlreadyInList
 ** Description: tests if the randomly selected room B was already used
 **     in room A adjacency list
 ** Parameters: struct room* roomA, struct room* roomB
 ** Pre-Conditions: both rooms not be null and have non empty adjacency
 **     lists
 ** Post-Conditions: None
 ** Return: int 1 = already used in list of A, int 0 == not already
 **     used in room A list
 *********************************************************************/
int roomAlreadyInList(struct room* roomA, struct room* roomB);


/*********************************************************************
 ** Function: createDirectory
 ** Description: creates a directory for saving files into
 ** Parameters: char* directoryName
 ** Pre-Conditions: must have valid directory name,
 ** Post-Conditions: None
 ** Return: None
 *********************************************************************/
void createDirectory(char* directoryName);


/*********************************************************************
 ** Function: findDirectory
 ** Description: find the directory that contains the most recent room
 **     files and copies it's name into a string
 ** Parameters: char sourceDirectory
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ** Return: None
 *********************************************************************/
void findDirectory(char directoryResult[]);


/*********************************************************************
 ** Function: readFileIn
 ** Description: reas a room file in and then stores it
 ** Parameters: char filePath[], struct *room roomsIn[]
 ** Pre-Conditions: directory of rooms must be known, rooms array
 ** must not be empty and be allocated
 ** Post-Conditions:
 ** Return: none
 *********************************************************************/
//void readFilesIn(char filePath[], struct room roomsIn[]);
void readFilesIn(struct room roomsIn[]);


/*********************************************************************
 ** Function: findStartEnd
 ** Description: finds the start and end rooms of the room array
 ** Parameters: struct room* theRoooms[], int startEndIndxs
 ** Pre-Conditions: rooms must have a start or end type
 ** Post-Conditions: none
 ** Return: int 1 = success, int 0 = failure
 *********************************************************************/
int findStartEnd(struct room theRooms[], int startEndIndxs[]);

/*********************************************************************
 ** Function: displayChoices
 ** Description: displays the contents of the rooms so the user can
 **     make a selection
 ** Parameters: struct room theRoooms[]
 ** Pre-Conditions: rooms must be initilized
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void displayChoices(struct room* aRoom);


/*********************************************************************
 ** Function: validInput
 ** Description: tests if input was valid or not for things other than
 ** 'time'
 ** Parameters: struct room theRoooms[], const char input[]
 ** Pre-Conditions: rooms must be initilized, cstring must not be empty
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
int validInput(struct room* aRoom, const char input[]);


/*********************************************************************
 ** Function: findRoom
 ** Description: tests if the room is a valid choice, if so returns its
 ** index in the room array
 ** Parameters: struct room theRoooms[], const char input[]
 ** Pre-Conditions: rooms must be initilized, cstring must not be empty
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
int findRoom(struct room rooms[], const char input[]);

/*********************************************************************
 ** Function: getTime
 ** Description: gets current time in string format and copies it to a
 **     cstring
 ** Parameters: char currentTime[]
 ** Pre-Conditions: currentTime must be initialized to repeating '\0'
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void getTime(char currentTime[]);


/*********************************************************************
 ** Function: writeTime
 ** Description: writes the current time to a txt file
 ** Parameters: char currentTime[]
 ** Pre-Conditions: currentTime must have time data in it
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
//void writeTime(char currentTime[]);
void* writeTime(void* currentTime);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    
    
    //create thread
    pthread_t stupidThread[MAXCHARSIZE];
    int threadCount = 0;
    
    //make the room
    struct room theRooms[MAXRANDOMROOMS];
    int i = 0;
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        memset(theRooms[i].roomName, '\0', MAXCHARSIZE);
        memset(theRooms[i].roomType, '\0', MAXCHARSIZE);
        theRooms[i].roomUsed = 0; //set the room to unused
        theRooms[i].connectionCount = 0;
        int j = 0;
        for(j = 0; j < MAXRANDOMROOMS; ++j){
            memset(theRooms[i].connections[j], '\0', MAXCHARSIZE);
        }
    }
    
    //read the files in
    readFilesIn(theRooms);
    //print the rooms out
    //printRooms(theRooms);
    
    //get the start and ending indxs
    int startEnd[2] = {-1, -1};
    findStartEnd(theRooms, startEnd);
    
    //start the search for the room
    int foundRoom = -1; //flag if the room was found
    int roomIndx = startEnd[0]; //index for what room we are in
    int roomCount = 0; ///room counter
    int visitTracker[BUFFERSIZE]; //tracks the index
    char userInput[BUFFERSIZE]; //stores user input
    memset(userInput, '\0', BUFFERSIZE);
    
    //while the room is not found, prompt user to find it
    while(foundRoom !=1 && roomCount < BUFFERSIZE){
        //did they find the room?
        if(roomIndx == startEnd[1]){
            foundRoom = 1;
            printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
            printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: \n", roomCount);
            for(i = 0; i < roomCount; ++i){
                printf("%s\n", theRooms[visitTracker[i]].roomName);
            }
            
        }
        //room is not found
        if(foundRoom != 1){
            printf("\n");
            displayChoices(&theRooms[roomIndx]);
            fgets(userInput, BUFFERSIZE, stdin);
            strtok(userInput, "\n");
        
            //find the room in question... but first what is the currnt time?...
            if(strcmp(userInput, "time") == 0){
                if(threadCount < 50){
                    ++threadCount;
                    char currentTiime[MAXCHARSIZE];
                    memset(currentTiime, '\0', MAXCHARSIZE);
                    getTime(currentTiime);
                    //create the thread
                    pthread_create(&stupidThread[threadCount], NULL, writeTime, (void *) &currentTiime[0]);
                    printf("\n");
                    printf("\t%s\n", currentTiime);
                    //join threads
                    pthread_join(stupidThread[threadCount], NULL);
                }
                else{
                    printf("Too many threads have been created, cannot call time anymore.\n");
                }
            }
            
            //not time tracking
            else{
                //test if inputwas valid
                if(validInput(&theRooms[roomIndx], userInput) == 1){
                    roomIndx = findRoom(theRooms, userInput);
                    if(roomIndx == -1){
                        printf("something terrible has happened\n");
                        return -1;
                    }
                    visitTracker[roomCount] = roomIndx;
                    ++roomCount;
                }
                //input was bad
                else{
                    printf("\n");
                    printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
                    printf("\n");
                }
                
            }
            
        }

    }
    
    pthread_mutex_destroy(&mutex);
    return 0;
}


int isListFull(struct room* theRoom){
    if(theRoom->connectionCount > 3)
        return 1;
    else
        return 0;
}

int isGraphFull(struct room theRooms[]){
    int graphStatus = 0;
    int i;
    //if graph stauts is less than 7 that means there is free space available
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        graphStatus += isListFull(&theRooms[i]);
    }
    //if graph status == 7, that means the all rooms have 3 or more connections
    if(graphStatus == 7){
        return 1;
    }
    //fewer than 7 "full" rooms
    else{
        return 0;
    }
}

int canAddConnection(struct room *theRoom){
    if(theRoom->connectionCount < 6)
        return 1;
    else
        return 0;
}


int isSameRoom(struct room* roomA, struct room* roomB){
    if(strcmp(roomA->roomName, roomB->roomName) == 0){
        return 1;
    }
    else
        return 0;
}


void connectRoom(struct room *roomA, struct room* roomB){
    //connect room
    //next free location will be connection count for roomA, set that as the index for the
    
    strcpy(roomA->connections[roomA->connectionCount], roomB->roomName);
    //increment room count
    ++roomA->connectionCount;
    //printf("connecting: %s to %s\n", roomA->roomName, roomB->roomName);
    
    return;
}

void addRandomConnection(struct room rooms[]){
    struct room* roomA = NULL;
    struct room* roomB = NULL;
    int noEmptyRoom = 1;
    
    //find an room that has connections available
    while(noEmptyRoom == 1){
        roomA = getRandomRoom(rooms);
        if(canAddConnection(roomA)){
            noEmptyRoom = 0;
        }
    }
    
    do{
        roomB = getRandomRoom(rooms);
        //room list must have space, cannot be connecting itself, and the romo must not already be used
    }while(canAddConnection(roomB) == 0 || isSameRoom(roomA, roomB) || roomAlreadyInList(roomA, roomB));
    
    //connect the rooms
    connectRoom(roomA, roomB);
    connectRoom(roomB, roomA);
    return;
}


struct room* getRandomRoom(struct room theRooms[]){
    int randomIndx = rand() % MAXRANDOMROOMS;
    //printf("random room: %s\n", theRooms[randomIndx]->roomName);
    
    return &theRooms[randomIndx];
}


void printRandomRooms(struct room theRooms[]){
    int i, j = 0;
    
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        printf("Room %i: %s\n", i + 1, theRooms[i].roomName);
        j = 0;
        while(theRooms[i].connections[j] != NULL && j < 6){
            printf("\tConnection %d: Room name: %s Type: %s\n", j + 1, theRooms[i].connections[j], theRooms[i].roomType);
            ++j;
        }
    }
    
    return;
}

void printRooms(struct room theRooms[]){
    int i, j = 0;
    
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        printf("Room %i: %s\n", i + 1, theRooms[i].roomName);
        j = 0;
        while(theRooms[i].connections[j][0] != '\0' && j < 6){
            if(theRooms[i].connections[j][0] != '\0'){
                printf("\tConnection %d: Room name: %s Type: %s\n", j + 1, theRooms[i].connections[j], theRooms[i].roomType);
            }
            ++j;
        }
    }
    
    return;
}

int roomAlreadyInList(struct room* roomA, struct room* roomB){
    
    int count = 0;
    //go through the adjacency list of room A and test if roon B is already connected
    while(count < roomA->connectionCount){
        if(strcmp(roomA->connections[count], roomB->roomName) == 0){
            //already used
            return 1;
        }
        ++count;
    }
    
    //not used
    return 0;
}

void createDirectory(char* directoryName){
    //copy in directory name
    strcpy(directoryName, "./goodwiel.rooms.");
    
    //get process id and convert it to cstring
    int processID = getpid();
    printf("process id: %d\n", processID);
    char convertProcessID[MAXCHARSIZE];
    sprintf(convertProcessID, "%d", processID);
    puts(convertProcessID);
    
    //make full directory name
    strcat(directoryName, convertProcessID);
    //strcat(directoryName, "/");
    
    
    //is it right?
    printf("file path is: %s\n", directoryName);
    
    int result = mkdir(directoryName,  0777);
    
    printf("result: %d\n", result);
    printf("...directory made...\n");
    
    return;
}

void writeFile(struct room theRoom[]){
    //create directory for saving files
    char filePath[FILEPATHNAME];
    memset(filePath, '\0', FILEPATHNAME);
    createDirectory(filePath);
    
    
    char fileName[FILEPATHNAME];
    FILE* fileOut;
    int i = 0;
    //go through the array of room and save the room data
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        memset(fileName, '\0', FILEPATHNAME);
        //create the file name and directory filepath name of file
        sprintf(fileName, "%s/room%d", filePath, i);
        
        //create FILE
        fileOut = fopen(fileName, "w+");
        
        //write contents of room into file
        //fprintf(fileOut, "ROOM NAME: %s\n", theRoom[i].roomName);
        int counter = 0;
        while(theRoom[i].connections[counter][0] != '\0' && counter < 6){
            fprintf(fileOut, "CONNECTION %d: %s\n", counter + 1, theRoom[i].connections[counter]);
            ++counter;
        }
        
        //fprintf(fileOut, "ROOM TYPE: %s\n", theRoom[i].roomType);
        //close file
        fclose(fileOut);
    }
    
    return;
}


void findDirectory(char directoryResults[]){
    int newestDirTime = -1;
    char targetPrefix[] = "goodwiel.rooms";
    char newestDirNam[BUFFERSIZE];
    memset(newestDirNam, '\0', BUFFERSIZE);
    
    DIR* dirToCheck;
    struct dirent *fileInDir;
    struct stat dirAttributes;
    
    dirToCheck  = opendir(".");
    //if the directory opened...
    if(dirToCheck > 0){
        while((fileInDir = readdir(dirToCheck)) != NULL){
            if(strstr(fileInDir->d_name, targetPrefix) != NULL){
                //printf("Found the prefix: %s\n", fileInDir->d_name);
                stat(fileInDir->d_name, &dirAttributes);
                if(((int)dirAttributes.st_mtime) > newestDirTime){
                    newestDirTime = (int)dirAttributes.st_mtime;
                    memset(newestDirNam, '\0', sizeof(newestDirNam));
                    strcpy(newestDirNam, fileInDir->d_name);
                    
                    //printf("Newer subdir: %s, new time: %d\n", fileInDir->d_name, newestDirTime);
                    //printf("newest directory: %s\n", newestDirNam);
                    strcpy(directoryResults, newestDirNam);
                    //printf("the directory is: %s\n", directoryResults);
                }
            }
        }
    }
    else{
        printf("Directory could not be found :'(\n");
    }
    //printf("done with directory finding\n");
    
    closedir(dirToCheck);
    return;
}


//void readFilesIn(char filePathName[], struct room roomsIn[]){;
void readFilesIn(struct room roomsIn[]){;
    //open file path
    FILE* file;
    char filePath[BUFFERSIZE];
    //memset(filePath, '\0', BUFFERSIZE);
    char fileName[BUFFERSIZE];
    //memset(fileName, '\0', BUFFERSIZE);
    
    findDirectory(filePath);
    
    //memset(holder, '\0', BUFFERSIZE);
    
    //so long as there are files, read each one into
    int i = 0;
    
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        //get the room name
        int connectionCounter = 0; //count the connections in the room
        //make the room name
        sprintf(fileName, "%s/room%d", filePath, i);
//        printf("%s\n", fileName);
        //open the file
        file = fopen(fileName, "r");
        
        //move the cursor to the begining of the room name
        fseek(file, 11, SEEK_CUR);
        //get the line into room name
        char holder[BUFFERSIZE]; //holds the word currently
        fgets(holder, BUFFERSIZE, file);
        strcpy(roomsIn[i].roomName, strtok(holder, "\n"));
        
        //while connections add them to the adjacency list by name only and increment counter
        while(fgets(holder, 12, file)){
            
            if(strcmp(holder, "CONNECTION ") == 0){
                //advance file pointer to the start of where the conneciton name is
                fseek(file, 3, SEEK_CUR);
                fgets(holder, BUFFERSIZE, file);
                //copy the room name in
                strcpy(roomsIn[i].connections[connectionCounter], strtok(holder, "\n"));
                ++connectionCounter;
            }
            
            //copy room type in to the right place
            else{
                fgets(holder, BUFFERSIZE, file);
                //printf("%s", holder);
                strcpy(roomsIn[i].roomType, strtok(holder, "\n"));
            }
            
        }
        
        //set the connection count
        roomsIn[i].connectionCount = connectionCounter;
        //close the curent file
        fclose(file);
    }
    
    return;
}

int findStartEnd(struct room theRooms[], int startEndIndxs[]){
    int i = 0;
    //printf("Searching...\n");
    
    for(i = 0; i < MAXRANDOMROOMS; ++i){
        
        if(strcmp(theRooms[i].roomType, START_ROOM) == 0){
            startEndIndxs[0] = i;
            //printf("found start @ %d\n", i);
        }
        if(strcmp(theRooms[i].roomType, END_ROOM) == 0){
            startEndIndxs[1] = i;
        }
    }
    
    if(startEndIndxs[0] != -1 && startEndIndxs[1] != -1)
        return 1;
    
    return 0;
}

void displayChoices(struct room* aRoom){
    printf("CURRENT LOCATION: %s\n", aRoom->roomName);
    int i;
    
    printf("POSSIBLE CONNECTIONS: ");
    for(i = 0; i < aRoom->connectionCount; ++i){
        printf("%s, ", aRoom->connections[i]);
    }
    printf("\n");
    printf("WHERE TO? >");
    
    return;
}

int validInput(struct room* aRoom, const char input[]){
    int i = 0;
    int found = -1;
    //tests if the input was valid for
    for(i = 0; i < aRoom->connectionCount && found < 0; ++i){
        if(strcmp(aRoom->connections[i], input) == 0){
            return 1;
        }
    }
    
    return 0;
}


int findRoom(struct room rooms[], const char input[]){
    int i = 0;
    int found = -1;
    //loop throught the array and test if the input exists or not
    for(i = 0; i < MAXRANDOMROOMS && found < 0; ++i){
        if(strcmp(rooms[i].roomName, input) == 0){
            found = i;
            return found;
        }
    }
    //if somethign weird happened
    return -1;
}


void getTime(char currentTime[]){
    time_t theTIme;
    time(&theTIme);
    
    //save current time to cstring
    strcpy(currentTime, ctime(&theTIme));
    
    return;
}



void* writeTime(void* param){
    //create mutex lock
    pthread_mutex_lock(&mutex);
    
    //specify file object
    FILE* file;
    char* currentTime = (char*)param;
    

    //create fileName
    char filename[MAXCHARSIZE];
    memset(filename, '\0', MAXCHARSIZE);
    strcpy(filename, "currentTime.txt");
    //open file
    file = fopen(filename, "a");
    
    //write currenTime to end of file
    fprintf(file, "%s", currentTime);
    //close file
    fclose(file);
    
    //unlock mutex
    pthread_mutex_unlock(&mutex);

    
    return NULL;
}

