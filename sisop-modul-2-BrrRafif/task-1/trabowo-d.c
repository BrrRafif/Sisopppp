#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *A_horror = "/home/berwyn/Sistem Operasi/film/FilmHorror";
    char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
    char *A_drama = "/home/berwyn/Sistem Operasi/film/FilmDrama";
    char *output_f = "/home/berwyn/Sistem Operasi/film/SortirFilm.zip"; 
    
    pid_t child;
    child = fork();

    if (child == 0) {
        execlp("/usr/bin/zip", "zip", "-r", output_f, A_horror, A_animasi, A_drama, NULL);
    }
    
    else {
        int status;
        wait(&status);

        if (WIFEXITED(status)){
            pid_t rid = fork();

            if (rid == 0) execlp("/usr/bin/rm", "rm", "-r", A_horror, A_animasi, A_drama, NULL);
            
            else {
                wait(&status);
                
                if (WIFEXITED(status))printf("\n Berhasil mengarsipkan ketiga folder dan menghapusnya.\n"); 
            }
        } 
    }
    return 0;
}
