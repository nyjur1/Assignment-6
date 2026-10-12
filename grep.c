#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // TODO: parse the arguments in argv.
    // You can expect argv[1] to be the mode
    // You can expect argv[2] to be the filepath
    // You can expect argv[3] to be the target word

    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <count|instance> <input_file> <target_word>\n",
                argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *filename = argv[2];
    char *target = argv[3];

    if(strcmp(mode,"count")==0){
        int count=search_count(filename, target);
        printf("Found: %d of %s in %s\n", count, target, filename);
    }
    else if(strcmp(mode,"instance")==0){
        struct count_result res =search_instance(filename, target);
        printf("Found: %d of %s in %s\n", res.count, target, filename);


        for(int i=0; i<res.count; i++){
            printf("res.instances[%d]: %s\n", i, res.instances[i]);
            free(res.instances[i]);

        }

        free(res.instances);
    }

    else{
        printf("Invalid mode\n");
        return 1; 
    }



    return 0;
}