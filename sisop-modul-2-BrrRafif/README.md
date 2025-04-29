[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9LcL5VTQ)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025221000 | Student 1 Name |
| 5025221000 | Student 2 Name |
| 5025241037 | Agil Lukman Hakim Muchdi |

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

## Task 3 (Agil)
#### Kode

```c
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

typedef struct {
    char *title;
    char *heroine_name;
    int id;
    char *image_urls;
    int release_month; 
} ManhwaInfo;

typedef struct {
    char *manhwa_name;
    char *heroine_name;
    int id;
    char *image_url;
    int num_images;
} DownloadTask;

ManhwaInfo manhwa_list[MAX_MANHWA] = {
    { // Lia
        .title = "Mistaken as the Monster Duke's Wife",
        .heroine_name = "Lia",
        .id = 168827,
        .image_urls = "https://i.pinimg.com/736x/cc/88/fc/cc88fc7520e11783dccbf081dcf5293d.jpg",
        .release_month = 0
    },
    { // Tia
        .title = "The Villainess Lives Again", 
        .heroine_name = "Tia", 
        .id = 147205,
        .image_urls = "https://i.pinimg.com/736x/f4/02/2c/f4022c1333fcd4f9bc015433a26e9a21.jpg",
        .release_month = 0
    },
    { // Adelia
        .title = "No, I Only Charmed the Princess!", 
        .heroine_name = "Adelia", 
        .id = 169731,
        .image_urls = "https://i.pinimg.com/736x/43/d6/72/43d672d7e05e96a234071beb02ee4d58.jpg",
        .release_month = 0
    },
    { // Lizen
        .title = "Darling, Why Can't We Divorce?", 
        .heroine_name = "Lizen", 
        .id = 175521,
        .image_urls = "https://i.pinimg.com/736x/6f/b1/bc/6fb1bc43cb83e70c2627a4405300b7ae.jpg",
        .release_month = 0
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


// Fungsi untuk ekstrak bulan dari tanggal
int extract_month_from_date(const char *date_str) {
    if (!date_str || strcmp(date_str, "N/A") == 0) {
        printf("WARNING: Invalid date string\n");
        return 1;
    }

    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) == 3) {
        printf("DEBUG: Parsed date: %d-%02d-%02d\n", year, month, day);
        return (month >= 1 && month <= 12) ? month : 1;
    }

    printf("WARNING: Failed to parse date: %s\n", date_str);
    return 1;
}

// Fungsi untuk mengambil bulan rilis dari API
int fetch_release_month(int id) {
    char temp_file[] = "/tmp/manhwa_temp_XXXXXX";
    int fd = mkstemp(temp_file);
    if (fd == -1) {
        perror("Failed to create temp file");
        return 1; // Default value
    }
    close(fd);

    char api_url[MAX_PATH];
    snprintf(api_url, sizeof(api_url), "https://api.jikan.moe/v4/manga/%d", id);

    pid_t pid = fork();
    if (pid == 0) {
        // Gunakan curl dengan timeout
        execlp("sh", "sh", "-c", 
               "curl -s --max-time 30 \"$0\" | jq -r '.data.published.from // \"N/A\" | split(\"T\")[0]' > \"$1\"", 
               api_url, temp_file);
        exit(EXIT_FAILURE);
    }

    waitpid(pid, NULL, 0); // Tunggu hingga selesai

    // Baca file dengan error handling
    FILE *file = fopen(temp_file, "r");
    if (!file) {
        remove(temp_file);
        return 1;
    }

    char date_str[20] = "N/A";
    fgets(date_str, sizeof(date_str), file);
    fclose(file);
    remove(temp_file);

    // Debugging: Print tanggal yang didapat
    printf("DEBUG: Raw date for ID %d: %s\n", id, date_str);

    return extract_month_from_date(date_str);
}


void* download_images(void *arg) {
    DownloadTask *task = (DownloadTask*)arg;
    // Dapatkan bulan rilis SEKALI di awal thread
    static pthread_mutex_t api_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&api_mutex);
    int release_month = fetch_release_month(task->id);
    pthread_mutex_unlock(&api_mutex);

    release_month = (release_month < 1 || release_month > 12) ? 1 : release_month;
    printf("CONFIRMED: %s release month = %d\n", task->heroine_name, release_month);

    // Buat direktori heroine
    char dirpath[MAX_PATH];
    snprintf(dirpath, sizeof(dirpath), "Heroines/%s", task->heroine_name);
    create_dir(dirpath);

    printf("Downloading %d copies of %s for %s\n", 
           release_month, task->image_url, task->heroine_name);

    // Download satu URL berkali-kali
    for (int i = 0; i < release_month; i++) {
        char filename[MAX_PATH];
        snprintf(filename, sizeof(filename), "%s/%s_%d.jpg",
                dirpath, task->heroine_name, i+1);

        pid_t pid = fork();
        if (pid == 0) { // Child process
            // Gunakan wget dengan timeout 30 detik
            execlp("wget", "wget", "-q", "--timeout=30", "-O", filename, task->image_url, NULL);
            exit(EXIT_FAILURE);
        } else { // Parent
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    printf("Success: %s\n", filename);
                } else {
                    fprintf(stderr, "Failed to download copy %d (Status: %d)\n",
                           i+1, WEXITSTATUS(status));
                }
            }
        }
        sleep(1); // Delay antara download
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
        tasks[i].id = manhwa_list[i].id;
        tasks[i].image_url = manhwa_list[i].image_urls;
        pthread_create(&threads[i], NULL, download_images, &tasks[i]);
    }
    
    for (int i = 0; i < MAX_MANHWA; i++) {
        pthread_join(threads[i], NULL);
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
        // Sort alphabetically
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(heroine_dirs[i], heroine_dirs[j]) > 0) {
                    char *temp = heroine_dirs[i];
                    heroine_dirs[i] = heroine_dirs[j];
                    heroine_dirs[j] = temp;
                }
            }
        }
        // Archive each directory
        for (int i = 0; i < count; i++) {
            // Find matching manhwa info
            ManhwaInfo *info = NULL;
            for (int j = 0; j < MAX_MANHWA; j++) {
                if (strcmp(manhwa_list[j].heroine_name, heroine_dirs[i]) == 0) {
                    info = &manhwa_list[j];
                    break;
                }
            }
            if (info) {
                char uppercase[MAX_FILENAME];
                get_uppercase_manhwa_name(info->title, uppercase);
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
            }   
            free(heroine_dirs[i]);
        }
        closedir(dir);
    }
}

int main() {
    // Part a: Fetch manhwa data
    create_dir("Manhwa");
    for (int i = 0; i < MAX_MANHWA; i++) {
        fetch_manhwa_data(manhwa_list[i].id, manhwa_list[i].title);
    }    
    // Part b: Zip text files
    zip_text_files();
    // Part c: Download images from provided URLs
    process_image_downloads();
    // Part d: Archive and clean
    archive_and_clean_heroine_images();
    printf("All tasks completed successfully!\n");
    return 0;
}
```

### A. Summoning the Manhwa Stats
Pada kode ini akan dilakukan pemisahan beberapa data terkait title, status, release, genre, theme, dan author. Dan menyimpannya dalam file teks dengan nama file berupa judul versi bahasa Inggris. 
1. Untuk langkah awal kita buat dulu beberapa inisiasi agar lebih mudah nantinya dan struct untuk menyimpan data awal dari manhwa serta struct untuk task download nantinya

```c
#define MAX_MANHWA 4 // Total manhwa
#define MAX_BUFFER 4096 // Ukuran buffer maksimal
#define MAX_PATH 1024 // Panjang maksimal path
#define MAX_FILENAME 256 // Panjang maksimal untuk nama file

typedef struct {
    char *title;
    char *heroine_name;
    int id;
    char *image_urls;
    int release_month; 
} ManhwaInfo;

typedef struct {
    char *manhwa_name;
    char *heroine_name;
    int id;
    char *image_url;
    int num_images;
} DownloadTask;
```

2. Lalu, kita masukkan data dari file manhwa yang ingin dicari dengan format berupa nama, nama heroine, code API sebagai ID, link poto heroine, dan inisiasi bulan rilis sebagai 0. 

```c
ManhwaInfo manhwa_list[MAX_MANHWA] = {
    { // Lia
        .title = "Mistaken as the Monster Duke's Wife",
        .heroine_name = "Lia",
        .id = 168827,
        .image_urls = "https://i.pinimg.com/736x/cc/88/fc/cc88fc7520e11783dccbf081dcf5293d.jpg",
        .release_month = 0
    },
    { // Tia
        .title = "The Villainess Lives Again", 
        .heroine_name = "Tia", 
        .id = 147205,
        .image_urls = "https://i.pinimg.com/736x/f4/02/2c/f4022c1333fcd4f9bc015433a26e9a21.jpg",
        .release_month = 0
    },
    { // Adelia
        .title = "No, I Only Charmed the Princess!", 
        .heroine_name = "Adelia", 
        .id = 169731,
        .image_urls = "https://i.pinimg.com/736x/43/d6/72/43d672d7e05e96a234071beb02ee4d58.jpg",
        .release_month = 0
    },
    { // Lizen
        .title = "Darling, Why Can't We Divorce?", 
        .heroine_name = "Lizen", 
        .id = 175521,
        .image_urls = "https://i.pinimg.com/736x/6f/b1/bc/6fb1bc43cb83e70c2627a4405300b7ae.jpg",
        .release_month = 0
    }
};
```

3. Ubah format dari nama yang sebelumnya spasi diubah menjadi underscore 

```c
void format_filename(char *str) {
    char temp[MAX_FILENAME] = {0};
    int j = 0;
    for (int i = 0; str[i] && j < MAX_FILENAME - 1; i++) {
        if (str[i] == ' ') { // Setiap bertemu spasi
            temp[j++] = '_'; // Copy underscore pada string baru
        } else if (isalnum(str[i]) || str[i] == '_') { // Jika alfanumerik atau underscore
            temp[j++] = str[i]; // Copy biasa ke string baru
        }
    }
    temp[j] = '\0';
    strncpy(str, temp, MAX_FILENAME); // Ganti inputnya dengan yang diubah
}
```
Membuat string baru (temp) yang akan menyimpan perubahan, dan kembalikan ke string utama dengan strncpy.

4. Mencari huruf kapital dan membuat fungsi untuk membuat folder menggunakan exeplp

```c
void get_uppercase_manhwa_name(const char *manhwa_name, char *result) {
    char temp[MAX_FILENAME]; // Char untuk menyimpan string baru
    strncpy(temp, manhwa_name, MAX_FILENAME - 1); // Copy string nama manhwa
    temp[MAX_FILENAME - 1] = '\0';  // Kasih NULL di belakang
    format_filename(temp); // Ganti ke underscore
    int j = 0;
    for (int i = 0; temp[i] && j < MAX_FILENAME - 1; i++) {
        if (isupper(temp[i])) { // Cek apakah Kapital
            result[j++] = temp[i]; // Jika iya, copy ke string baru
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
```

5. Membuat suatu fungsi untuk mencari data-data yang diperlukan untuk file teksnya

```c
void fetch_manhwa_data(int id, const char *title) {
    char filename[MAX_FILENAME];
    strncpy(filename, title, MAX_FILENAME - 1);
    filename[MAX_FILENAME - 1] = '\0';
    format_filename(filename);
    // Membuat variabel baru untuk menyimpan filename yang sudah diubah formatnya dari spasi ke underscore

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "Manhwa/%s.txt", filename);
    // Membuat file tujuan untuk menyimpan data yang akan dibuat

    pid_t pid = fork(); // Membuat child process dengan fork untuk mengambil data
    if (pid == 0) {
        char api_url[MAX_PATH]; // Variabel untuk menyimpan link
        snprintf(api_url, MAX_PATH, "https://api.jikan.moe/v4/manga/%d", id); // Ubah link dengan mengganti id terakhir
        
        char command[MAX_BUFFER];
        snprintf(command, MAX_BUFFER, 
                "curl -s \"%s\" | jq -r '[" // Run curl untuk menjadi data dari link yang disimpan tadi
                ".data.title // \"N/A\", " // Judul
                ".data.status // \"N/A\", " // Status
                "(.data.published.from // \"N/A\" | split(\"T\")[0]), " // Release dengan keinginan mengambil tanggal saja
                "((.data.genres // []) | map(.name) | join(\", \") // \"N/A\"), " // Genre, jika lebih dari satu gabungkan dan tambahkan koma
                "((.data.themes // []) | map(.name) | join(\", \") // \"N/A\"), " // Tema, jika lebih dari satu gabungkan dan tambahkan koma
                "((.data.authors // []) | map(.name) | join(\", \") // \"N/A\")] | " // Author, jika lebih dari satu gabungkan dan tambahkan koma
                "join(\"\\n\")' > \"%s\"", // Digabungkan dan diberi newline untuk setiap poin dan disimpan ke folder manhwa
                api_url, path); 
        execlp("sh", "sh", "-c", command, NULL); // Eksekusi
        exit(EXIT_FAILURE); // Exit jika gagal
    } else {
        wait(NULL); 
        FILE *file = fopen(path, "r+"); // Buka dir dan lakukan update
        if (file) {
            char content[MAX_BUFFER] = {0}; // Buat char baru
            size_t bytes_read = fread(content, 1, MAX_BUFFER - 1, file); // Membaca file per bit dan menyimpannya di content
            content[bytes_read] = '\0'; // Menyisakan satu string terakhir untuk null
            char *lines[6] = {0}; // Membuat string baru untuk menyimpan dalam bentuk teks
            char *token = strtok(content, "\n"); // Memisahkan konten dengan batasan newline
            int i = 0;
            while (token && i < 6) {
                lines[i++] = token; // Masukkan ke line 
                token = strtok(NULL, "\n");
            }
            rewind(file);
            fprintf(file, "Title: %s\n", lines[0] ? lines[0] : "N/A");
            fprintf(file, "Status: %s\n", lines[1] ? lines[1] : "N/A");
            fprintf(file, "Release: %s\n", lines[2] ? lines[2] : "N/A");
            fprintf(file, "Genre: %s\n", lines[3] ? lines[3] : "N/A");
            fprintf(file, "Theme: %s\n", lines[4] ? lines[4] : "N/A");
            fprintf(file, "Author: %s\n", lines[5] ? lines[5] : "N/A");
            fclose(file); // Urutkan ulang
        }
    }
}
```

### B. Seal the Scrolls
Menyimpan data Manhwa ke dalam file zip

```c
void zip_text_files() {
    create_dir("Archive"); // Buat folde baru Archive
    DIR *dir; // pointer ke directory
    struct dirent *ent; // Struct untuk menyimpan info entry direktori
    if ((dir = opendir("Manhwa")) != NULL) { // Membuka direktori Manhwa untuk dibaca
        while ((ent = readdir(dir)) != NULL) { // Membaca setiap entry dalam direktori dan melakukan loop hingga NULL
            if (strstr(ent->d_name, ".txt")) { // Memeriksa apakah nama file mengandung ekstensi .txt dan 
                char filename[MAX_FILENAME];
                strncpy(filename, ent->d_name, MAX_FILENAME - 1);
                filename[MAX_FILENAME - 1] = '\0'; // Menyimpan ulang nama dari filename
                char *dot = strrchr(filename, '.'); // Mencari titik terakhir berupa .txt
                if (dot) *dot = '\0'; // Mengubah .txt menjadi null
                char uppercase[MAX_FILENAME]; // Menyimpan kapital
                get_uppercase_manhwa_name(filename, uppercase); // Mengubah judul menjadi mengambil huruf kapital saja
                char src_path[MAX_PATH]; // Char untuk menyimmpan data sumber yang sebelumnya
                snprintf(src_path, MAX_PATH, "Manhwa/%s", ent->d_name); // Path lengkap file sumber
                char dest_path[MAX_PATH]; // Char untuk menyimpan file zip
                snprintf(dest_path, MAX_PATH, "Archive/%s.zip", uppercase); // Menyimpan file dalam zip dengan format JUDULKAPITAL.zip
                pid_t pid = fork(); // childe process
                if (pid == 0) {
                    execlp("zip", "zip", "-j", dest_path, src_path, NULL); // Eksekusi file ke dalam zip
                    exit(EXIT_FAILURE); // Exit jika gagal
                } else {
                    wait(NULL); // Menunggu child process
                }
            }
        }
        closedir(dir); // Menutup directory
    }
}

```

### C. Making the Waifu Gallery
Mendownload gambar dari heroine alias Female Main Character (FMC) dengan jumlah unduhan sesuai dengan bulan rilis manhwa.
1. Tentukan bulan release

```c
// Fungsi untuk ekstrak bulan dari tanggal
int extract_month_from_date(const char *date_str) {
    if (!date_str || strcmp(date_str, "N/A") == 0) { // Cek jika date_str NULL atau string "N/A"
        printf("WARNING: Invalid date string\n"); // Cetak warning 
        return 1; // Dan return default atau 1
    }

    int year, month, day; // int untuk menyimpan tanggal, bulan dan tahun
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) == 3) { // Return value 3 menandakan semua 3 komponen berhasil diparse
        printf("DEBUG: Parsed date: %d-%02d-%02d\n", year, month, day); // Tampilkan ulang pada terminal
        return (month >= 1 && month <= 12) ? month : 1; // Cek dan tampilkan bulannya saja
    }

    printf("WARNING: Failed to parse date: %s\n", date_str); // Tampilkan WARNING, jika gagal
    return 1; // Return default
}

// Fungsi untuk mengambil bulan rilis dari API
int fetch_release_month(int id) {
    char temp_file[] = "/tmp/manhwa_temp_XXXXXX"; // Buat int baru dan template nama file dengan XXXXXX yang akan diganti
    int fd = mkstemp(temp_file); // Diubah dengan kode unik
    if (fd == -1) {
        perror("Failed to create temp file"); // Print Failed jika gagal
        return 1; // Default value
    }
    close(fd); // Tutup file descriptor

    char api_url[MAX_PATH]; // Char untuk link Manhwa
    snprintf(api_url, sizeof(api_url), "https://api.jikan.moe/v4/manga/%d", id); // Memasukkan id

    pid_t pid = fork(); // Child process
    if (pid == 0) { 
        // Gunakan curl dengan timeout
        execlp("sh", "sh", "-c", 
               "curl -s --max-time 30 \"$0\" | jq -r '.data.published.from // \"N/A\" | split(\"T\")[0]' > \"$1\"", 
               api_url, temp_file); // Run curl dengan menjalanakn jq untuk mengambil data published from dan akan disimpan di tempt_file
        exit(EXIT_FAILURE); // Exit
    }

    waitpid(pid, NULL, 0); // Tunggu hingga selesai

    // Baca file dengan error handling
    FILE *file = fopen(temp_file, "r"); // Buka file, dan baca saja
    if (!file) {
        remove(temp_file); // Jika file tidak dapat dibuka maka hapus file 
        return 1; // Kembalikan default
    }

    char date_str[20] = "N/A"; // Buat char untuk menyimpan string tanggal dan inisiasi menggunakan N/A
    fgets(date_str, sizeof(date_str), file); // Membaca baris tanggal dan menyimpannya pada string sebelumnya
    fclose(file); // Tutup file
    remove(temp_file); // Hapus file temp yang sudah tidak diperlukan 

    // Debugging: Print tanggal yang didapat
    printf("DEBUG: Raw date for ID %d: %s\n", id, date_str); // Tampilkan tanggal mentah yang didapat

    return extract_month_from_date(date_str); // Panggil fungsi untuk memisahkannya
}
```

2. Download poto (perulangan tergantung dari bulan rilis)

```c
void* download_images(void *arg) {
    DownloadTask *task = (DownloadTask*)arg; // Pannggil struct DownloadTask dan samakan dengan input void yang dimasukkan
    // Dapatkan bulan rilis SEKALI di awal thread
    static pthread_mutex_t api_mutex = PTHREAD_MUTEX_INITIALIZER; // Inisialisasi static mutex
    pthread_mutex_lock(&api_mutex); // Lock
    int release_month = fetch_release_month(task->id); // Jalankan untuk mencari bulan rilis
    pthread_mutex_unlock(&api_mutex); // Unlock

    release_month = (release_month < 1 || release_month > 12) ? 1 : release_month; // Cek apakah sudah sesuai dengan input
    printf("CONFIRMED: %s release month = %d\n", task->heroine_name, release_month); // Print hasilnya ke terminal

    // Buat direktori heroine
    char dirpath[MAX_PATH]; // Untuk folder heroine
    snprintf(dirpath, sizeof(dirpath), "Heroines/%s", task->heroine_name); // Lokasi folder heroine
    create_dir(dirpath); // Buat foldernya 

    printf("Downloading %d copies of %s for %s\n", 
           release_month, task->image_url, task->heroine_name); // Tampilkan pada terminal berapa banyak yang didownload

    // Download satu URL berkali-kali
    for (int i = 0; i < release_month; i++) { // Loop sebanyak bulan rilis
        char filename[MAX_PATH]; // Untuk menyimpan nama file image
        snprintf(filename, sizeof(filename), "%s/%s_%d.jpg",
                dirpath, task->heroine_name, i+1); // Menamakan ulang untuk gambar yang akan didownload dan juga foldernya menjadi folder/namaheroine_loopkeberapa.txt

        pid_t pid = fork(); // 
        if (pid == 0) { // Child process
            // Gunakan wget dengan timeout 30 detik
            execlp("wget", "wget", "-q", "--timeout=30", "-O", filename, task->image_url, NULL); // Exekusi program dari wget untuk mendownload gambar dan format sesuai dengan filename
            exit(EXIT_FAILURE); // Keluar jika gagal 
        } else { // Parent
            int status; // int status
            waitpid(pid, &status, 0); // Tunggu Child Process selesai
            
            if (WIFEXITED(status)) { // Proses berhenti normal (bukan karena signal)
                if (WEXITSTATUS(status) == 0) { // Exit code == 0 (sukses)
                    printf("Success: %s\n", filename); // Print sukses
                } else { // Gagal
                    fprintf(stderr, "Failed to download copy %d (Status: %d)\n", 
                           i+1, WEXITSTATUS(status)); // Print gagal, dan tunjukkan statusnya
                }
            }
        }
        sleep(1); // Delay antara download
    }
    return NULL; // sebagai penanda selesai
}
```

3. Membuat folder dan memanggil fungsi download_images

```c
void process_image_downloads() {
    create_dir("Heroines"); // Buat folder Heroines
    pthread_t threads[MAX_MANHWA]; // Array untuk menyimpan thread (total 4 manhwa)
    DownloadTask tasks[MAX_MANHWA]; // Buat struct DownloadTask baru (sebanyak 4 (untuk 4 manhwa))
    
    for (int i = 0; i < MAX_MANHWA; i++) { // loop 
        tasks[i].manhwa_name = manhwa_list[i].title; // Samakan struct DownloadTask dengan Manhwa_list untuk judul
        tasks[i].heroine_name = manhwa_list[i].heroine_name; // Nama Heroine
        tasks[i].id = manhwa_list[i].id; // Id (untuk cek data manhwa)
        tasks[i].image_url = manhwa_list[i].image_urls; // URL (link download poto)
        static pthread_mutex_t api_mutex = PTHREAD_MUTEX_INITIALIZER; // Buat Mutex statis 
        pthread_mutex_lock(&api_mutex); // Blok
        pthread_create(&threads[i], NULL, download_images, &tasks[i]); // Run download image
        pthread_mutex_unlock(&api_mutex); // Unblok
        pthread_join(threads[i], NULL); // Join jika thread sudah kosong (tunggu hingga thread selesai)
    }
}
```

### D. Zip. Save. Goodbye
Menyimpan semua poto di dalam zip dengan format [HURUFKAPITALNAMAMANHWA]_[namaheroine].zip dan disimpan di folder Archive/Images. Setelah zip selesai, gambar pada masing masing folder Heroine akan dihapus secara urut dengan abjad.

1. Buat fungsi untuk cek apakah directory atau folder yang ingin di archive ada

```c
int is_directory(const char *path) {
    struct stat statbuf; // statbuf adalah struct yang akan menyimpan informasi tentang file/direktori
    if (stat(path, &statbuf)) { // Stat akan mengisi statbuf dengan path dan menceknya apakah sama dengan 0 atau sesuai 
        return 0; // Jika bukan 0, maka return 0
    }
    return S_ISDIR(statbuf.st_mode); // Jika sesuai akan dicek apakah berupa directory atau bukan dengan bit permission
}
```

2. Buat fungsi untuk archive dan hapus file setelah semua berhasil di archive

```c
void archive_and_clean_heroine_images() {
    create_dir("Archive/Images"); // Buat directory Archive/Images
    DIR *dir; // buat variabel directory baru 
    struct dirent *ent; // Buat struct untuk fungsi drectory
    if ((dir = opendir("Heroines")) != NULL) { // Buka directory Heroines dan cek apakah ada
        char *heroine_dirs[MAX_MANHWA]; // Buat char baru untuk tempat file yang akan menjadi lokasi directory heroine
        int count = 0; // int untuk menghitung total dir image 
        while ((ent = readdir(dir)) != NULL && count < MAX_MANHWA) { // Membaca directory heroines dan mengeceknya apakah ada isinya atau tidak
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) { // Mengecek apakah directory sekarang dan directory induk atau sebelumnya tu ada, dan tidak akan dianggap(hanya fokus dengan didalamnya)
                char path[MAX_PATH]; // Buat char baru
                snprintf(path, MAX_PATH, "Heroines/%s", ent->d_name); // Menyimpan folder dalam path
                if (is_directory(path)) { // Cek apakah path termasuk directory
                    heroine_dirs[count] = strdup(ent->d_name); // Subdirectory dalam Heroines akan dimasukkan dalam heroines_dirs
                    if (heroine_dirs[count]) count++; // Cek apakah berhasil dan lanjut
                }
            }
        }
        // Sort alphabetically (Mengurutkan nama Heroine)
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(heroine_dirs[i], heroine_dirs[j]) > 0) {
                    char *temp = heroine_dirs[i];
                    heroine_dirs[i] = heroine_dirs[j];
                    heroine_dirs[j] = temp;
                }
            }
        }
        // Archive each directory
        for (int i = 0; i < count; i++) {
            // Find matching manhwa info
            ManhwaInfo *info = NULL; // Buat struct ManhwaInfo (info) dengan inisiasi awal NULL
            for (int j = 0; j < MAX_MANHWA; j++) {
                if (strcmp(manhwa_list[j].heroine_name, heroine_dirs[i]) == 0) { // Cek apakha antara nama heroine pada ManhwaInfo sama dengan nama directory pada Heroine
                    info = &manhwa_list[j]; // Tulis ulang dalam struct baru
                    break; // keluar dan akan nge loop yang sebelumnya
                }
            }
            if (info) { // Cek apakah info ada (dalam artian apakah datanya berhasil disimpan pada char baru)
                char uppercase[MAX_FILENAME]; // untuk penamaan judul manhwa untuk zip image
                get_uppercase_manhwa_name(info->title, uppercase); // Masukkan hanya kapital
                char zip_path[MAX_PATH]; // Nama zip dan lokasi zipnya
                snprintf(zip_path, MAX_PATH, "Archive/Images/%s_%s.zip", uppercase, heroine_dirs[i]); // Berlokasi di Archive/Images/ dengan format nama JUDUL(kapital)_namaheroine.zip
                char src_path[MAX_PATH]; // Lokasi atau tempat yang ingin di zip
                snprintf(src_path, MAX_PATH, "Heroines/%s", heroine_dirs[i]); // Lokasinya di folder Heroines/NAMAHEROINE
                pid_t pid = fork(); // Child process
                if (pid == 0) {
                    execlp("zip", "zip", "-r", zip_path, src_path, NULL); // Eksekusi file zip dengan zip_path (lokasi tujuan dan nama file zip) dan src_path(sebagai folder yang ingin di zip)
                    exit(EXIT_FAILURE); // Exit jika gagal
                } else {
                    wait(NULL); // Tunggu hingga child process selesai
                }
                pid = fork(); // Buat Child Process
                if (pid == 0) { // berhasil membuat child process
                    execlp("rm", "rm", "-rf", src_path, NULL); // hapus folder src_path(tetapi tidak Heroines folder)
                    exit(EXIT_FAILURE); // Keluar jika gagal
                } else {
                    wait(NULL); // Tunggu hingga child process selesai
                }
            }   
            free(heroine_dirs[i]); // Kosongkan char heroine_dirs
        }
        closedir(dir); // Keluar dari directory
    }
}
```
## Int Main
Memanggil seluruh fungsi
```c
int main() {
    // Part a: Fetch manhwa data
    create_dir("Manhwa");
    for (int i = 0; i < MAX_MANHWA; i++) {
        fetch_manhwa_data(manhwa_list[i].id, manhwa_list[i].title);
    }    
    // Part b: Zip text files
    zip_text_files();
    // Part c: Download images from provided URLs
    process_image_downloads();
    // Part d: Archive and clean
    archive_and_clean_heroine_images();
    printf("All tasks completed successfully!\n");
    return 0;
}
```

## Task 4
### A. Client Mengirimkan Pesan ke Load Balancer (Agil)
Membuat program C (client.c) yang akan mengirimkan pesan ke loadbalancer sebanyak jumlah yang ditulis menggunakan IPC dengan metode shared memory.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

typedef struct {
    char message[100];
    int count;
    int is_available; // Flag untuk menandai ketersediaan data
} MessageData; // Struct untuk menulis pesan nantinya dan memastikan loadbalancer tidak mengakses shared memory bersamaan

void log_message(const char* message, int count) {
    time_t now; 
    time(&now); // Mendapatkan waktu saat ini
    FILE* log_file = fopen("sistem.log", "a"); // membuka file sistem.log dalam mode append
    if (log_file == NULL) {
        perror("Failed to open log file"); // print error jika gagal membuka
        return;
    }
    fprintf(log_file, "Message from client: %s\n", message); // Mencatat pesan apa yang dituliskan dalam sistem.log
    fprintf(log_file, "Message count: %d\n", count); // Mencatat jumlah pesan dalam sistem.log
    fclose(log_file); // Keluar dari sistem.log
}

int main() {
    key_t key = ftok("loadbalancer", 65); // Membuat key unik untuk shared memory
    int shmid = shmget(key, sizeof(MessageData), 0666|IPC_CREAT); // Membuat/alokasi shared memory (0666 = izin akses read/write untuk semua user) (IPC_CREAT = membuat shared memory jika belum ada)
    if (shmid == -1) {
        perror("shmget failed"); // Tampilkan error jika gagal membuat shared memory
        exit(1); // Keluar
    }

    MessageData *data = (MessageData*) shmat(shmid, (void*)0, 0); // Menghubungkan ke shared memory (shmid = id shared memory), (void*)0 = alamat shared memory yang akan di attach, 0 berarti  memilih alamat secara otomatis, dan 0 (terakhir) flag tambahan/ tidak ada flag tambahan
    if (data == (void*)-1) { // Gagal masuk ke alamat shared memory
        perror("shmat failed"); // Print error jika gagal membuat shared memory
        exit(1);
    }

    // Tunggu sampai load balancer siap
    while (data->is_available == 1) { // Menunggu run loadbalancer.c
        sleep(1);
    }

    char input[100]; // Char untuk menyimpan input 
    printf("Enter message and count (format: message;count): "); // Input pesan yang diinginkan
    fgets(input, sizeof(input), stdin); // Membaca input dari user
    input[strcspn(input, "\n")] = 0; // Menghapus newline karena input berupa message;jumlahmessage

    char *message = strtok(input, ";"); // memisahkan input sebelum ";"
    char *count_str = strtok(NULL, ";"); // Memisahkan input sebelum NULL dan setelah ";"
    int count = atoi(count_str); // Mengubah char "count_str" menjadi int biasa

    strncpy(data->message, message, sizeof(data->message)); // Copy message ke struct data->message
    data->count = count; // Input data->count dari inputan sebelumnya yang disimpan di int count
    data->is_available = 1; // Tandai data tersedia untuk loadbalancer

    log_message(message, count); // Jalankan log_message

    // Tunggu sampai load balancer selesai memproses
    while (data->is_available == 1) {
        sleep(1);
    }

    shmdt(data); // Melepaskan shared memory
    return 0;
}
```
