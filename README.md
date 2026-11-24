# Sistem Manajemen Perpustakaan Berbasis CLI (Project UAS)

Aplikasi Sistem Manajemen Perpustakaan berbasis antarmuka baris perintah (*Command Line Interface* / CLI) yang dibangun menggunakan bahasa pemrograman **C Standar (ANSI/C)**. Proyek ini dikembangkan untuk memenuhi tugas Ujian Akhir Semester (UAS) Praktikum Pemrograman Kelas A, Program Studi Informatika, FMIPA Universitas Syiah Kuala.

---

## Anggota Kelompok

| No. | Nama Lengkap | NPM | Peran / Modul Tanggung Jawab |
| --- | --- | --- | --- |
| 1 | Raffi Akbar | 260810701100055 | Arsitek Data, Autentikasi, & File Handling |
| 2 | Aishara | 260810701100058 | Modul Hak Akses Admin (CRUD & Laporan) |
| 3 | Bunga Thahira Putri Akram | 260810701100003 | Modul Hak Akses User (Sirkulasi & Pencarian) |
| 4 | Syarifah Keisya Kirany | 260810701100056 | Modul Algoritma Sorting, Main Controller, & Repo |

---

## Deskripsi & Fitur Utama

Aplikasi ini mengelola sirkulasi peminjaman buku dan katalog perpustakaan dengan penyimpanan data persisten berbasis berkas teks (`.txt`):

1. **Autentikasi & Manajemen Hak Akses (*Role-Based Access*):**
    - Mendukung login interaktif maupun melalui *Command Line Arguments* (CLA).
    - Pemisahan hak akses antara **Admin (Pustakawan)** dan **User (Mahasiswa/Anggota)**.
2. **Fitur Admin:**
    - **CRUD Katalog:** Menambah, melihat, memperbarui, dan menghapus data buku di `Books.txt`.
    - **Laporan Transaksi:** Melihat riwayat peminjaman seluruh pengguna secara global dari `peminjaman.txt`.
3. **Fitur User:**
    - Melihat katalog buku tersedia dan mencari buku berdasarkan ID (`ocaid`) atau Judul (`title`).
    - Melakukan transaksi **Peminjaman** (otomatis mengurangi stok) dan **Pengembalian** (otomatis menambah stok).
    - Melihat daftar peminjaman pribadi yang sedang aktif.
4. **Pengurutan Katalog (3 Algoritma Manual):**
    - Implementasi mandiri tanpa fungsi bawaan `qsort` untuk **Bubble Sort**, **Selection Sort**, dan **Insertion Sort**.
    - Mendukung pengurutan *Ascending* maupun *Descending* berdasarkan ID Buku (`ocaid`), Judul (`title`), Tahun Terbit (`year`), dan Jumlah Halaman (`page_count`) / Stok.

---

## Arsitektur Aplikasi & Struktur File

Program dirancang secara modular untuk memudahkan pemeliharaan kode (*clean code*) dan kolaborasi tim:

```
├── main.c             # Entry point program, alur menu utama, dan manajemen sesi
├── models.h           # Definisi struct utama (Buku, User, Peminjaman)
├── file_handler.h     # Deklarasi fungsi I/O berkas teks
├── file_handler.c     # Implementasi parsing dan sinkronisasi data (.txt)
├── auth.h             # Deklarasi fungsi autentikasi pengguna
├── auth.c             # Implementasi login (CLI & CLA) dan validasi role
├── admin.h            # Deklarasi fungsi fitur Admin
├── admin.c            # Implementasi CRUD buku dan laporan transaksi global
├── user.h             # Deklarasi fungsi fitur User
├── user.c             # Implementasi pencarian, peminjaman, dan pengembalian
├── sorting.h          # Deklarasi fungsi algoritma pengurutan
├── sorting.c          # Implementasi manual Bubble, Selection, dan Insertion Sort
├── Makefile           # Otomatisasi proses kompilasi (compiling & linking)
├── Books.txt          # Basis data katalog buku perpustakaan
├── user.txt           # Basis data kredensial akun (username, password, role)
├── peminjaman.txt     # Basis data rekaman transaksi peminjaman
└── README.md          # Dokumentasi proyek
```

---

## Panduan Kompilasi (Compile) & Menjalankan Program (Run)

### 1. Prasyarat

- Compiler C standar (`gcc`)
- Utilitas `make` (opsional, jika menggunakan `Makefile`)

### 2. Kompilasi Program

Buka terminal pada direktori utama proyek, lalu jalankan perintah berikut:

**Menggunakan `Makefile` (Direkomendasikan):**

```bash
make
```

**Atau kompilasi manual menggunakan `gcc`:**

```bash
gcc -Wall -Wextra -std=c99 main.c file_handler.c auth.c admin.c user.c sorting.c -o perpustakaan
```

### 3. Menjalankan Program

Setelah proses kompilasi berhasil dan menghasilkan file executable (`perpustakaan` atau `perpustakaan.exe` pada Windows), jalankan aplikasi dengan salah satu cara berikut:

**Mode Interaktif Standar:**

```bash
./perpustakaan
```

**Mode Command Line Arguments (Langsung Login):**

```bash
./perpustakaan <username> <password>
```

*(Contoh: `./perpustakaan admin1 rahasia123`)*

### 4. Membersihkan File Hasil Kompilasi

Untuk menghapus file objek (`.o`) dan executable hasil build:

```bash
make clean
```

---

## Akun Pengujian Default (Testing Credentials)

Gunakan kredensial berikut (sesuai isi `user.txt`) untuk menguji fitur masing-masing peran:

| Role | Username | Password |
| --- | --- | --- |
| **Admin** | `admin` | `admin123` |
| **User** | `mahasiswa1` | `mhs123` |
