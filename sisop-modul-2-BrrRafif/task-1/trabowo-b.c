#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *file;
    char *output_f = "/home/berwyn/Sistem Operasi/film";
    char poster[51][51];
    int i = 0, acak;

    dir = opendir(output_f);
    
    while ((file = readdir(dir)) != NULL){
        if(strstr(file->d_name, ".jpg")){
            strcpy(poster[i], file->d_name);
            i++;
        }
    }

    closedir(dir);

    srand(time(NULL));
    acak = rand() % i;
    printf("Film for Trabowo & Peddy: %s", poster[acak]);

    return 0;
}
