#include <stdio.h> // Standard Input/Output functions
#include <string.h> // String handling functions
#include <time.h> // Date time functions
#include <math.h> // Math functions
#include <stdbool.h> // Boolean
#include <stdlib.h> // System commands
#include <ctype.h> // Used for tolower
#include <unistd.h> // Used for getCwd
#include <windows.h> // Used for SetConsoleTitle
#include <dirent.h> // Used for DIR
#include <sys/stat.h> // Used for DIR and MkDir
#include <stdint.h> // Used for DIR
#include <pthread.h> // Used for Marquee

#define MAX_STR_LEN 100
typedef enum { LEFT, RIGHT, LEFT_CIRCULAR, RIGHT_CIRCULAR } Direction;

void sjpSay(char* phrase);
void sjpCls();
void sjpDir();
void sjpPwd();
void sjpTime();
void sjpTitle(char* newTitle);
void sjpColor(char* colors);
void sjpMkfldr(char* fldrName);
void sjpFind(char* searchStr);
void sjpOpen(char* fileNme);
void sjpCopy(char* fileNme);
void sjpRename(char* fileNme);
void sjpDelete(char* fileNme);
void sjpSort(char* fileNme);
void sjpMarquee(char* marqueeMsg);
bool sjpExit();
char * getColor(char * color);
void * createMarquee(void* msgStr);
void marquee(char *, size_t count, Direction);
void delay(int);
void shift(char *, size_t distance, Direction);
bool isCircularShift(Direction);
bool isAllSpaces(const char *);

void main(){  
    sjpCls();
    char command[100];
    bool isRunning = true;

    while(isRunning){
        fflush(stdin);
        printf("sjpOS > ");
        scanf("%[^\n]%*c", command);
        char* action = strtok(command, " ");

        int i=0;
        char lowerCaseAction[100];
        memset(lowerCaseAction, 0, 100);
        while (command[i]){
            lowerCaseAction[i] = tolower(command[i]);
            i++;
        }

        if(strcmp(lowerCaseAction, "say") == 0){
            sjpSay(command);
        } else if(strcmp(action, "cls") == 0){
            sjpCls();
        } else if(strcmp(action, "dir") == 0){
            sjpDir();
        } else if(strcmp(action, "pwd") == 0){
            sjpPwd();
        } else if(strcmp(action, "time") == 0){
            sjpTime();
        } else if(strcmp(action, "title") == 0){
            sjpTitle(command);
        } else if(strcmp(action, "color") == 0){
            sjpColor(command);
        } else if(strcmp(action, "mkfldr") == 0){
            sjpMkfldr(command);
        } else if(strcmp(action, "find") == 0){
            sjpFind(command);
        } else if(strcmp(action, "open") == 0){
            sjpOpen(command);
        } else if(strcmp(action, "copy") == 0){
            sjpCopy(command);
        } else if(strcmp(action, "rename") == 0){
            sjpRename(command);
        } else if(strcmp(action, "delete") == 0){
            sjpDelete(command);
        } else if(strcmp(action, "sort") == 0){
            sjpSort(command);
        } else if(strcmp(action, "marquee") == 0){
            sjpMarquee(command);
        } else if(strcmp(action, "exit") == 0){
            isRunning = sjpExit();
        }else{
            printf("\nInvalid command! Please enter a valid command.\n\n");
        }
    }
}

void sjpSay(char* phrase){
    phrase = strtok(0, " ");

    while (phrase != 0) {
        printf("%s ", phrase);
        phrase = strtok(0, " ");
    }
    printf("\n\n");
}

void sjpCls(){
    system("cls");
}

void sjpDir(){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        struct stat statbuf;
        struct tm *tm;
        char datestring[256];

        while ((dir = readdir(d)) != NULL) {
            if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ) {
                continue;
            }

            if (stat(dir->d_name, &statbuf) == -1){
                continue;
            } 

            printf("Filename: %s\n", dir->d_name);
            printf("File Size: %d bytes\n", (intmax_t)statbuf.st_size);
            strftime(datestring, sizeof(datestring), "%a %b %d %X %Y", localtime(&statbuf.st_mtime));
            printf("Created on: %s\n\n", datestring);
        }
        closedir(d);
    }
}

void sjpPwd(){
    char* curDir = getcwd(NULL, 0);
    printf("Current Directory: %s\n\n", curDir);
}

void sjpTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    printf ("%s\n", asctime(timeinfo));
}

void sjpTitle(char* newTitle){
    char* tempTitle = strtok(0, " ");

    while (newTitle != 0) {
        newTitle = strtok(0, " ");
        if(newTitle != NULL){
            strcat(tempTitle, newTitle);
        }
    }
    if(SetConsoleTitle(tempTitle)){
        printf("Title has been succesffuly updated to : %s\n\n", tempTitle);
    } else{
        printf("Title has not been succesffuly updated. \n\n");   
    }  
}

void sjpColor(char* colors){
    char colorCmd[] = "COLOR \0";
    int counter = 0;
    char* background;
    char* foreground;

    while (colors != 0) {
        colors = strtok(0, " ");
        
        if(colors != NULL){
            if(counter == 0){
                counter++;
                background = getColor(colors);
                strcat(colorCmd, background);
            } else if(counter == 1){
                counter++;
                foreground = getColor(colors);
                strcat(colorCmd, foreground);
            }
        }
    }
    system(colorCmd);
    printf("\n\n");
}

void sjpMkfldr(char* fldrName){
    char* curDir = getcwd(NULL, 0);
    char* dirName = strtok(0, " ");
    strcat(curDir, "\\");
    strcat(curDir, dirName);

    if (!mkdir(curDir))
        printf("Folder created\n\n");
    else {
        printf("Unable to create folder\n\n");
    }
}

void sjpFind(char* searchStr){
    char* txtName = strtok(0, " ");
    bool isFound = false;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        struct stat statbuf;
        struct tm *tm;
        char datestring[256];

        while ((dir = readdir(d)) != NULL) {
            if( strcmp( dir->d_name,  txtName) == 0 ) {
                printf("File found!\n");
                isFound = true;

                if (stat(dir->d_name, &statbuf) == -1){
                    continue;
                }

                printf("Filename: %s\n", dir->d_name);
                printf("File Size: %d bytes\n", (intmax_t)statbuf.st_size);
                strftime(datestring, sizeof(datestring), "%a %b %d %X %Y", localtime(&statbuf.st_mtime));
                printf("Created on: %s\n\n", datestring);
            }
        }
        closedir(d);
    }
    if(!isFound){
        printf("File %s not found!\n\n", txtName);
    }
}

void sjpOpen(char* fileNme){
    FILE *fileReader;
    char * fleName = strtok(0, " ");

    fileReader = fopen(fleName, "r");

    if(fileReader == NULL){
        printf("Error file %s can't be found!", fleName);
    }

    char content = fgetc(fileReader);
    while(content != EOF){
        printf("%c", content);
        content = fgetc(fileReader);
    }

    fclose(fileReader);
    printf("\n\n");
}

void sjpCopy(char* fileNme){
    char content;
    FILE *sourceFileReader;
    FILE *targetFileReader;
    char * sourceFleName = strtok(0, " ");
    char * destFleName = strtok(0, " ");
    bool isError = false;

    sourceFileReader = fopen(sourceFleName, "r");
    if(sourceFileReader == NULL){
        printf("Error file %s can't be found!", sourceFleName);
    }

    targetFileReader = fopen(destFleName, "w");
    if(targetFileReader == NULL){
        printf("Error file %s can't be found!", destFleName);
    }

    while ((content = fgetc(sourceFileReader)) != EOF){
        fputc(content, targetFileReader);
    }

    if(!isError){
        printf("File %s is successfully copied to %s!", sourceFleName, destFleName);
    }
    printf("\n\n");
    fclose(sourceFileReader);
    fclose(targetFileReader);
}

void sjpRename(char* fileNme){
    char * oldName = strtok(0, " ");
    char * newName = strtok(0, " ");

    if(rename(oldName, newName) == 0){
        printf("Successfully renamed %s!\n\n", oldName);
    }else{
        printf("Error in renaming the file into %s!\n\n", newName);
    }
}

void sjpDelete(char* fileNme){
    char * fleName = strtok(0, " ");

    if(remove(fleName) == 0){
        printf("Successfully deleted %s!\n\n", fleName);
    }else{
        printf("Error in deleting %s!\n\n", fleName);
    }
}

struct words{
    char word[50];
} words[200];

void sjpSort(char* fileNme){
    char * fleName = strtok(0, " ");
    char * numLines = strtok(0, " ");
    int i = 0;
    int j =0;
    int size;

    FILE * fileReader = fopen(fleName, "r");
    if(fileReader == NULL){
        printf("Error file %s can't be found!", fleName);
    }

    char content;
    struct words temp;
    while(content != EOF){
        fscanf(fileReader, "%s", words[i].word);
        content = fgetc(fileReader);
        i++;
    }

    size = i-1;

    for(i=0; i<=size; i++){
        for(j=0 ; j<size-i; j++){
            if(strcmp(words[j+1].word,words[j].word)<0){
                temp = words[j];
                words[j] = words[j+1];
                words[j+1] = temp;
            }
        }		
    }

    for(i=0; i<=size; i++){
        printf("%s\n", words[i].word);
    }

    fclose(fileReader);
    printf("\n\n");
}

void sjpMarquee(char* marqueeMsg){
    char* msgStr = strtok(0, " ");

    while (marqueeMsg != 0) {
        marqueeMsg = strtok(0, " ");
        if(marqueeMsg != NULL){
            strcat(msgStr, marqueeMsg);
        }
    }

    pthread_t tid;
    pthread_create(&tid, NULL, createMarquee, (void *) msgStr);
    printf("\n\n");
}

bool sjpExit(){
    return false;
}

char * getColor(char * color){
    char * colorCode;

    if(strcmp(color, "black") == 0){
        colorCode = "0";
    } else if(strcmp(color, "blue") == 0){
        colorCode = "1";
    } else if(strcmp(color, "green") == 0){
        colorCode = "2";
    } else if(strcmp(color, "red") == 0){
        colorCode = "4";
    } else if(strcmp(color, "purple") == 0){
        colorCode = "5";
    } else if(strcmp(color, "yellow") == 0){
        colorCode = "6";
    } else if(strcmp(color, "white") == 0){
        colorCode = "7";
    } else if(strcmp(color, "gray") == 0){
        colorCode = "8";
    } else{
        printf("Invalid Color");
    }

    return colorCode;
}

char msg[MAX_STR_LEN];
const size_t repeatCount = 2;
const int delayCount = 1e6;
const int shiftLen = 1;

void marquee(char *msgStr, size_t count, Direction dir) {
    size_t len = strlen(msgStr) * 8;
    size_t i;
    size_t j;
    char * str;

    if (len > 0) {
        str = calloc(len+1, sizeof(char));
        memset(str, ' ', len);
        memcpy(str, msgStr, strlen(msgStr));

        for (i = 0; i < count; i++) {
            for (j = 0; j < len; j++) {
                printf("\r%s",str); 
                fflush(stdout);
                shift(str, shiftLen, dir);
                delay(delayCount);
            }
        }

        dir = (dir == RIGHT_CIRCULAR) ? RIGHT : LEFT;
        while (isAllSpaces(str) == false) {
            shift(str, shiftLen, dir);
            printf("\r%s",str); 
            fflush(stdout);
            delay(delayCount);
        }
        printf("\r"); 
        fflush(stdout);
        free(str);
    }
}

void shift(char *str, size_t distance, Direction dir) {
    char *source;
    char *dest;
    char *bumpFrom;
    char *rotateTo;
    char temp;
    size_t n;
    size_t strLen = strlen(str);

    if (distance > 0) {
        if ((dir == RIGHT) || (dir == RIGHT_CIRCULAR)) {
            source = str; 
            dest = str+1;
            bumpFrom = &str[strLen-1]; 
            rotateTo = &str[0];
        } else {
            source = str+1; 
            dest = str;
            bumpFrom = &str[0]; 
            rotateTo = &str[strLen-1];
        }
        for (n = 0; n < distance; n++) {
            temp = *bumpFrom;
            memmove(dest,source,strLen-1);
            if (isCircularShift(dir) == true) {
                *rotateTo = temp;
            } else {
                *rotateTo = ' ';
            }
        }
    }
}

bool isCircularShift(Direction dir) {
    return (dir == LEFT_CIRCULAR) || (dir == RIGHT_CIRCULAR);
}

bool isAllSpaces(const char *s) {
    bool allSpaces = true;
    const char *p = s;

    while ((*p != '\0') && (allSpaces == true)) {
        allSpaces = (*p++ == ' ');
    }
    return allSpaces;
}

double factorial(unsigned n) {
    if (n == 1) return n;
    return n * factorial(n-1);
}

void delay(int n) {
    int i;
    for (i = 0; i < n; i++) {
        factorial(10);
    }
}

void* createMarquee (void *msgStr){
    marquee(msgStr, repeatCount, LEFT_CIRCULAR);
    marquee(msgStr, repeatCount, RIGHT_CIRCULAR);
}

