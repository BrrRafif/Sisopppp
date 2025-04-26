#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    char *zip = "/home/berwyn/Sistem Operasi/film.zip";
    char *output_f = "/home/berwyn/Sistem Operasi";
    char *url = "https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B";
    int scc = 0;

    if (pid == 0) execlp("wget", "wget", "-O", zip, url, NULL);

    else {
        int status;
        wait(&status);

        if (WIFEXITED(status)){
            scc = 1;
            char *argv[] = {"unzip", zip, "-d", output_f, NULL};
            execv("/usr/bin/unzip", argv);
        } 
        
        if (scc == 1) printf("\nBerhasil mengekstrak file zip\n");
        
        else if (scc == 0) printf("\nGagal mengekstrak file zip\n");
    }

    return 0;
}
