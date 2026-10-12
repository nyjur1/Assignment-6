#include "engine.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <omp.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int search_count(char *filename, char *target) {

  if(strlen(target)==0){
        return 0;
    }

    struct stat st;

    if(stat(filename, &st)!=0){
        return -1;
    }

    long size = st.st_size;
    long chunk = size/4;
    int thread_count = 4; 

    int some[4] ={0};

    #pragma omp parallel for num_threads(thread_count)


    for(int i=0; i<thread_count; i++){
        long start = i*chunk;
        long end;
        if (i==3){
            end= size;
        }
        else{
            end = (i+1)*chunk;
        }

        FILE *file = fopen(filename, "r");
        if (file == NULL){
            continue; 
        }

        if(start>0){
            fseek(file, start-1, SEEK_SET);
            int before = fgetc(file);
            if(!isspace(before)){
                int c;

                while((c=fgetc(file))!=EOF && !isspace(c)){

                }
            }

        }
        else{
            fseek(file,0,SEEK_SET);
        }
       

        char word [256];

        while(1){

        
            int c; 
            while((c=fgetc(file))!=EOF && isspace(c)){
            }

            if (c==EOF){
                break;
            }
            long position = ftell(file)-1;

            if(position>=end){
                break;
            }
            fseek(file,position,SEEK_SET);

            if(fscanf(file, "%255s", word)!= 1){
                break;
            }
            char *ptr = word; 

            while((ptr=strstr(ptr,target))!=NULL){
                some[i]++;
                ptr+= strlen(target);
            }
        }
        fclose(file);
        
}
    int total = 0;

    for(int i=0; i<thread_count; i++){
        total+=some[i];
    }

    return total; 


   
}


struct count_result search_instance(char *filename,char *target){

    struct count_result res ={0,NULL};
    if(strlen(target)==0){
        return res;
    }

    struct stat st;

    if(stat(filename, &st)!=0){
        return res;
    }

    long size = st.st_size;
    long chunk = size/4;
    int thread_count = 4; 

    struct count_result some[4] ={0};

    #pragma omp parallel for num_threads(thread_count)

    for(int i=0; i<thread_count; i++){
        long start = i*chunk;
        long end;
        if (i==3){
            end= size;
        }
        else{
            end = (i+1)*chunk;
        }

        FILE *file = fopen(filename, "r");
        if (file == NULL){
            continue; 
        }

         if(start>0){
            fseek(file, start-1, SEEK_SET);
            int before = fgetc(file);
            
            if(before!='\n'){
                int c;

                while((c=fgetc(file))!=EOF && c!='\n'){

                }
            }

        }
        else{
            fseek(file,0,SEEK_SET);
        }
       

        char line [MAX_LINE_LENGTH];

        some[i].count=0;
        some[i].instances=NULL; 

        while (ftell(file)<end&& fgets(line,sizeof(line), file)!= NULL){
        line[strcspn(line,"\r\n")] ='\0';
        char *ptr= line; 

        while((ptr=strstr(ptr, target))!=NULL){
            char **temp = realloc(some[i].instances, (some[i].count+1)*sizeof(char*));
            
            if(temp== NULL){
                break;
            }
            some[i].instances = temp; 
            some[i].instances[some[i].count] = malloc(strlen(line)+1); 
            if (some[i].instances[some[i].count]==NULL){
                break;
            }

            strcpy(some[i].instances[some[i].count], line);

            some[i].count++;
            ptr+= strlen(target);
            

            }

        }

        fclose(file); 
}



    }