#include "Material.h"
#include <string.h>

Material::Material(const char *path){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Impossible to open the file \n");
        return;
    }

    while(1){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);

        if(res == EOF)
            break;
        
        if(strcmp(lineHeader, "Ns") == 0){
            fscanf(file, "%f\n", &Ns);
        }else if(strcmp(lineHeader, "Ka") == 0){
            fscanf(file, "%f %f %f\n", &Ka.p[0], &Ka.p[1], &Ka.p[2]);
        }else if(strcmp(lineHeader, "Kd") == 0){
            fscanf(file, "%f %f %f\n", &Kd.p[0], &Kd.p[1], &Kd.p[2]);
        }else if(strcmp(lineHeader, "Ks") == 0){
            fscanf(file, "%f %f %f\n", &Ks.p[0], &Ks.p[1], &Ks.p[2]);
        }else if(strcmp(lineHeader, "Sh") == 0){
            fscanf(file, "%f\n", &Sh);
        }else if(strcmp(lineHeader, "Rs") == 0){
            fscanf(file, "%f\n", &Rs);
        }
    }
}