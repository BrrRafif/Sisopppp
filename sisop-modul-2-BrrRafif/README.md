[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9LcL5VTQ)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025221000 | Student 1 Name |
| 5025221000 | Student 2 Name |
| 5025221000 | Student 3 Name |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Trabowo & Peddy Movie Night](/task-1/)

- [Task 2 - Organize and Analyze Anthony's Favorite Films](/task-2/)

- [Task 3 - Cella’s Manhwa](/task-3/)

- [Task 4 - Pipip's Load Balancer](/task-4/)

### Laporan Resmi Praktikum Modul 2 _(Module 2 Lab Work Report)_

Tulis laporan resmi di sini!

_Write your lab work report here!_

## Task 2 (Reza)

#### Kode

```c
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
```

### **a. One Click and Done!**

Pada soal ini kita ditugaskan untuk mendownload zip dari link google drive yang sudah diberikan, melakukan extract terhadap zip, dan melakukan penghapusan file zip.

1. Melakukan download terhadap file zip.

```c

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

```

- **pid_t pid = fork()** -> Membuat proses baru. Ada dua proses setelah ini yaitu, parent dan child.
- **execlp("wget", "wget", "-O", ZIP_NAME, LINK, NULL)** -> Menjalankan perintah wget untuk mengunduh file dari internet dan menyimpannya sebagai download.zip.
- **perror** -> Mencetak pesan error apabila wget gagal dijalankan.
- **exit** -> Jika ad error maka keluar dari prosess child.

2. Melakukan extract terhadap file zip.

```c

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

```

- **pid_t pid = fork()** -> Membuat proses baru. Ada dua proses setelah ini yaitu, parent dan child.
- **execlp("unzip", "unzip", ZIP_NAME, "-d", OUTPUT_DIR, NULL);** -> Menjalankan perintah unzip untuk mengekstrak file download.zip ke folder.
- **perror** -> Mencetak pesan error apabila wget gagal dijalankan.
- **exit** -> Jika ad error maka keluar dari prosess child.
- **pid_t mv_pid = fork();** -> Setelah unzip selesai, buat proses baru lagi untuk menyalin file CSV.
- **execlp("sh", "sh", "-c", "cp output/\*.csv netflixData.csv", NULL)** -> Menyalin file CSV dari folder output dan mengganti namanya menjadi netflixData.csv.
- **perror("execlp cp")** -> Kalau perintah copy gagal dijalankan, cetak pesan error.

3. Melakukan penghapusan terhadap file zip.

```c

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

```

- **execlp("rm", "rm", ZIP_NAME, NULL);** -> Menjalankan perintah rm untuk menghapus file download.zip.
- **perror("execlp rm")** -> Mencetak error jika perintah rm gagal dijalankan.

4. Isi file netflixData.csv.

![alt text](<Screenshot from 2025-04-28 22-05-33.png>)

### **b. Sorting Like a Pro**

Pada soal ini kita diminta untuk melakukan pensortingan terhadap huruf pertama dari judul film dan berdasarkan tahun rilis, setelah itu kita mencatat semua proses yang terjadi kedalam file bernama **log.txt**.

1. Membuat fungsi parse yang akan digunakan saat melakukan sorting nanti, parse disini berfungsi saat kita membaca data pada file csv dan ingin membagi-bagi nya menjadi beberapa bagian.

```c

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

```

- **static char \*fields[4]** -> Membuat array untuk menyimpan 4 bagian data hasil pecahan CSV.
- **int field = 0, in_quotes = 0** -> field untuk menghitung sudah berapa bagian yang dipisah, in_quotes untuk mengecek apakah kita sedang di dalam tanda kutip.
- **char \*ptr = line, \*start = line** -> ptr untuk berjalan satu per satu karakter, start menandai awal dari setiap bagian data.
- **in_quotes = !in_quotes;** -> Mengubah status apabila sedang berada di tanda **"**.
- **\*ptr == ',' && !in_quotes** -> Menemukan tanda **,** yang berarti kita sudah berada pada batas antar data.
- **fields[field++] = start** -> Simpan bagian data yang sudah dipisahkan ke dalam array fields, lalu pindah ke field berikutnya.
- **start = ptr + 1;** -> Set start ke karakter setelah koma untuk bagian data berikutnya.
- **if (field == 3) { fields[3] = start; fields[3]strcspn(fields[3], "\r\n")] = '\0'; }** -> Setelah dapat 3 bagian (title, director, country), bagian ke-4 (year) langsung diambil dari sisa string.
- **\*title = fields[0]** -> Simpan hasil bagian pertama sebagai title.
- **\*director = fields[1]** -> Simpan hasil bagian kedua sebagai director.
- **\*country = fields[2]** -> Simpan hasil bagian ketiga sebagai country.
- **\*year = fields[3];** -> Simpan hasil bagian keempat sebagai year.

2. Membuat log untuk mencatat seluruh proses pada saat pensortingan.

```c

void write_log(const char *kategori, const char *judul) {
    FILE *log = fopen("log.txt", "a");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(log, "[%02d:%02d:%02d] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
        t->tm_hour, t->tm_min, t->tm_sec, kategori, judul);
    fclose(log);
}

```

- **FILE \*log = fopen("log.txt", "a")** -> Membuka file log.txt dengan mode append (a), supaya isi lama tidak terhapus dan bisa menambah data baru di bawahnya.
- **time_t now = time(NULL)** -> Mengambil waktu sekarang dalam format detik.
- **struct tm \*t = localtime(&now)** -> Mengubah waktu now menjadi format waktu lokal (jam, menit, detik).
- **fprintf(log, "[%02d:%02d:%02d] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",t->tm_hour, t->tm_min, t->tm_sec, kategori, judul)** -> Mengelompokkan berdasarkan kategori.
- **fclose(log)** -> Menutup file **log.txt**.

3. Membuat fungsi sorting berdasarkan huruf awal pada judul.

```c

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

```

- **mkdir("judul", 0755)** -> Membuat folder baru bernama judul dengan permission 0755 (bisa dibaca semua orang, hanya pemilik yang bisa edit).
- **FILE \*file = fopen(CSV, "r")** -> Membuka file CSV (netflixData.csv) untuk dibaca.
- **char line[MAX];** -> Menyediakan tempat untuk menyimpan satu baris dari file CSV.
- **fgets(line, MAX, file)** -> Membaca satu baris pertama (biasanya header) lalu dilewati.
- **char *title = NULL, *director = NULL, *country = NULL, *year = NULL** -> Menyiapkan variabel untuk menyimpan hasil parsing (judul, sutradara, negara, tahun).
- **parse(line, &title, &director, &country, &year)** -> Memanggil fungsi parse untuk memisahkan isi baris menjadi 4 bagian.
- **if (!title || !director || !year) continue** -> Jika ada data penting yang kosong (judul, sutradara, atau tahun), lompat ke baris berikutnya.
- **write_log("Abjad", title)** -> Menulis aktivitas ke file log, memberi tanda bahwa judul ini sedang diproses.
- **unsigned char first = title[0]** -> Mengambil huruf pertama dari judul.
- **char filename[100]** -> Menyiapkan variabel untuk menyimpan nama file tujuan.
- **if ((first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') || (first >= '0' && first <= '9'))** -> Mengecek apakah huruf pertama adalah huruf alfabet atau angka.
- **snprintf(filename, sizeof(filename), "judul/%c.txt", toupper(first))** -> Membuat nama file berdasarkan huruf besar dari karakter pertama.
- **snprintf(filename, sizeof(filename), "judul/#.txt")** -> Jika bukan huruf atau angka, simpan ke file #.txt.
- **FILE \*out = fopen(filename, "a")** -> Membuka file tujuan (atau membuat baru jika belum ada) untuk ditambahkan isinya.
- **fprintf(out, "%s - %s - %s\n", title, year, director)** -> Tulis ke dalam file: judul, tahun, dan sutradara.
- **fclose(out)** -> Tutup file setelah selesai menulis.
- **fclose(file)** -> Setelah semua data diproses, tutup file CSV.

4. Membuat fungsi sorting berdasarkan tahun rilis.

```c

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

```

- **mkdir("tahun", 0755)** -> Membuat folder baru bernama tahun dengan permission 0755 (bisa dibaca semua, tapi hanya owner yang bisa edit).
- **FILE \*file = fopen(CSV, "r")** -> Membuka file CSV (netflixData.csv) untuk dibaca.
- **char line[MAX];** -> Menyediakan tempat untuk menyimpan satu baris dari file CSV.
- **fgets(line, MAX, file)** -> Membaca satu baris pertama (biasanya header) lalu dilewati.
- **char *title = NULL, *director = NULL, *country = NULL, *year = NULL** -> Menyiapkan variabel untuk menyimpan hasil parsing (judul, sutradara, negara, tahun).
- **parse(line, &title, &director, &country, &year)** -> Memanggil fungsi parse untuk memisahkan isi baris menjadi 4 bagian.
- **if (!title || !director || !year) continue** -> Jika ada data penting yang kosong (judul, sutradara, atau tahun), lompat ke baris berikutnya.
- **if (strlen(year) != 4) continue** -> Jika tidak ada tahun rilis maka data tidak akan diproses.
- **int valid = 1** -> Menyiapkan flag untuk memeriksa apakah tahun hanya terdiri dari angka.
- **for (int i = 0; i < 4; i++) { if (!isdigit(year[i])) { valid = 0; break; } }** -> Mengecek setiap karakter di tahun, harus berupa angka semua. Kalau ada yang bukan angka, tandai tidak valid.
- **if (!valid) continue** -> Jika tidak valid maka tidak lanjut di proses.
- **write_log("Tahun", title)** -> Mencatat ke file log bahwa sedang mengelompokkan film berdasarkan tahun.
- **char filename[100]** -> Menyiapkan variabel untuk menyimpan nama file tujuan.
- **snprintf(filename, sizeof(filename), "tahun/%s.txt", year)** -> Membuat nama file output berdasarkan tahun.
- **\*FILE outf = fopen(filename, "a")** -> Membuka file tahun tersebut dalam mode append (menambahkan data baru).
- **if (outf) { fprintf(outf, "%s - %s - %s\n", title, year, director); fclose(outf); }** -> Jika file berhasil dibuka, tulis data film dalam format: judul - tahun - sutradara, lalu tutup file.
- **fclose(file)** -> Menutup file CSV setelah semua baris dibaca.

5. Menjalankan sorting dengan memanggil fungsi sorting judul dan tahun yang sudah dibuat.

```c

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

```

- **pid_t pid1 = fork()** -> Membuat proses baru (child process) untuk mengelompokkan film berdasarkan huruf depan.
- **if (pid1 == 0) { process_by_abjad(); }** -> Jalankan fungsi process_by_abjad() (kelompokkan berdasarkan abjad).
- **pid_t pid2 = fork()** -> Membuat proses baru lagi untuk mengelompokkan film berdasarkan tahun.
- **if (pid2 == 0) { process_by_year(); }** -> Jika ini adalah proses anak kedua, jalankan fungsi process_by_year() (kelompokkan berdasarkan tahun).

6. Hasil dari sorting.

### **c. The Ultimate Movie Report**

Pada soal ini kita diminta untuk melakukan sorting film berdasarkan tahun rilis dan juga mengelompokkan film tersebut dengan format **`report_ddmmyyyy.txt`**. Membuat menu untuk melakukan download, pengelompokkan film, dan membuat report.

1. Membuat fungsi report untuk menampilkan data hasil sortingan berdasarkan asal negara dan juga tahun rilis. Membagi film yang dirilis setelah dan sebelum tahun 2000.

```c

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

```

- **FILE \*file = fopen(CSV, "r")** -> Membuka file CSV yang berisi data film untuk dibaca.
- **typedef struct { ... } CountryStat** -> Membuat tipe data baru CountryStat yang berisi nama negara dan jumlah film sebelum dan sesudah tahun 2000.
- **CountryStat stats[500]** -> Menyiapkan array stats untuk menyimpan data maksimal 500 negara.
- **int count = 0** -> Variabel untuk menghitung berapa banyak negara yang sudah tercatat.
- **char line[MAX]** -> Buffer untuk membaca setiap baris dari file CSV.
- **fgets(line, MAX, file)** -> Membaca dan melewati baris pertama (header CSV).
- **while (fgets(line, MAX, file)) { ... }** -> Membaca setiap baris film satu per satu.
- **char *title = NULL, *director = NULL, *country = NULL, *year_str = NULL** -> Menyiapkan pointer untuk menyimpan data film dari hasil parsing.
- **parse(line, &title, &director, &country, &year_str)** -> Memanggil fungsi parse untuk memecah satu baris CSV menjadi komponen-komponen.
- **if (!title || !director || !country || !year_str) continue** -> Jika ada data yang kosong, lewati baris ini.
- **int year = atoi(year_str)** -> Mengubah year_str (string) menjadi angka integer.
- **if(strlen(country)==0) continue** -> Jika negara kosong lewati.
- **int found = 0** -> Variabel untuk menandai apakah negara ini sudah pernah dicatat sebelumnya.
- **for (int i = 0; i < count; i++) { ... }** -> Cari apakah negara sudah ada di dalam array stats.
- **if (year < 2000) stats[i].before_2000++; else stats[i].after_2000++** -> Tambahkan jumlah film ke kategori sebelum atau sesudah tahun 2000.
- **found = 1; break** -> Tandai negara sudah ditemukan, keluar dari loop.
- **if (!found && count < 500) { ... }** -> Kalau negara belum ditemukan dan array belum penuh.
- **strncpy(stats[count].country, country, sizeof(stats[count].country) - 1)** -> Simpan nama negara.
- **fclose(file)** -> Tutup file CSV setelah semua data dibaca.
- **time_t now = time(NULL); struct tm t = localtime(&now)** -> Untuk mendapatkan waktu sekarang.
- **snprintf(filename, sizeof(filename), "report\_%02d%02d%04d.txt", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900)** -> Membuat nama file laporan berdasarkan tanggal hari ini (format: report_ddmmyyyy.txt).
- **FILE \*report = fopen(filename, "w")** -> Membuka file laporan untuk ditulis.
- **fprintf(report, "%d. Negara: %s\n", i + 1, stats[i].country)** -> Tulis nomor dan nama negara.
- **fprintf(report, "Film sebelum 2000: %d\n", stats[i].before_2000)** -> Tulis jumlah film sebelum tahun 2000.
- **printf(report, "Film setelah 2000: %d\n\n", stats[i].after_2000)** -> Tulis jumlah film setelah tahun 2000.
- **fclose(report)** -> Tutup file laporan setelah selesai menulis.

2. Membuat menu interaktif agar mudah untuk mengoperasikan program.

```c

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

```

- **case 1** -> Dibuat untuk melakukan download file.
- **case 2** -> Dibuat untuk mengelompokkan film.
- **case 3** -> Dibuat untuk membuat report.

3. Hasil dari sorting.
