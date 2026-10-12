#include "engine.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

struct worker_args {
    char *filename;
    char *target;
    int count; 
    long start; //start of the chunk to process
    long end; //end of the chunk to process
    char **instances; 
};


void *instance_worker(void *arg) {
    struct worker_args *args = arg;
    FILE *file = fopen(args->filename, "r");
        if (file == NULL){
            return NULL;
        }

    if(args->start>0){
            fseek(file, args->start-1, SEEK_SET);
            int before = fgetc(file);
            if(before != '\n'){
                int c;

                while((c=fgetc(file))!=EOF && c!='\n'){

                }
            }

        }
        else{
            fseek(file,0,SEEK_SET);
        }
       

    char line[MAX_LINE_LENGTH];

    args->count=0;
    args->instances=NULL;
    
    while (ftell(file)<args->end&& fgets(line,sizeof(line), file)!= NULL){
        line[strcspn(line,"\r\n")] ='\0';
        char *ptr= line; 

        while((ptr=strstr(ptr, args->target))!=NULL){
            char **temp = realloc(args->instances, (args->count+1)*sizeof(char*));
            
            if(temp== NULL){
                break;
            }
            args->instances = temp; 
            args->instances[args->count] = malloc(strlen(line)+1); 
            if (args->instances[args->count]==NULL){
                break;
            }

            strcpy(args->instances[args->count], line);

            args->count++;
            ptr+= strlen(args->target);
            

            }

        }

        fclose(file);
        return NULL; 
}

void *count_worker(void *arg) {
    struct worker_args *args = arg;

    FILE *file = fopen(args->filename, "r");
        if (file == NULL){
            return NULL; 
        }

        if(args->start>0){
            fseek(file, args->start-1, SEEK_SET);
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

            if(position>=args->end){
                break;
            }
            fseek(file,position,SEEK_SET);

            if(fscanf(file, "%255s", word)!= 1){
                break;
            }
            char *ptr = word; 

            while((ptr=strstr(ptr, args->target))!=NULL){
                args->count++;
                ptr+= strlen(args->target);
            }
        }
        fclose(file);
        return NULL; 
}




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

    pthread_t threads[4];
    struct worker_args args[4];

    for(int i=0; i<4; i++){
        args[i].filename = filename;
        args[i].target = target;
        args[i].start = i*chunk;
        if (i==3){
            args[i].end= size;
        }
        else{
            args[i].end = (i+1)*chunk;
        }

        args[i].count = 0;

        pthread_create(
        &threads[i], //pointer to pthread_t identifier
        NULL, //attributes, set to NULL
        count_worker, //function where worker starts execution
        &args[i] //pointer to argument structure
        ); 

    }

    int total =0;

    for(int i=0; i<4; i++){
        pthread_join(threads[i], NULL);
        total+=args[i].count;
    }

    return total; 

}

  


struct count_result search_instance(char *filename,char *target){
    struct count_result res = {0, NULL};

    if(strlen(target)==0){
        return res;
    }

    struct stat st;

     if(stat(filename, &st)!=0){
        return res;
    }

    long size = st.st_size;
    long chunk = size/4;

    pthread_t threads[4];
    struct worker_args args[4];

    for(int i=0; i<4; i++){
        args[i].filename = filename;
        args[i].target = target;
        args[i].start = i*chunk;
        if (i==3){
            args[i].end= size;
        }
        else{
            args[i].end = (i+1)*chunk;
        }

        args[i].count = 0;
        args[i].instances= NULL; 

        pthread_create(
        &threads[i], //pointer to pthread_t identifier
        NULL, //attributes, set to NULL
        instance_worker, //function where worker starts execution
        &args[i] //pointer to argument structure
        ); 

    }

    for(int i=0; i<4; i++){
        pthread_join(threads[i], NULL);
        res.count += args[i].count;
    }

    res.instances = malloc(res.count * sizeof(char *));

    if(res.count>0 && res.instances == NULL){
        for(int i=0; i<4; i++){
            for(int j=0; j<args[i].count; j++){
                free(args[i].instances[j]);
            }
            free(args[i].instances);

    }

    res.count=0;
    return res; 

}


int index =0;

for(int i =0; i<4; i++){
    for(int j=0; j<args[i].count; j++){
        res.instances[index] = args[i].instances[j];
        index++;
}

        free(args[i].instances);
    }

    return res;

}
