#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>

#define ZIP_NAME "download.zip"
#define OUTPUT_DIR "output"
#define LINK "https://drive.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J"
#define CSV "netflixData.csv"
#define MAX 1024

void* download_file(void* argv) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("wget", "wget", "-O", ZIP_NAME, LINK, NULL);
        perror("execlp wget");
        exit(EXIT_FAILURE);
    } 
    
    else {
        wait(NULL);
    }

    return NULL;
}

void* extract_file(void* argv) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", ZIP_NAME, "-d", OUTPUT_DIR, NULL);
        perror("execlp unzip");
        exit(EXIT_FAILURE);
    } 
    
    else {
        wait(NULL);

        pid_t mv_pid = fork();
        if (mv_pid == 0) {
            execlp("sh", "sh", "-c", "cp output/*.csv netflixData.csv", NULL);
            perror("execlp cp");
            exit(EXIT_FAILURE);
        } 

        else {
            wait(NULL);
        }
    }
    return NULL;
}

void* delete_zip(void* argv) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("rm", "rm", ZIP_NAME, NULL);
        perror("execlp rm");
        exit(EXIT_FAILURE);
    } 
    
    else {
        wait(NULL);
    }
    
    return NULL;
}

void write_log(const char *kategori, const char *judul) {
    FILE *log = fopen("log.txt", "a");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(log, "[%02d:%02d:%02d] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
        t->tm_hour, t->tm_min, t->tm_sec, kategori, judul);
    fclose(log);
}

void parse(char *line, char **title, char **director, char **country, char **year) {
    static char *fields[4] = {NULL};
    int field = 0, in_quotes = 0;
    char *ptr = line, *start = line;

    while (*ptr && field < 4) {
        if (*ptr == '"') {
            in_quotes = !in_quotes;
        } else if (*ptr == ',' && !in_quotes) {
            *ptr = '\0';
            fields[field++] = start;
            start = ptr + 1;
        }
        ptr++;
    }

    if (field == 3) {
        fields[3] = start;
        fields[3][strcspn(fields[3], "\r\n")] = '\0';
    }

    *title = fields[0];
    *director = fields[1];
    *country = fields[2];
    *year = fields[3];
}

void process_by_abjad() {
    mkdir("judul", 0755);

    FILE *file = fopen(CSV, "r");

    char line[MAX];
    fgets(line, MAX, file); 

    while (fgets(line, MAX, file)) {
        char *title = NULL, *director = NULL, *country = NULL, *year = NULL;
        parse(line, &title, &director, &country, &year);

        if (!title || !director || !year) continue;

        write_log("Abjad", title);

        unsigned char first = title[0];
        char filename[100];
        if ((first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') || (first >= '0' && first <= '9')) {
            snprintf(filename, sizeof(filename), "judul/%c.txt", toupper(first));
        } else {
            snprintf(filename, sizeof(filename), "judul/#.txt");
        }

        FILE *out = fopen(filename, "a");
        if (out) {
            fprintf(out, "%s - %s - %s\n", title, year, director);
            fclose(out);
        }
    }

    fclose(file);
    exit(0);
}

void process_by_year() {
    mkdir("tahun", 0755);

    FILE *file = fopen(CSV, "r");

    char line[MAX];
    fgets(line, MAX, file); 

    while (fgets(line, MAX, file)) {
        char *title = NULL, *director = NULL, *country = NULL, *year = NULL;
        parse(line, &title, &director, &country, &year);

        if (!title || !director || !year) continue;
        if (strlen(year) != 4) continue;

        int valid = 1;
        for (int i = 0; i < 4; i++) {
            if (!isdigit(year[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) continue;

        write_log("Tahun", title);

        char filename[100];
        snprintf(filename, sizeof(filename), "tahun/%s.txt", year);

        FILE *outf = fopen(filename, "a");
        if (outf) {
            fprintf(outf, "%s - %s - %s\n", title, year, director);
            fclose(outf);
        }
    }

    fclose(file);
    exit(0);
}

void generate_report() {
    FILE *file = fopen(CSV, "r");

    typedef struct {
        char country[100];
        int before_2000;
        int after_2000;
    } CountryStat;

    CountryStat stats[500];
    int count = 0;

    char line[MAX];
    fgets(line, MAX, file); 

    while (fgets(line, MAX, file)) {
        char *title = NULL, *director = NULL, *country = NULL, *year_str = NULL;
        parse(line, &title, &director, &country, &year_str);

        if (!title || !director || !country || !year_str) continue;


        int year = atoi(year_str);

        if (year <= 0) continue;

        if(strlen(country)==0) continue;

        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(stats[i].country, country) == 0) {
                if (year < 2000) stats[i].before_2000++;
                else stats[i].after_2000++;
                found = 1;
                break;
            }
        }

        if (!found && count < 500) {
            strncpy(stats[count].country, country, sizeof(stats[count].country) - 1);
            stats[count].before_2000 = (year < 2000) ? 1 : 0;
            stats[count].after_2000 = (year >= 2000) ? 1 : 0;
            count++;
        }
    }

    fclose(file);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[100];
    snprintf(filename, sizeof(filename), "report_%02d%02d%04d.txt", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    FILE *report = fopen(filename, "w");

    for (int i = 0; i < count; i++) {
        fprintf(report, "%d. Negara: %s\n", i + 1, stats[i].country);
        fprintf(report, "Film sebelum 2000: %d\n", stats[i].before_2000);
        fprintf(report, "Film setelah 2000: %d\n\n", stats[i].after_2000);
    }

    fclose(report);
}

void group_movies() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        process_by_abjad();
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        process_by_year();
    }

    wait(NULL);
    wait(NULL);
}

int main() {
    int n;
    pthread_t thread1, thread2, thread3;

    while (1) {
        printf("1. Download File\n");
        printf("2. Mengelompokkan Film\n");
        printf("3. Membuat Report\n");

        scanf("%d", &n);

        switch (n) {
            case 1:
                pthread_create(&thread1, NULL, download_file, NULL);
                pthread_join(thread1, NULL);
                pthread_create(&thread2, NULL, extract_file, NULL);
                pthread_join(thread2, NULL);
                pthread_create(&thread3, NULL, delete_zip, NULL);
                pthread_join(thread3, NULL);
                break;
            case 2:
                group_movies();
                break;
            case 3:
                generate_report();
                break;
            default:
                printf("Opsi tidak valid. Silakan pilih lagi.\n");
        }
    }

    return 0;
}
