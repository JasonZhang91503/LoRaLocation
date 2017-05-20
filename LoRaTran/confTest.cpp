#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_PATH "postman.conf"
#define SIZE         256

/*config vars*/
char path[SIZE];
char extension[SIZE];
int limit=0;

bool readConfig(){
    char name[SIZE];
    char value[SIZE];

    FILE *fp = fopen(CONFIG_PATH, "r");
    if (fp == NULL) { return false; }
    while(!feof(fp)){
        memset(name,0,SIZE); memset(value,0,SIZE);

        /*Read Data*/
        fscanf(fp,"%s = %s\n", name, value);

        if (!strcmp(name, "path")){
            strcpy(path, value);

        }else if (!strcmp(name, "extension")){
            strcpy(extension, value);

        }else if (!strcmp(name, "limit")){
            limit = atoi(value);
        }
    }
    fclose(fp);

    return true;
}


/*= main function=*/
int main(int argc, const char ** argv){
    memset(path,0,SIZE);
    memset(extension,0,SIZE);

    /*read config*/
    if(!readConfig()){
        fprintf(stderr,"read config fail!");
        return 1;
    }

    printf("path = %s\n", path);
    printf("extension = %s\n", extension);
    printf("limit = %d\n", limit);
    system("PAUSE");
    return 0;
}