#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>

char *A_horror = "/home/berwyn/Sistem Operasi/film/FilmHorror";
char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
char *A_drama = "/home/berwyn/Sistem Operasi/film/FilmDrama";
char *output_f = "/home/berwyn/Sistem Operasi/film";
int giliran = 0, idx = 0;

void pindah(char *film, char *alamat, FILE *FileLog, char *folder){
    pid_t child = fork();

    if (child == 0){
        char src[300], dest[300];

        sprintf(src, "%s/%s", output_f, film);
        sprintf(dest, "%s/", alamat);

        if (execl("/usr/bin/mv", "mv", src, dest, NULL) == -1) {
            perror("Gagal memindahkan file");
            exit(EXIT_FAILURE);
        }
    }

    else {
        int status;
        wait(&status);

        if(WIFEXITED(status)){

            time_t waktu = time(NULL);
            struct tm *time = localtime(&waktu);
            char simpanWaktu[30], *nama;
            strftime(simpanWaktu, sizeof(simpanWaktu), "%d-%m-%Y %H:%M:%S", time);

            if(giliran % 2 == 0) nama = "Trabowo";
            else nama = "Peddy";
            (giliran)++;

            fprintf(FileLog, "[%s] %s: %s telah dipindahkan ke %s\n", simpanWaktu, nama, film, folder);
        }
    }
}

void memilah(FILE *FileLog, char poster[][51]){
    DIR *dir;
    struct dirent *file;

    dir = opendir(output_f);

    while ((file = readdir(dir)) != NULL){
        if(strstr(file->d_name, ".jpg")){
            strcpy(poster[idx], file->d_name);
            (idx)++;
        }
    }

    closedir(dir);

    pid_t child_id = fork();
    if (child_id == 0){
        for (int i = 0; i < (idx / 2); i++){
            if (strstr(poster[i], "horror")) pindah(poster[i], A_horror, FileLog, "FilmHorror");
            
            else if (strstr(poster[i], "animasi")) pindah(poster[i], A_animasi, FileLog, "FilmAnimasi");
            
            else if (strstr(poster[i], "drama")) pindah(poster[i], A_drama, FileLog, "FilmDrama");
        }
    }

    else{
        for (int i = (idx - 1); i >= (idx / 2); i--){
            if (strstr(poster[i], "horror")) pindah(poster[i], A_horror, FileLog, "FilmHorror");
        
            else if (strstr(poster[i], "animasi")) pindah(poster[i], A_animasi, FileLog, "FilmAnimasi");

            else if (strstr(poster[i], "drama")) pindah(poster[i], A_drama, FileLog, "FilmDrama");
        }
    }
}


void hitung(char poster[][51]){
    FILE *total;
    int hor, ani, dr;
    hor = ani = dr = 0;

    for(int i = 0; i < idx; i++){
        if (strstr(poster[i], "horror")) hor++;
        else if (strstr(poster[i], "animasi")) ani++;
        else if (strstr(poster[i], "drama")) dr++;
    }

    total = fopen("total.txt", "w");
    // if(total!=NULL){
        fprintf(total, "Jumlah film horror: %d\n", hor);
        fprintf(total, "Jumlah film animasi: %d\n", ani);
        fprintf(total, "Jumlah film drama: %d\n", dr);

    if (hor > ani && hor > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Horror\n");

    else if (ani > hor && ani > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Animasi\n");

    else fprintf(total, "Genre dengan jumlah film terbanyak: Drama\n");

    fclose(total);
}

int main(){
    FILE *FileLog;
    struct stat st = {0};

    char poster[51][51];

    if (stat(A_horror, &st) == -1) mkdir(A_horror, 0777);
    if (stat(A_animasi, &st) == -1) mkdir(A_animasi, 0777);
    if (stat(A_drama, &st) == -1) mkdir(A_drama, 0777);

   FileLog = fopen("recap.txt", "w");

    memilah(FileLog, poster);
    hitung(poster);

    fclose(FileLog);

    return 0;
} 
