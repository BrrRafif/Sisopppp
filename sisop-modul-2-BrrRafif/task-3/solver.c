#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>

#define MAX_MANHWA 4
#define MAX_BUFFER 4096
#define MAX_PATH 1024
#define MAX_FILENAME 256
#define MAX_URLS 12  

typedef struct {
    char *title;
    char *heroine_name;
    int id;
    char *image_urls[MAX_URLS]; 
} ManhwaInfo;

typedef struct {
    char *manhwa_name;
    char *heroine_name;
    int num_images;
    char **image_urls;
} DownloadTask;

ManhwaInfo manhwa_list[MAX_MANHWA] = {
    { // Lia
        "Mistaken as the Monster Duke's Wife", 
        "Lia", 
        168827,
        {
            "https://i.pinimg.com/736x/cc/88/fc/cc88fc7520e11783dccbf081dcf5293d.jpg",
            "https://i.pinimg.com/736x/79/97/55/799755d95242f86e1930a0979d231d1c.jpg",
            "https://i.pinimg.com/736x/11/d9/ad/11d9ad85a47892f5fd979a0209162377.jpg",
            NULL
        }
    },
    { // Tia
        "The Villainess Lives Again", 
        "Tia", 
        147205,
        {
            "https://i.pinimg.com/736x/f4/02/2c/f4022c1333fcd4f9bc015433a26e9a21.jpg",
            "https://i.pinimg.com/736x/99/55/65/995565e0dd16be8dae7b5c506d209dac.jpg",
            "https://i.pinimg.com/736x/ab/19/29/ab1929f0cef80d54e97694d5b58ffea7.jpg",
            "https://i.pinimg.com/736x/b2/81/77/b28177734bc58c15067b1552963341fd.jpg",
            "https://i.pinimg.com/736x/2f/00/28/2f00281e41a96c23c3eae4522cc89a11.jpg",
            "https://i.pinimg.com/736x/40/f5/b8/40f5b8585bc68ac954963f8a472f5f09.jpg",
            NULL
        }
    },
    { // Adelia
        "No, I Only Charmed the Princess!", 
        "Adelia", 
        169731,
        {
            "https://i.pinimg.com/736x/43/d6/72/43d672d7e05e96a234071beb02ee4d58.jpg",
            "https://i.pinimg.com/736x/ec/0a/2b/ec0a2b4b0c3f048fa39dfd55fd2fede5.jpg",
            "https://i.pinimg.com/736x/ca/84/db/ca84dbfa7338ba818a825d18ee42c888.jpg",
            "https://i.pinimg.com/736x/fd/26/a7/fd26a75cbc439e66ed6b55bbd5c904f2.jpg",
            NULL
        }
    },
    { // Lizen
        "Darling, Why Can't We Divorce?", 
        "Lizen", 
        175521,
        {
            "https://i.pinimg.com/736x/6f/b1/bc/6fb1bc43cb83e70c2627a4405300b7ae.jpg",
            "https://i.pinimg.com/736x/15/b3/85/15b38534535d5e5264859b93cfaf8816.jpg",
            "https://i.pinimg.com/736x/5b/b6/a3/5bb6a33be21af50cf219fb52c863e95c.jpg",
            "https://i.pinimg.com/736x/7a/dd/2e/7add2ea8ef29c248a542e1540f6e249d.jpg",
            "https://i.pinimg.com/736x/a9/87/d3/a987d37f002da2f9a5af471d857743c7.jpg",
            "https://i.pinimg.com/736x/2c/b3/9f/2cb39f82aa061c3f9bc29624498124c2.jpg",
            "https://i.pinimg.com/736x/75/ef/ac/75eface4fd788f17e3f5f02c1102f9be.jpg",
            "https://i.pinimg.com/736x/9f/50/e4/9f50e4e09644cfd77df3436e95683d0e.jpg",
            "https://i.pinimg.com/736x/93/cb/0d/93cb0dadacbe1dd060dc613ffc95c060.jpg",
            "https://i.pinimg.com/736x/3a/b2/a2/3ab2a24854b7c48df698782c6c017f26.jpg",
            NULL
        }
    }
};


void format_filename(char *str) {
    char temp[MAX_FILENAME] = {0};
    int j = 0;
    for (int i = 0; str[i] && j < MAX_FILENAME - 1; i++) {
        if (str[i] == ' ') {
            temp[j++] = '_';
        } else if (isalnum(str[i]) || str[i] == '_') {
            temp[j++] = str[i];
        }
    }
    temp[j] = '\0';
    strncpy(str, temp, MAX_FILENAME);
}

void get_uppercase_manhwa_name(const char *manhwa_name, char *result) {
    char temp[MAX_FILENAME];
    strncpy(temp, manhwa_name, MAX_FILENAME - 1);
    temp[MAX_FILENAME - 1] = '\0';
    format_filename(temp);
    int j = 0;
    for (int i = 0; temp[i] && j < MAX_FILENAME - 1; i++) {
        if (isupper(temp[i])) {
            result[j++] = temp[i];
        }
    }
    result[j] = '\0';
}


void create_dir(const char *path) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "-p", path, NULL);
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

void* download_images(void *arg) {
    DownloadTask task = (DownloadTask)arg;
    char dirpath[MAX_PATH];
    snprintf(dirpath, MAX_PATH, "Heroines/%s", task->heroine_name);
    create_dir(dirpath);
    
    printf("Downloading %d images for %s\n", task->num_images, task->heroine_name);
    
    for (int i = 0; i < task->num_images && task->image_urls[i] != NULL; i++) {
        char filename[MAX_PATH];
        snprintf(filename, MAX_PATH, "%s/%s_%d.jpg", 
                dirpath, task->heroine_name, i+1);
        pid_t pid = fork();
        if (pid == 0) {

            execlp("wget", "wget", "-q", "-O", filename, task->image_urls[i], NULL);
            exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Downloaded %s\n", filename);
            } else {
                fprintf(stderr, "Failed to download %s\n", task->image_urls[i]);
            }
        }
        sleep(1);
    }
    return NULL;
}

void process_image_downloads() {
    create_dir("Heroines");
    pthread_t threads[MAX_MANHWA];
    DownloadTask tasks[MAX_MANHWA];
    for (int i = 0; i < MAX_MANHWA; i++) {
        tasks[i].manhwa_name = manhwa_list[i].title;
        tasks[i].heroine_name = manhwa_list[i].heroine_name;
        tasks[i].image_urls = manhwa_list[i].image_urls;

        int count = 0;
        while (count < MAX_URLS && manhwa_list[i].image_urls[count] != NULL) {
            count++;
        }
        tasks[i].num_images = count;
        pthread_create(&threads[i], NULL, download_images, &tasks[i]);
    }
    for (int i = 0; i < MAX_MANHWA; i++) {
        pthread_join(threads[i], NULL);
    }
}

void fetch_manhwa_data(int id, const char *title) {
    char filename[MAX_FILENAME];
    strncpy(filename, title, MAX_FILENAME - 1);
    filename[MAX_FILENAME - 1] = '\0';
    format_filename(filename);
    
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "Manhwa/%s.txt", filename);
    
    pid_t pid = fork();
    if (pid == 0) {
        char api_url[MAX_PATH];
        snprintf(api_url, MAX_PATH, "https://api.jikan.moe/v4/manga/%d", id);
        
        char command[MAX_BUFFER];
        snprintf(command, MAX_BUFFER, 
                "curl -s \"%s\" | jq -r '["
                ".data.title // \"N/A\", "
                ".data.status // \"N/A\", "
                "(.data.published.from // \"N/A\" | split(\"T\")[0]), "
                "((.data.genres // []) | map(.name) | join(\", \") // \"N/A\"), "
                "((.data.themes // []) | map(.name) | join(\", \") // \"N/A\"), "
                "((.data.authors // []) | map(.name) | join(\", \") // \"N/A\")] | "
                "join(\"\\n\")' > \"%s\"", 
                api_url, path);
        execlp("sh", "sh", "-c", command, NULL);
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        FILE *file = fopen(path, "r+");
        if (file) {
            char content[MAX_BUFFER] = {0};
            size_t bytes_read = fread(content, 1, MAX_BUFFER - 1, file);
            content[bytes_read] = '\0';
            char *lines[6] = {0};
            char *token = strtok(content, "\n");
            int i = 0;
            while (token && i < 6) {
                lines[i++] = token;
                token = strtok(NULL, "\n");
            }
            rewind(file);
            fprintf(file, "Title: %s\n", lines[0] ? lines[0] : "N/A");
            fprintf(file, "Status: %s\n", lines[1] ? lines[1] : "N/A");
            fprintf(file, "Release: %s\n", lines[2] ? lines[2] : "N/A");
            fprintf(file, "Genre: %s\n", lines[3] ? lines[3] : "N/A");
            fprintf(file, "Theme: %s\n", lines[4] ? lines[4] : "N/A");
            fprintf(file, "Author: %s\n", lines[5] ? lines[5] : "N/A");
            fclose(file);
        }
    }
}

void zip_text_files() {
    create_dir("Archive");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("Manhwa")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                char filename[MAX_FILENAME];
                strncpy(filename, ent->d_name, MAX_FILENAME - 1);
                filename[MAX_FILENAME - 1] = '\0';
                char *dot = strrchr(filename, '.');
                if (dot) *dot = '\0';
                char uppercase[MAX_FILENAME];
                get_uppercase_manhwa_name(filename, uppercase);
                char src_path[MAX_PATH];
                snprintf(src_path, MAX_PATH, "Manhwa/%s", ent->d_name);
                char dest_path[MAX_PATH];
                snprintf(dest_path, MAX_PATH, "Archive/%s.zip", uppercase);
                pid_t pid = fork();
                if (pid == 0) {
                    execlp("zip", "zip", "-j", dest_path, src_path, NULL);
                    exit(EXIT_FAILURE);
                } else {
                    wait(NULL);
                }
            }
        }
        closedir(dir);
    }
}


int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf)) {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}

void archive_and_clean_heroine_images() {
    create_dir("Archive/Images");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("Heroines")) != NULL) {
        char *heroine_dirs[MAX_MANHWA];
        int count = 0;
        while ((ent = readdir(dir)) != NULL && count < MAX_MANHWA) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                char path[MAX_PATH];
                snprintf(path, MAX_PATH, "Heroines/%s", ent->d_name);
                if (is_directory(path)) {
                    heroine_dirs[count] = strdup(ent->d_name);
                    if (heroine_dirs[count]) count++;
                }
            }
        }
        
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(heroine_dirs[i], heroine_dirs[j]) > 0) {
                    char *temp = heroine_dirs[i];
                    heroine_dirs[i] = heroine_dirs[j];
                    heroine_dirs[j] = temp;
                }
            }
        }
        
        for (int i = 0; i < count; i++) {
            char uppercase[MAX_FILENAME];
            get_uppercase_manhwa_name(manhwa_list[i].title, uppercase);
            char zip_path[MAX_PATH];
            snprintf(zip_path, MAX_PATH, "Archive/Images/%s_%s.zip", uppercase, heroine_dirs[i]);
            char src_path[MAX_PATH];
            snprintf(src_path, MAX_PATH, "Heroines/%s", heroine_dirs[i]);
            pid_t pid = fork();
            if (pid == 0) {
                execlp("zip", "zip", "-r", zip_path, src_path, NULL);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
            pid = fork();
            if (pid == 0) {
                execlp("rm", "rm", "-rf", src_path, NULL);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
            
            free(heroine_dirs[i]);
        }
        
        closedir(dir);
    }
}

int main() {
    create_dir("Manhwa");
    for (int i = 0; i < MAX_MANHWA; i++) {
        fetch_manhwa_data(manhwa_list[i].id, manhwa_list[i].title);
    }
    
    zip_text_files();
    
    process_image_downloads();
    
    archive_and_clean_heroine_images();
    
    printf("All tasks completed successfully!\n");
    return 0;
}
