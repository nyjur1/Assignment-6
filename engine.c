#include "engine.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int search_count(char *filename, char *target) {
    FILE *file = fopen(filename, "r");
        if (file == NULL){
            return -1;
        }

        int count = 0;
        char word [256];

        if(strlen(target)==0){
            fclose(file);
            return 0;
        }

        while(fscanf(file, "%255s", word)==1){
            char *ptr = word;

            while ((ptr=strstr(ptr,target))!=NULL){
                count++;
                ptr+=strlen(target);
            }
        }

        fclose(file);
        return count; 

   
}

struct count_result search_instance(char *filename,char *target){

    struct count_result res = {0, NULL};
     FILE *file = fopen(filename, "r");
        if (file == NULL){
            return res;
        }

        res.count= search_count(filename,target);
        if(res.count<=0){
            fclose(file);
            return res;
        }

        res.instances = malloc(res.count * sizeof(char *));

        if(res.instances==NULL){
            res.count=0;
            fclose(file);
            return res;
        }


        char line[MAX_LINE_LENGTH];
        int index = 0;

        while(fgets(line,sizeof(line), file)!= NULL){
            line[strcspn(line, "\r\n")]='\0';
            char *ptr = line;

            while (index<res.count&& (ptr=strstr(ptr,target))!= NULL){
                res.instances[index] = malloc(strlen(line)+1);

                if(res.instances[index]== NULL){
                    break;
                }
                strcpy(res.instances[index],line);

                index++;
                ptr+= strlen(target);
            }

        }

        fclose(file);
        return res; 

    
}