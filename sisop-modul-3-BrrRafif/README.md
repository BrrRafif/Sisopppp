[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Eu-CByJh)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241051 | Reza Afzaal FT |
| 5025221000 | Student 2 Name |
| 5025221000 | Student 3 Name |

# Praktikum Modul 3 _(Module 3 Lab Work)_

### Laporan Resmi Praktikum Modul 3 _(Module 3 Lab Work Report)_

Di suatu pagi hari yang cerah, Budiman salah satu mahasiswa Informatika ditugaskan oleh dosennya untuk membuat suatu sistem operasi sederhana. Akan tetapi karena Budiman memiliki keterbatasan, Ia meminta tolong kepadamu untuk membantunya dalam mengerjakan tugasnya. Bantulah Budiman untuk membuat sistem operasi sederhana!

_One sunny morning, Budiman, an Informatics student, was assigned by his lecturer to create a simple operating system. However, due to Budiman's limitations, he asks for your help to assist him in completing his assignment. Help Budiman create a simple operating system!_

### Soal 1

> Sebelum membuat sistem operasi, Budiman diberitahu dosennya bahwa Ia harus melakukan beberapa tahap terlebih dahulu. Tahap-tahapan yang dimaksud adalah untuk **mempersiapkan seluruh prasyarat** dan **melakukan instalasi-instalasi** sebelum membuat sistem operasi. Lakukan seluruh tahapan prasyarat hingga [perintah ini](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) pada modul!

> _Before creating the OS, Budiman was informed by his lecturer that he must complete several steps first. The steps include **preparing all prerequisites** and **installing** before creating the OS. Complete all the prerequisite steps up to [this command](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) in the module!_

**Answer:**

- **Code:**

```bash
1.
sudo apt -y update
sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev libssl-dev mtools grub-pc-bin xorriso tmux

2.
mkdir -p osboot
cd osboot

3.
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
tar -xvf linux-6.1.1.tar.xz
cd linux-6.1.1

4.
64-Bit Kernel
General Setup > Configure standard kernel features > Enable support for printk
General Setup > Configure standard kernel features > Enable futex support
General Setup > Initial RAM filesystem and RAM disk (initramfs/initrd) support
General Setup > Control Group Support
Enable the block layer > Legacy autoloading support
Enable the block layer > Partition type > Advanced Partition Selection
Device Drivers > Character devices > Enable TTY
Device Drivers > Character devices > Virtio console
Device Drivers > Character devices > /dev/mem virtual device support
Device Drivers > Network device support > Virtio Network Driver
Device Drivers > Serial Ata / Paralel ATA
Device Drivers > Block Devices > Virtio block driver
Device Drivers > Block Devices > loopback device support
Device Drivers > Block Devices > RAM block device support
Device Drivers > Virtio drivers
Device Drivers > Virtualization Drivers
Device Drivers > Generic Driver Options > Maintain a devtmpfs at filesystems
Device Drivers > Generic Driver Options > Automount devtmpfs at /dev
Executable file formats > Kernel Support for ELF binaries
Executable file formats > Kernel Support scripts starting with #!
File Systems > FUSE support
File Systems > The extended 3 filesystem
File Systems > The extended 4 filesystem
File Systems > Second extended fs support
File Systems > Virtio Filesystem
File Systems > Kernel automounter support
File Systems > Pseudo File Systems > /proc file system support
File Systems > Pseudo File Systems > sysctl support
File Systems > Pseudo File Systems > sysfs file system support
Networking Support > Networking options > Unix domain sockets
Networking Support > Networking options > TCP/IP Networking


make tinyconfig
make menuconfig

5.
make -j$(nproc)

6.
cp arch/x86/boot/bzImage ..

7.
sudo apt install -y busybox-static
whereis busybox

```

- **Explanation:**

  1. Menginstall qemu system.
  2. Membuat direktori osboot.
  3. Download dan ekstrak kernel linux.
  4. Konfigurasi kernel.
  5. Kompilasi kernel.
  6. Hasilkan file `bzimage`.
  7. Instal busybox.

- **Screenshot:**
1. Install qemu.
![installqemu](https://github.com/user-attachments/assets/77f281bb-1d5f-46f1-a1bd-80d4d266c4cf)

2. Membuat dir osboot.

![mkdirosboot](https://github.com/user-attachments/assets/f6499a18-b0b4-490b-9661-26df6ea6df59)

3. Download dan install linux.
![install_linux](https://github.com/user-attachments/assets/60f0f1cf-798b-42eb-aee3-38273fd1a5a0)

4. Hasilkan file bzimage.
![makejs](https://github.com/user-attachments/assets/5198e0c6-49fb-4ebf-9f56-c15b746b4484)

5. Install busybox.
![installbusybox](https://github.com/user-attachments/assets/8f9276a7-39f2-4e61-afe0-6b82225522cc)





   
  


### Soal 2

> Setelah seluruh prasyarat siap, Budiman siap untuk membuat sistem operasinya. Dosen meminta untuk sistem operasi Budiman harus memiliki directory **bin, dev, proc, sys, tmp,** dan **sisop**. Lagi-lagi Budiman meminta bantuanmu. Bantulah Ia dalam membuat directory tersebut!

> _Once all prerequisites are ready, Budiman is ready to create his OS. The lecturer asks that the OS should contain the directories **bin, dev, proc, sys, tmp,** and **sisop**. Help Budiman create these directories!_

**Answer:**

- **Code:**

```sh
1.
sudo bash

2.
mkdir -p myramdisk/{bin,dev,proc,sys,tmp,sisop,etc,root,home/Budiman,home/guest,home/praktikan1,home/praktikan2}`

3.
cp -a /dev/null myramdisk/dev
cp -a /dev/tty* myramdisk/dev
cp -a /dev/zero myramdisk/dev
cp -a /dev/console myramdisk/dev

4.
cp /usr/bin/busybox myramdisk/bin
cd myramdisk/bin./busybox --install .

5.
nano init

#!/bin/sh
/bin/mount -t proc none /proc
/bin/mount -t sysfs none /sys

while true
do
    /bin/getty -L tty1 115200 vt100
    sleep 1
done`

6.
chmod +x init

7.
find . | cpio -oHnewc | gzip > ../myramdisk.gz

```

- **Explanation:**

  1. Masuk ke mode superuser.
  2. Membuat direktori untuk initramfs.
  3. Salin file device ke direktori dev.
  4. Salin busybox ke direktori bin.
  5. Membuat file init.
  6. Memberikan izin eksekusi pada file init.
  7. Kompresi dan buat file init.

- **Screenshot:**

  1. Membuat direktori initram.
    
  ![Screenshot from 2025-05-28 18-08-08](https://github.com/user-attachments/assets/2616bb9b-d801-4d8f-8988-3994a7cdbfce)

  2. Membuat file init.
     
  ![Screenshot from 2025-05-28 18-10-25](https://github.com/user-attachments/assets/68281021-a06f-452d-a02e-b0148074aa15)

  3. Memberikan izin eksekusi pada file init.
     
  ![Screenshot from 2025-05-28 18-11-38](https://github.com/user-attachments/assets/37b9d5a3-ec85-4441-9b0e-23954cc5be87)
  


### Soal 3

> Budiman lupa, Ia harus membuat sistem operasi ini dengan sistem **Multi User** sesuai permintaan Dosennya. Ia meminta kembali kepadamu untuk membantunya membuat beberapa user beserta directory tiap usernya dibawah directory `home`. Buat pula password tiap user-usernya dan aplikasikan dalam sistem operasi tersebut!

> _Budiman forgot that he needs to create a **Multi User** system as requested by the lecturer. He asks your help again to create several users and their corresponding home directories under the `home` directory. Also set each user's password and apply them in the OS!_

**Format:** `user:pass`

```
root:Iniroot
Budiman:PassBudi
guest:guest
praktikan1:praktikan1
praktikan2:praktikan2
```

**Answer:**

- **Code:**

```sh
1.
openssl passwd -1 password

2.
mkdir passwd
root:<<hasilgeneratorrootpassword>>:0:0:root:/root:/bin/sh
Budiman:<<hasilgeneratorrootpassword>>:1001:100:user1:/home/Budiman:/bin/sh
guest:<<hasilgeneratorrootpassword>>:1002:100:user1:/home/guest:/bin/sh
praktikan1:<<hasilgeneratorrootpassword>>:1003:100:user1:/home/praktikan1:/bin/sh
praktikan2:<<hasilgeneratorrootpassword>>:1004:100:user1:/home/praktikan2:/bin/sh

3.
mkdir group
root:x:0:
bin:x:1:root
sys:x:2:root
tty:x:5:root
disk:x:6:root
wheel:x:10:root
users:x:100:Budiman, guest, praktikan1, praktikan2
```

- **Explanation:**

  1. Membuat password dari masing-masing user.
  2. Membuat file passwd didalam etc yang berisikan hasil dari nomor 1.
  3. Membuat file group didalam etc.

- **Screenshot:**

  1. Isi file passwd.
  ![Screenshot from 2025-05-28 18-12-59](https://github.com/user-attachments/assets/f3eacb3f-d09b-4771-aa75-492387c13457)

  2. Isi file group.
     
  ![Screenshot from 2025-05-28 18-13-16](https://github.com/user-attachments/assets/2ca57db0-795e-4e78-bb04-13604c78e451)

### Soal 4

> Dosen meminta Budiman membuat sistem operasi ini memilki **superuser** layaknya sistem operasi pada umumnya. User root yang sudah kamu buat sebelumnya akan digunakan sebagai superuser dalam sistem operasi milik Budiman. Superuser yang dimaksud adalah user dengan otoritas penuh yang dapat mengakses seluruhnya. Akan tetapi user lain tidak boleh memiliki otoritas yang sama. Dengan begitu user-user selain root tidak boleh mengakses `./root`. Buatlah sehingga tiap user selain superuser tidak dapat mengakses `./root`!

> _The lecturer requests that the OS must have a **superuser** just like other operating systems. The root user created earlier will serve as the superuser in Budiman's OS. The superuser should have full authority to access everything. However, other users should not have the same authority. Therefore, users other than root should not be able to access `./root`. Implement this so that non-superuser accounts cannot access `./root`!_

**Answer:**

- **Code:**

```sh
  chown 0:0 root
```

- **Explanation:**

  1. Mengubah akses root agar hanya bisa diakses oleh root.

- **Screenshot:**

 
![Screenshot from 2025-05-28 18-00-20](https://github.com/user-attachments/assets/87948bbd-61b9-4719-853a-9b07ab69449c)

### Soal 5

> Setiap user rencananya akan digunakan oleh satu orang tertentu. **Privasi dan otoritas tiap user** merupakan hal penting. Oleh karena itu, Budiman ingin membuat setiap user hanya bisa mengakses dirinya sendiri dan tidak bisa mengakses user lain. Buatlah sehingga sistem operasi Budiman dalam melakukan hal tersebut!

> _Each user is intended for an individual. **Privacy and authority** for each user are important. Therefore, Budiman wants to ensure that each user can only access their own files and not those of others. Implement this in Budiman's OS!_

**Answer:**

- **Code:**

  `put your answer here`

- **Explanation:**

  `put your answer here`

- **Screenshot:**

  `put your answer here`

### Soal 6

> Dosen Budiman menginginkan sistem operasi yang **stylish**. Budiman memiliki ide untuk membuat sistem operasinya menjadi stylish. Ia meminta kamu untuk menambahkan tampilan sebuah banner yang ditampilkan setelah suatu user login ke dalam sistem operasi Budiman. Banner yang diinginkan Budiman adalah tulisan `"Welcome to OS'25"` dalam bentuk **ASCII Art**. Buatkanlah banner tersebut supaya Budiman senang! (Hint: gunakan text to ASCII Art Generator)

> _Budiman wants a **stylish** operating system. Budiman has an idea to make his OS stylish. He asks you to add a banner that appears after a user logs in. The banner should say `"Welcome to OS'25"` in **ASCII Art**. Use a text to ASCII Art generator to make Budiman happy!_ (Hint: use a text to ASCII Art generator)

**Answer:**

- **Code:**

  `put your answer here`

- **Explanation:**

  `put your answer here`

- **Screenshot:**

  `put your answer here`

### Soal 7

> Melihat perkembangan sistem operasi milik Budiman, Dosen kagum dengan adanya banner yang telah kamu buat sebelumnya. Kemudian Dosen juga menginginkan sistem operasi Budiman untuk dapat menampilkan **kata sambutan** dengan menyebut nama user yang login. Sambutan yang dimaksud berupa kalimat `"Helloo %USER"` dengan `%USER` merupakan nama user yang sedang menggunakan sistem operasi. Kalimat sambutan ini ditampilkan setelah user login dan setelah banner. Budiman kembali lagi meminta bantuanmu dalam menambahkan fitur ini.

> _Seeing the progress of Budiman's OS, the lecturer is impressed with the banner you created. The lecturer also wants the OS to display a **greeting message** that includes the name of the user who logs in. The greeting should say `"Helloo %USER"` where `%USER` is the name of the user currently using the OS. This greeting should be displayed after user login and after the banner. Budiman asks for your help again to add this feature._

**Answer:**

- **Code:**

  `put your answer here`

- **Explanation:**

  `put your answer here`

- **Screenshot:**

  `put your answer here`

### Soal 8

> Dosen Budiman sudah tua sekali, sehingga beliau memiliki kesulitan untuk melihat tampilan terminal default. Budiman menginisiatif untuk membuat tampilan sistem operasi menjadi seperti terminal milikmu. Modifikasilah sistem operasi Budiman menjadi menggunakan tampilan terminal kalian.

> _Budiman's lecturer is quite old and has difficulty seeing the default terminal display. Budiman takes the initiative to make the OS look like your terminal. Modify Budiman's OS to use your terminal appearance!_

**Answer:**

- **Code:**
  
  di directory myramdisk
  ```bash
  nano init
  !/bin/sh
  /bin/mount -t proc none /proc
  /bin/mount -t sysfs none /sys

  while true
  do
    /bin/getty -L ttyS0 115200 vt100
    sleep 1
  done

  # lakukan penambahan code pada etc/profile
  cat >> /etc/profile << 'EOF'
  export PS1='\[\e[1;32m\]\u@\h:\w\$ \[\e[0m\]'
  EOF
  # untuk run gunakan code berikut
  qemu-system-x86_64 \
  -smp 2 \
  -m 256 \
  -display gtk \
  -nographic \
  -kernel bzImage \
  -initrd myramdisk.gz
  ```

- **Explanation:**

  `-L ttyS0` = Memulai login prompt pada serial console atau terminal asli

  `PS1='\[\e[1;32m\]\u@\h:\w\$ \[\e[0m\]` = Untuk mengubah prompt shell menjadi warna hijau

  `display gtk` = Menampilkan window GUI QEMU dengan virtual VGA (saat butuh tampilan grafis)

  `nographic` = Menonaktifkan output grafis, mengalihkan semua I/O ke console/text

- **Screenshot:**

  1. File init
  ![Screenshot 2025-05-28 135016](https://github.com/user-attachments/assets/13ba2cfa-1910-48ee-81f2-8145d771182a)

  2. File profile
  ![Screenshot 2025-05-28 135111](https://github.com/user-attachments/assets/071c5f24-7042-420f-92a6-11dd92816ccf)

  3. Code run
  ![Screenshot 2025-05-28 135530](https://github.com/user-attachments/assets/cb1f405c-28e7-46cf-a531-4f4d8f7b71c9)

  4. Tampilan
  ![Screenshot 2025-05-28 135555](https://github.com/user-attachments/assets/f80eab24-fb44-42f1-aa8f-71d15791a5ee)

### Soal 9

> Ketika mencoba sistem operasi buatanmu, Budiman tidak bisa mengubah text file menggunakan text editor. Budiman pun menyadari bahwa dalam sistem operasi yang kamu buat tidak memiliki text editor. Budimanpun menyuruhmu untuk menambahkan **binary** yang telah disiapkan sebelumnya ke dalam sistem operasinya. Buatlah sehingga sistem operasi Budiman memiliki **binary text editor** yang telah disiapkan!

> _When trying your OS, Budiman cannot edit text files using a text editor. He realizes that the OS you created does not have a text editor. Budiman asks you to add the prepared **binary** into his OS. Make sure Budiman's OS has the prepared **text editor binary**!_

**Answer:**

- **Code:**

  Di dir lokasi myramdisk
  ```
  git clone https://github.com/morisab/budiman-text-editor.git
  cd budiman-text-editor
  g++ -static main.cpp -o edit
  cd ..
  cp budiman-text-editor/edit myramdisk/bin
  ```

- **Explanation:**

  `git clone https://github.com/morisab/budiman-text-editor.git` = melakukan git clown pada budiman-text-editor.git

  `g++ -static main.cpp -o edit` = Compile binary statically linked agar bisa run tanpa dependency dengan nama edit untuk melakukan run pada terminal

- **Screenshot:**

  1. Isi file Budiman setelah dicompile
     ![Screenshot 2025-05-28 135748](https://github.com/user-attachments/assets/25f3b387-d6ec-4933-befe-f0b2786c12e3)
     
  2. Lokasi copy-an di bin
     ![Screenshot 2025-05-28 135842](https://github.com/user-attachments/assets/bf7625cf-f595-4174-8eef-52ae6c87d1c8)
     
  3. Penggunaan
     ![Screenshot 2025-05-28 140244](https://github.com/user-attachments/assets/fb1dd423-2b24-416e-be38-0d163132bf84)
     
- **Kendala**
  Bisa diketik dan di save serta dilakukan cat tetapi tidak dapat menampilkan ketikan pada text editor
  ![Screenshot 2025-05-28 140308](https://github.com/user-attachments/assets/990d6cd9-da84-4531-bbba-ba2e217a9f24)

### Soal 10

> Setelah seluruh fitur yang diminta Dosen dipenuhi dalam sistem operasi Budiman, sudah waktunya Budiman mengumpulkan tugasnya ini ke Dosen. Akan tetapi, Dosen Budiman tidak mau menerima pengumpulan selain dalam bentuk **.iso**. Untuk terakhir kalinya, Budiman meminta tolong kepadamu untuk mengubah seluruh konfigurasi sistem operasi yang telah kamu buat menjadi sebuah **file .iso**.

> After all the features requested by the lecturer have been implemented in Budiman's OS, it's time for Budiman to submit his assignment. However, Budiman's lecturer only accepts submissions in the form of **.iso** files. For the last time, Budiman asks for your help to convert the entire configuration of the OS you created into a **.iso file**.

**Answer:**

- **Code:**

  Di dir osboot
  ```
  mkdir -p mylinuxiso/boot/grub
  cp bzImage mylinuxiso/boot/
  cp myramdisk.gz mylinuxiso/boot/

  cat > mylinuxiso/boot/grub/grub.cfg << 'EOF'
  set timeout=5                  # Tunggu 5 detik input user
  set default=0                  # Pilih entry pertama secara default

  menuentry "MyLinux OS" {       # Label yang ditampilkan
    linux /boot/bzImage        # Path kernel relatif terhadap root ISO
    initrd /boot/myramdisk.gz  # Path initramfs
  }
  EOF

  grub-mkrescue -o mylinux.iso mylinuxiso
  ```

- **Explanation:**

  `mkdir -p mylinuxiso/boot/grub` = Buat directory ISO

  `cp bzImage mylinuxiso/boot/` = Copy kernel dan initramfs

  `cp myramdisk.gz mylinuxiso/boot/` = Copy binary editor

  `grub-mkrescue` = Membuat ISO dengan bootloader GRUB yang berisi kernel (bzImage), initramfs (myramdisk.gz), dan binary editor (edit).  
  
- **Screenshot:**

  1. File grub.cfg
  ![Screenshot 2025-05-28 142411](https://github.com/user-attachments/assets/228027ed-0404-4af7-90df-caa2cb7d99c9)

  2. Run `grub-mkrescue -o mylinux.iso mylinuxiso`
  ![Screenshot 2025-05-28 143526](https://github.com/user-attachments/assets/c5fc7dac-c2f6-4dde-b45d-741eaedea6d0)

  3. Isi file osboot
  ![Screenshot 2025-05-28 143609](https://github.com/user-attachments/assets/1114a9d1-70cc-460e-91f5-eac85931d360)

---

Pada akhirnya sistem operasi Budiman yang telah kamu buat dengan susah payah dikumpulkan ke Dosen mengatasnamakan Budiman. Kamu tidak diberikan credit apapun. Budiman pun tidak memberikan kata terimakasih kepadamu. Kamupun kecewa tetapi setidaknya kamu telah belajar untuk menjadi pembuat sistem operasi sederhana yang andal. Selamat!

_At last, the OS you painstakingly created was submitted to the lecturer under Budiman's name. You received no credit. Budiman didn't even thank you. You feel disappointed, but at least you've learned to become a reliable creator of simple operating systems. Congratulations!_
