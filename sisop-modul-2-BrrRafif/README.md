[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9LcL5VTQ)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241029 | Berwyn Rafif Alvaro |
| 5025241051 | Reza Afzaal Faizullah Taqy|
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

## Task 1 (Berwyn)

## **a. Ekstraksi File ZIP**

Pada soal ini kita diminta untuk mendownload file zip yang berisi poster film dan mengekstrak file itu.

### Kode 

```sh
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    char *zip = "/home/berwyn/Sistem Operasi/film.zip";
    char *output_f = "/home/berwyn/Sistem Operasi";
    char *url = "https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B";

    if (pid == 0) execlp("wget", "wget", "-O", zip, url, NULL);

    else {
        int status;
        wait(&status);

        if (WIFEXITED(status)){
            char *argv[] = {"unzip", zip, "-d", output_f, NULL};
            execv("/usr/bin/unzip", argv);
        } 
    }
    return 0;
}
```

1. Membuat proses baru dan membuat 3 variabel yang berisi path ke file .zip, direktori penyimpanan file .zip, dan alamat url file.
```sh
pid_t pid = fork();
char *zip = "/home/berwyn/Sistem Operasi/film.zip";
char *output_f = "/home/berwyn/Sistem Operasi";
char *url = "https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B";
```

2. Menjalankan child process untuk mendownload file .zip
```sh
if (pid == 0) execlp("wget", "wget", "-O", zip, url, NULL);
```

- **if (pid == 0)** -> jika pid = 0 (Child proses sedang berjalan), maka jalankan blok perintah selanjutnya
- **execlp("wget", "wget",** -> memanggil perintah wget untuk mendownload file .zip
- **"-O", zip** -> mengubah nama file yang telah di download sesuai dengan variabel zip dan menyimpannya di path zip
- **url** -> path untuk mendownload file
- **NULL** -> penanda akhir argumen

3. Menjalankan parent process untuk melakukan ekstraksi file .zip
```sh
else {
        int status;
        wait(&status);

        if (WIFEXITED(status)){
            char *argv[] = {"unzip", zip, "-d", output_f, NULL};
            execv("/usr/bin/unzip", argv);
        } 
    }
return 0;
```

- **status** -> variabel yang menyimpan status dari child process
- **wait(&status)8** -> menunggu child process sampai selesai
- **if (WIFEXITED(status))** -> percabangan untuk memeriksa apakah child process berjalan semestinya
- ***argv[]** -> parameter untuk melakukan unzip
- **unzip"** -> perintah yang akan dilakukan
- **zip** -> variabel yang menyimpan alamat file yang mau di ekstrak
- **"-d", output_f** -> menentukan output direktori hasil ekstrak ke variabel "output_f"
- **NULL** -> penanda akhir argumen
- **execv("/usr/bin/unzip", argv);** -> melakukan ekstraksi dengan argumen yang sudah disiapkan
- **return 0** -> mengakhiri program

4. Hasil run program
![image](https://github.com/user-attachments/assets/2606d758-d2f5-4361-b3a7-debd2296e74b)

![image](https://github.com/user-attachments/assets/06e5ea83-7e1f-465a-98b6-1fb978448fda)


## **b. Pemilihan Film Secara Acak**

Pada soal ini kita diminnta untuk memilihkan film yang akan ditonton oleh trabowo dan peddy

### Kode
```sh
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
```

1. Membuat pointer untuk membuka dan membaca isi direktori, dan membuat variable alamat direktori tempat penyimpanan poster.
```sh
DIR *dir;
struct dirent *file;
char *output_f = "/home/berwyn/Sistem Operasi/film";
```

2. Membuat variable untuk menyimpan dan menghitung poster film dan variable untuk memilih film secara acak
```sh
char poster[51][51];
int i = 0, acak;
```

3. Membuka dan menyimpan isi direktori
```sh
dir = opendir(output_f);

while ((file = readdir(dir)) != NULL){
    if (strstr(file->d_name, ".jpg")){
        strcpy(poster[i], file->d_name);
        i++;
    }
}

closedir(dir);
```

- **dir = opendir(output_f);** -> membuka folder film
- **while ((file = readdir(dir)) != NULL)** -> membaca setiap isi dari folder film
- **if (strstr(file->d_name, ".jpg"))** -> menyaring hanya file yang mengandung ".jpg" (file gambar)
- **strcpy(poster[i], file->d_name);** -> menyimpan nama film ke array "poster"
- **i++** -> menghitung jumlah film yang berhasil disimpan
- **closedir(dir);** -> menutup folder film saat sudah selesai

4. Memilih film secara acak untuk ditonton
```sh
srand(time(NULL));
acak = rand() % i;
printf("Film for Trabowo & Peddy: %s", poster[acak]);

return 0;
```

- **srand(time(NULL))** -> inisialisasi seed generator angka acak agar hasil rand() berbeda setiap dijalankan
- **acak = rand() % i;** -> mmenghasilkan angka secara acak dan disimpan ke variable `acak`
- **printf("Film for Trabowo & Peddy: %s", poster[acak]);** -> menampilkan nama film untuk ditonton
- **return 0** ->mengakhiri program

5. Hasil run program

![image](https://github.com/user-attachments/assets/b714583a-6936-46fb-98f0-a5e51413be7e)


## **c. Memilah Film Berdasarkan Genre**

Dalam soal ini, kita diminta untuk mengelompokkan film berdasarkan genrenya. Terdapat dua orang yang akan melakukannya, yaitu Trabowo dan Peddy. Trabowo akan memproses film dari urutan awal, sedangkan Peddy dari urutan akhir. Setiap kali memindahkan film ke folder genre yang sesuai, mereka harus mencatat log-nya secara bergantian ke dalam file "recap.txt". Setelah semua film selesai dikelompokkan, mereka akan menghitung jumlah film untuk setiap genre dan menentukan genre dengan jumlah terbanyak. Hasil perhitungan ini akan disimpan di file "total.txt".

### Kode
```sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

char *A_horror  = "/home/berwyn/Sistem Operasi/film/FilmHorror";
char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
char *A_drama   = "/home/berwyn/Sistem Operasi/film/FilmDrama";
char *source_f = "/home/berwyn/Sistem Operasi/film";

FILE *FileLog;
char *poster[51];
int idx = 0, giliran = 0;  

pthread_mutex_t mutex_turn = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_turn  = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

int get_index(char *name) {
    int temp_idx = 0;
    while (*name && *name != '_') {
        if (*name >= '0' && *name <= '9'){
            temp_idx = temp_idx * 10 + (*name - '0');
        }
        name++;
    }
    return temp_idx;
}

int sort(const void *a, const void *b) {
    char *fa = *(char **)a;
    char *fb = *(char **)b;
    return get_index(fa) - get_index(fb);
}

void write_log(char *nama, char *film, char *folder) {
    time_t waktu = time(NULL);
    struct tm time = *localtime(&waktu);
    char simpanWaktu[64];
    strftime(simpanWaktu, sizeof(simpanWaktu), "%d-%m-%Y %H:%M:%S", &time);

    pthread_mutex_lock(&mutex_log);

    fprintf(FileLog, "[%s] %s: %s telah dipindahkan ke %s\n", simpanWaktu, nama, film, folder);
    
    pthread_mutex_unlock(&mutex_log);
}

void memilah(char *nama, char *film) {
    char *dest_dir, *folder;
    if (strstr(film, "horror")){
        dest_dir = A_horror;
        folder = "FilmHorror";
    } 
    
    else if (strstr(film, "animasi")) {
        dest_dir = A_animasi;
        folder = "FilmAnimasi";
    } 
    
    else if (strstr(film, "drama")) {
        dest_dir = A_drama;
        folder = "FilmDrama";
    }

    char src[256], dst[256];
    sprintf(src, "%s/%s", source_f, film);
    sprintf(dst, "%s/%s", dest_dir, film);

    if (rename(src, dst) == 0) write_log(nama, film, folder);
}

void *trabowo(void *arg) {
    for (int i = 0; i < (idx/2); i++) {
        pthread_mutex_lock(&mutex_turn);
        while (giliran != 0) pthread_cond_wait(&cond_turn, &mutex_turn);

        memilah("Trabowo", poster[i]);

        giliran = 1;
        pthread_cond_signal(&cond_turn);
        pthread_mutex_unlock(&mutex_turn);
    }
}

void *peddy(void *arg) {
    for (int i = idx - 1; i >= idx/2; i--) {
        pthread_mutex_lock(&mutex_turn);
        while (giliran != 1) pthread_cond_wait(&cond_turn, &mutex_turn);

        memilah("Peddy", poster[i]);

        giliran = 0;
        pthread_cond_signal(&cond_turn);
        pthread_mutex_unlock(&mutex_turn);
    }
}

void hitung(){
    FILE *total;
    int hor, ani, dr;
    hor = ani = dr = 0;

    for(int i = 0; i < idx; i++){
        if (strstr(poster[i], "horror")) hor++;
        else if (strstr(poster[i], "animasi")) ani++;
        else if (strstr(poster[i], "drama")) dr++;
    }

    total = fopen("total.txt", "w");
    fprintf(total, "Jumlah film horror: %d\n", hor);
    fprintf(total, "Jumlah film animasi: %d\n", ani);
    fprintf(total, "Jumlah film drama: %d\n", dr);

    if (hor > ani && hor > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Horror\n");

    else if (ani > hor && ani > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Animasi\n");

    else fprintf(total, "Genre dengan jumlah film terbanyak: Drama\n");

    fclose(total);
}


int main() {
    DIR *dir = opendir(source_f);
    struct dirent *file;
    
    while ((file = readdir(dir)) != NULL) {
        if (strstr(file->d_name, ".jpg")) {
            poster[idx] = strdup(file->d_name);
            idx++;
        }
    }    
    
    closedir(dir);

    qsort(poster, idx, sizeof(char*), sort);

    mkdir(A_horror,  0777);
    mkdir(A_animasi, 0777);
    mkdir(A_drama,   0777);

    FileLog = fopen("recap.txt", "a");

    pthread_t th0, th1;
    pthread_create(&th0, NULL, trabowo, NULL);
    pthread_create(&th1, NULL, peddy,   NULL);
    pthread_join(th0, NULL);
    pthread_join(th1, NULL);

    hitung();
    fclose(FileLog);

    printf("Semua film telah berhasil di sortir\n");

    return 0;
}
```

1. Variabel global untuk menyimpan lokasi folder hasil sortir tiap genre, dan lokasi folder asal film
```sh
char *A_horror  = "/home/berwyn/Sistem Operasi/film/FilmHorror";
char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
char *A_drama   = "/home/berwyn/Sistem Operasi/film/FilmDrama";
char *source_f = "/home/berwyn/Sistem Operasi/film";
```

2. Array dan variable global untuk daftar film dan sinkronisasi
```sh
FILE *FileLog;
char *poster[51];
int idx = 0, giliran = 0; 

pthread_mutex_t mutex_turn = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_turn  = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;
```

- ***poster[]** -> array untuk menyimpan film
- **idx** -> jumlah film
- **giliran** -> menandai giliran antarthread
- **mutex_turn, cond_turn** -> untuk mengatur thread supaya bekerja secara bergantian
- **mutex_log** -> untuk penulisan log

3. Mengambil angka dari nama file untuk disorting
```sh
int get_index(char *name) {
    int temp_idx = 0;
    while (*name && *name != '_') {
        if (*name >= '0' && *name <= '9'){
            temp_idx = temp_idx * 10 + (*name - '0');
        }
        name++;
    }
    return temp_idx;
}
```
- **temp_idx** -> menyimpan angka urutan film untuk nantinya direturn
- **while (*name && *name != `'_'`)** -> membaca semua angka yang ada di file film hingga karakter '_'
- **if (name >= '0' && *name <= '9')** -> memeriksa apakah karakter yang ditunjuk oleh pointer adalah sebuah angka
- **temp_idx = temp_idx * 10 + (*name - '0')** -> mengubah karakter menjadi bentuk angka dan disimpan ke dalam variabel "temp_idx"
- **name++** -> menggeser pointernya ke karakter berikutnya
- **return temp_idx;** -> return nilai angka urutan film

4. Melakukan sort menggunakan quicksort
```sh
int sort(const void *a, const void *b) {
    char *fa = *(char **)a;
    char *fb = *(char **)b;
    return get_index(fa) - get_index(fb);
}
```
- **char *fa = *(char ** )a** -> mengambil isi dari array `poster` dan disimpan ke dalam 'fa'
- **char *fb = *(char ** )b** -> mengambil isi dari array "poster" dan disimpan ke dalam 'fb'
- **return get_index(fa) - get_index(fb);** -> memanggil fungsi `get_index()` untuk mendapatkan angka urutan file, dan mengembalikan hasil selisih dari kedua angka urutan file itu

Apabila hasilnya + maka fa dipindah ke kanan fb, karena fa > fb
Apabila hasilnya - maka fa dipindah ke kiri fb, karena fa < fb

5. Menulis log pemindahan film
```sh
void write_log(char *nama, char *film, char *folder) {
    time_t waktu = time(NULL);
    struct tm time = *localtime(&waktu);
    char simpanWaktu[64];
    strftime(simpanWaktu, sizeof(simpanWaktu), "%d-%m-%Y %H:%M:%S", &time);

    pthread_mutex_lock(&mutex_log);

    fprintf(FileLog, "[%s] %s: %s telah dipindahkan ke %s\n", simpanWaktu, nama, film, folder);
    
    pthread_mutex_unlock(&mutex_log);
}
```

- **time_t waktu = time(NULL)** -> mengambil waktu saat program ini dijalankan dalam format `time_t`
- **struct tm time = *localtime(&waktu);** -> merubah formatnya menjadi struktur `struct tm` agar bisa membaca tanggal
- **char simpanWaktu[64];** -> array untuk menyimpan waktu dalam bentuk string
- **strftime(simpanWaktu, sizeof(simpanWaktu), "%d-%m-%Y %H:%M:%S", &time);** -> mengubah waktu yang disimpan di variable `time` menjadi bentuk string yang kemudian akan disimpan di array `simpanWaktu` dengan format tanggal-bulan-tahun jam:menit:detik
- **pthread_mutex_lock(&mutex_log);** -> mengunci mutexnya supaya tidak ada thread lain yang menulis log di `FileLog`
- **fprintf(FileLog, "[%s] %s: %s telah dipindahkan ke %s\n", simpanWaktu, nama film, folder);** -> menulis ke dalam `FileLog`dengan format [waktu] oleh_siapa: film_apa telah dipindahkan ke folder_mana
- **pthread_mutex_unlock(&mutex_log);** -> membuka kembali mutexnya saat sudah selesai

6. Mengelompokkan film berdasarkan genre
```sh
void memilah(char *nama, char *film) {
    char *dest_dir, *folder;
    if (strstr(film, "horror")){
        dest_dir = A_horror;
        folder = "FilmHorror";
    } 
    
    else if (strstr(film, "animasi")) {
        dest_dir = A_animasi;
        folder = "FilmAnimasi";
    } 
    
    else if (strstr(film, "drama")) {
        dest_dir = A_drama;
        folder = "FilmDrama";
    }

    char src[256], dst[256];
    sprintf(src, "%s/%s", source_f, film);
    sprintf(dst, "%s/%s", dest_dir, film);

    if (rename(src, dst) == 0) write_log(nama, film, folder);
}
```

- ***dest_dir, *folder** -> variable untuk menyimpan alamat folder tujuan pemindahan dan nama folder yang akan dicatat di log

- **if (strstr(film, "horror"))** -> memeriksa apakah film yang sedang diproses saat ini adalah film horror
- **dest_dir = A_horror;** -> memasukkan path folder tujuannya ke folder film horror
- **folder = "FilmHorror";** -> mengubah folder tempat disimpannya ke folder film horror

- **if (strstr(film, "animasi"))** -> memeriksa apakah film yang sedang diproses saat ini adalah film animasi
- **dest_dir = A_animasi;** -> memasukkan path folder tujuannya ke folder film animasi
- **folder = "FilmAnimasi";** -> mengubah folder tempat disimpannya ke folder film animasi

- **if (strstr(film, "drama"))** -> memeriksa apakah film yang sedang diproses saat ini adalah film drama
- **dest_dir = A_drama;** -> memasukkan path folder tujuannya ke folder film drama
- **folder = "FilmDrama";** -> mengubah folder tempat disimpannya ke folder film drama

- **char src[256], dst[256]** -> array untuk menyimpan path file sumber dan file tujuan
- **sprintf(src, "%s/%s", source_f, film)** -> membuat path untuk lokasi awal file dan menyimpannya ke array `src`
- **sprintf(dst, "%s/%s", dest_dir, film)** -> membuat path untuk lokasi file tujuan dan menyimpannya ke array `dst`
- **if (rename(src, dst) == 0) write_log(nama, film, folder);** -> memindahkan file dari `src` ke `dst`, jika berhasil maka akan menuliskan lognya menggunakan fungsi `write_log`


7. Thread prabowo yang memproses file dari awal
```sh
void *trabowo(void *arg) {
    for (int i = 0; i < (idx/2); i++) {
        pthread_mutex_lock(&mutex_turn);
        while (giliran != 0) pthread_cond_wait(&cond_turn, &mutex_turn);

        memilah("Trabowo", poster[i]);

        giliran = 1;
        pthread_cond_signal(&cond_turn);
        pthread_mutex_unlock(&mutex_turn);
    }
}
```
- **`for (int i = 0; i < (idx/2); i++)`** -> loop untuk memproses file film dari awal
- **pthread_mutex_lock(&mutex_turn)** -> mengunci mutexnya supaya hanya ada satu thread yang berjalan
- **while (giliran != 0) pthread_cond_wait(&cond_turn, &mutex_turn);** -> memeriksa apakah sekarang adalah giliran thread ini berjalan, apabila bukan maka thread akan menunggu hingga thread lain selesai dan memberikan sinyal untuk thread ini berjalan
- **memilah("Trabowo", poster[i]);** -> memanggil fungsi `memilah` untuk memindahkan file ke folder yang sesuai dengan genre filmnya dan menandai film ini diproses oleh trabowo
- **giliran = 1** -> mengubah giliran menjadi 1 supaya thread lain bisa berjalan
- **pthread_cond_signal(&cond_turn)** -> mengirim sinyal ke thread lain untuk bisa berjalan
- - **pthread_mutex_unlock(&mutex_log);** -> membuka kembali mutexnya saat sudah selesai


8. Thread peddy yang memproses file dari akhir
```sh
void *peddy(void *arg) {
    for (int i = idx - 1; i >= idx/2; i--) {
        pthread_mutex_lock(&mutex_turn);
        while (giliran != 1) pthread_cond_wait(&cond_turn, &mutex_turn);

        memilah("Peddy", poster[i]);

        giliran = 0;
        pthread_cond_signal(&cond_turn);
        pthread_mutex_unlock(&mutex_turn);
    }
}
```
- **for (int i = idx - 1; i >= idx/2; i--)** -> loop untuk memproses file film dari akhir
- **pthread_mutex_lock(&mutex_turn)** -> mengunci mutexnya supaya hanya ada satu thread yang berjalan
- **while (giliran != 1) pthread_cond_wait(&cond_turn, &mutex_turn);** -> memeriksa apakah sekarang adalah giliran thread ini berjalan, apabila bukan maka thread akan menunggu hingga thread lain selesai dan memberikan sinyal untuk thread ini berjalan
- **memilah("Peddy", poster[i]);** -> memanggil fungsi `memilah` untuk memindahkan file ke folder yang sesuai dengan genre filmnya dan menandai film ini diproses oleh peddy
- **giliran = 0** -> mengubah giliran menjadi 1 supaya thread lain bisa berjalan
- **pthread_cond_signal(&cond_turn)** -> mengirim sinyal ke thread lain untuk bisa berjalan
- - **pthread_mutex_unlock(&mutex_log);** -> membuka kembali mutexnya saat sudah selesai

9. Menghitung total film per genre, menentukan genre dengan film terbanyak, dan menulis hasilnya ke dalam total.txt
```sh
void hitung(){
    FILE *total;
    int hor, ani, dr;
    hor = ani = dr = 0;

    for(int i = 0; i < idx; i++){
        if (strstr(poster[i], "horror")) hor++;
        else if (strstr(poster[i], "animasi")) ani++;
        else if (strstr(poster[i], "drama")) dr++;
    }

    total = fopen("total.txt", "w");
    fprintf(total, "Jumlah film horror: %d\n", hor);
    fprintf(total, "Jumlah film animasi: %d\n", ani);
    fprintf(total, "Jumlah film drama: %d\n", dr);

    if (hor > ani && hor > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Horror\n");

    else if (ani > hor && ani > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Animasi\n");

    else fprintf(total, "Genre dengan jumlah film terbanyak: Drama\n");

    fclose(total);
}
```
- **FILE *total** -> pointer file untuk file total.txt
- **int hor, ani, dr;** -> inisialisasi variable untuk menyimpan jumlah film tiap genre
- **hor = ani = dr = 0** -> variable di set nilai awalnya 0
- **`for(int i = 0; i < idx; i++)`** -> loop untuk menghitung jumlah film tiap genre
- **if (strstr(poster[i], "horror")) hor++** -> jika film adalah film horror, maka jumlah film horror bertambah
- **else if (strstr(poster[i], "animasi")) ani++** -> jika film adalah film animasi, maka jumlah film animasi bertambah
- **else if (strstr(poster[i], "drama")) dr++** -> jika film adalah film drama, maka jumlah film drama bertambah
- **total = fopen("total.txt", "w")** -> membuat dan membuka file total.txt jika blm ada
- **fprintf(total, "Jumlah film horror: %d\n", hor)** -> menulis jumlah film horror ke dalam total.txt
- **fprintf(total, "Jumlah film animasi: %d\n", ani)** -> menulis jumlah film animasi ke dalam total.txt
- **fprintf(total, "Jumlah film drama: %d\n", dr))** -> menulis jumlah film horror ke dalam total.txt
- **if (hor > ani && hor > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Horror\n")** -> memeriksa apakah genre horror memiliki film paling banyak, jika iya maka tulis di program
- **else if (ani > hor && ani > dr) fprintf(total, "Genre dengan jumlah film terbanyak: Animasi\n")** -> memeriksa apakah genre animasi memiliki film paling banyak, jika iya maka tulis di program
- **else fprintf(total, "Genre dengan jumlah film terbanyak: Drama\n")** -> jika bukan kedua genre diatas, maka genre film drama lah yang memiliki jumlah film paling banyak
- **fclose(total)** -> tutup file total.txt apabila sudah selesai


10. Fungsi main
```sh
int main() {
    DIR *dir = opendir(source_f);
    struct dirent *file;
    
    while ((file = readdir(dir)) != NULL) {
        if (strstr(file->d_name, ".jpg")) {
            poster[idx] = strdup(file->d_name);
            idx++;
        }
    }    
    
    closedir(dir);

    qsort(poster, idx, sizeof(char*), sort);

    mkdir(A_horror,  0777);
    mkdir(A_animasi, 0777);
    mkdir(A_drama,   0777);

    FileLog = fopen("recap.txt", "a");

    pthread_t th0, th1;
    pthread_create(&th0, NULL, trabowo, NULL);
    pthread_create(&th1, NULL, peddy,   NULL);
    pthread_join(th0, NULL);
    pthread_join(th1, NULL);

    hitung();
    fclose(FileLog);

    printf("Semua film telah berhasil di sortir\n");

    return 0;
}
```

- **IR *dir = opendir(source_f)** -> membuka direktori penyimpanan file film
- **struct dirent *file** -> pointer untuk mennyimpan nama file dari dalam direktori
- **while ((file = readdir(dir)) != NULL)** -> membaca setiap data dari dalam direktori
- **if (strstr(file->d_name, ".jpg"))** -> jika file di dalam direktori yang sedang dibuka berformat jpg (di nama filenya ada .jog)
- **poster[idx] = strdup(file->d_name)** -> maka simpan nama file itu ke dalam array poster
- **idx++** -> increment indexnya
- **closedir(dir)** -> tutup direktorinya saat sudah selesai dibaca
- **qsort(poster, idx, sizeof(char*), sort)** -> mengurutkan data yang ada di dalam array poster secara ascending
- **mkdir(A_horror,  0777); mkdir(A_animasi, 0777); mkdir(A_drama,   0777);** -> membuat direktori baru untuk setiap genre film
- **FileLog = fopen("recap.txt", "a")** -> membuka file recap.txt

```c
    pthread_t th0, th1;
    pthread_create(&th0, NULL, trabowo, NULL);
    pthread_create(&th1, NULL, peddy,   NULL);
    pthread_join(th0, NULL);
    pthread_join(th1, NULL);
```
-> membuat dan menjalankan thread untuk memilah film

- **hitung()** -> memanggil fungsi hitung
- **fclose(FileLog)** -> menutup file recap.txt
- **printf("Semua film telah berhasil di sortir\n")** -> mmenampilkan pesan bahwa proses penyortiran telah berhasil
- **return 0** -> menghentikan program

11. Hasil run program

 ![image](https://github.com/user-attachments/assets/f3ae23bf-6a50-4918-8b07-025edd0aad83)

![image](https://github.com/user-attachments/assets/90bf415a-170b-4821-a482-cc50874822a6)

![image](https://github.com/user-attachments/assets/cee1e8ba-4829-4e5b-9b80-40ce035818e0)

![image](https://github.com/user-attachments/assets/7baae72e-2bcd-4acd-8c9a-10aa863079fb)

![image](https://github.com/user-attachments/assets/62b5481b-2717-49e2-a6ae-9352e2fb15f9)

![image](https://github.com/user-attachments/assets/2abfeb9c-c0d1-4e30-9d4e-59881020bd84)

![image](https://github.com/user-attachments/assets/8321bb01-f7e7-42b7-b41d-aeb9bb68b2d9)

## **d. Pengarsipan Film**

Pada soal ini, folder-folder hasil penyortiran berdasarkan genre kita arsipkan kembali menjadi format .zip untuk menghemat ruang penyimpanan

### Kode

```sh
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *A_horror = "/home/berwyn/Sistem Operasi/film/FilmHorror";
    char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
    char *A_drama = "/home/berwyn/Sistem Operasi/film/FilmDrama";
    char *output_f = "/home/berwyn/Sistem Operasi/film/Sortir Film.zip"; 
    
    pid_t child;
    child = fork();
    if (child == 0) execlp("/usr/bin/zip", "zip", "-rm", output_f, A_horror, A_animasi, A_drama, NULL);
    
    else {
        int status;
        wait(&status);

        if (WIFEXITED(status)) printf("\nBerhasil mengarsipkan dan menghapus folder.\n");
    }
}

```

1. Membuat variable untuk menyimpan path ke folder hasil sortir tiap genre, dan path ke direktori hasil pengarsipan 
```sh
char *A_horror = "/home/berwyn/Sistem Operasi/film/FilmHorror";
char *A_animasi = "/home/berwyn/Sistem Operasi/film/FilmAnimasi";
char *A_drama = "/home/berwyn/Sistem Operasi/film/FilmDrama";
char *output_f = "/home/berwyn/Sistem Operasi/film/Sortir Film.zip"; 
```

2. Membuat proses baru dan menjalankan child process untuk melakukan pengarsipan dan penghapusan folder
```sh
pid_t child;
child = fork();

if (child == 0) execlp("/usr/bin/zip", "zip", "-rm", output_f, A_horror, A_animasi, A_drama, NULL);
```

- **if (child == 0)** -> jika child = 0 (child process sedang berjalan), maka jalankan bagian selanjutnya
- **execlp("/usr/bin/zip", "zip"** -> mencari program zip dan menjalankannya (mengarsipkan folder ke format .zip)
- **"-rm"** -> menghapus folder beserta isinya setelah selesai diarsipkan
- **output_f** -> alamat tempat menyimpan file hasil proses pengarsipan
- **A_horror** -> alamat folder film horror
- **A_animasi** -> alamat folder film animasi
- **A_drama** -> alamat folder film drama
- **NULL** -> penanda akhir argumen

3. Parent process untuk memberikan output apabila berhasil melakukan pengarsipan
```sh
else {
    int status;
    wait(&status);

    if (WIFEXITED(status)) printf("\nBerhasil mengarsipkan dan menghapus folder.\n");
}
```

- **status** -> variabel yang menyimpan status dari child process
- **wait(&status)** -> menunggu child process sampai selesai
- **if (WIFEXITED(status))** -> percabangan untuk memeriksa apakah child process berjalan dengan normal
- **printf("\nBerhasil mengarsipkan dan menghapus folder.\n");** -> menampilkan pesan bahwa proses pengarsipan sudah berhasil

4. Hasil run program
   
![image](https://github.com/user-attachments/assets/dd8c0cbd-4d66-49ad-99fc-8a3476aa5d47)


![image](https://github.com/user-attachments/assets/50425b4c-4a8e-41d4-ae5a-fb434bbd9c54)

![image](https://github.com/user-attachments/assets/c85f7f47-b29f-46d2-b5b3-a586c0cf8ef8)

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
               api_url, temp_file, NULL);
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
        static pthread_mutex_t api_mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&api_mutex);
        pthread_create(&threads[i], NULL, download_images, &tasks[i]);
        pthread_mutex_unlock(&api_mutex);
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

Untuk ouputnya akan di langsung dipisahkan dengan format underscore
![Screenshot 2025-04-30 053044](https://github.com/user-attachments/assets/640ec8e7-a318-47a3-8d81-898d87510a2c)
![Screenshot 2025-04-30 052841](https://github.com/user-attachments/assets/9acc3cee-c841-4c48-9aba-2fe9b3b22f94)

(Salah satu contoh data manhwanya)
![image](https://github.com/user-attachments/assets/2f3ab9c5-bc9c-4157-a24f-8cad43bc61f6)

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

Hasil akhirnya :
![image](https://github.com/user-attachments/assets/e88e6163-6fe5-48f7-8cd3-485a418fe76a)
![image](https://github.com/user-attachments/assets/3906ab6f-ee60-4010-9ca0-07fe474b353d)


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
Hasilnya download akan dilakukan satu persatu dari heroines manhwa (dengan urutan manhwa seperti sebelumnya) sejumlah bulan rilis:
![Screenshot 2025-04-30 050723](https://github.com/user-attachments/assets/26c1ae2a-2c13-4a0d-9455-625d9b0e5405)
Salah satu hasilnya
![image](https://github.com/user-attachments/assets/29369041-3c11-4028-b1df-7ca6b141cf52)

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
Archive dan penghapusan akan dilakukan secara berurutan (sesuai abjad):
![Screenshot 2025-04-30 050737](https://github.com/user-attachments/assets/ac8f8fb0-ede6-4cfc-b38c-bf496d4c6c7a)
![Screenshot 2025-04-30 052919](https://github.com/user-attachments/assets/e88b1c31-9ae3-467c-8918-51bc9a365d16)
Salah satu output (jika zip di unzip):
![Screenshot 2025-04-30 052936](https://github.com/user-attachments/assets/116f90f4-c453-44d0-a013-e942679b4f0b)


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
Jika di run akan menampilkan output untuk input nya:
![image](https://github.com/user-attachments/assets/662ed8a7-ed65-4697-bd2f-f6c815753365)
Dan akan menunggu loadbalancer hingga ready, serta langsung menulis pada sistem.log:
![image](https://github.com/user-attachments/assets/9a381ad9-be6e-458f-be1b-b6dfb897d947)


