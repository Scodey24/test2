#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_io.h"

#define BUFFER_BARIS    2048
#define KAPASITAS_AWAL  64

/* ---------- fungsi bantu (private) ---------- */

static void hapus_newline(char *s)
{
    size_t n = strlen(s);

    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static void ganti_pipa(char *s)
{
    for (; *s != '\0'; s++) {
        if (*s == '|') {
            *s = ' ';
        }
    }
}

/* Gandakan kapasitas array. Return pointer baru, atau NULL jika gagal
   (array lama TIDAK di-free di sini, biar pemanggil yang mengurusnya). */
static void *perbesar(void *array, size_t ukuran_elemen, int *kapasitas)
{
    void *baru;

    *kapasitas *= 2;
    baru = realloc(array, (size_t)(*kapasitas) * ukuran_elemen);
    return baru;
}

/* Baca satu kolom dari baris Books.txt asli mulai dari posisi p.
   - Kalau diawali tanda petik: baca sampai petik penutup ("" di dalam = satu tanda petik).
   - Kalau tidak: baca sampai spasi/tab berikutnya.
   Teks yang kepanjangan dipotong. Return posisi setelah kolom. */
static const char *baca_kolom(const char *p, char *out, int ukuran)
{
    int n = 0;

    while (*p == ' ' || *p == '\t') {
        p++;
    }
    if (*p == '"') {
        p++;
        while (*p != '\0') {
            if (*p == '"') {
                if (*(p + 1) == '"') {
                    if (n < ukuran - 1) {
                        out[n++] = '"';
                    }
                    p += 2;
                    continue;
                }
                p++;
                break;
            }
            if (n < ukuran - 1) {
                out[n++] = *p;
            }
            p++;
        }
    } else {
        while (*p != '\0' && *p != ' ' && *p != '\t') {
            if (n < ukuran - 1) {
                out[n++] = *p;
            }
            p++;
        }
    }
    out[n] = '\0';
    return p;
}

/* Parse satu baris data Books.txt asli. Return 1 jika sukses. */
static int parse_baris_asli(const char *baris, Buku *b)
{
    const char *p = baris;

    p = baca_kolom(p, b->ocaid, PANJANG_OCAID);
    p = baca_kolom(p, b->title, PANJANG_JUDUL);
    p = baca_kolom(p, b->author, PANJANG_PENGARANG);

    if (b->ocaid[0] == '\0') {
        return 0;
    }
    if (b->title[0] == '\0') {
        strcpy(b->title, "-");
    }
    if (b->author[0] == '\0') {
        strcpy(b->author, "-");
    }
    return sscanf(p, "%d %d", &b->year, &b->page_count) == 2;
}

/* ---------- umum ---------- */

int file_ada(const char *nama_file)
{
    FILE *fp = fopen(nama_file, "r");

    if (fp == NULL) {
        return 0;
    }
    fclose(fp);
    return 1;
}

/* ---------- buku ---------- */

int import_buku_asli(const char *file_asli, const char *file_tujuan, int stok_default)
{
    FILE *in = fopen(file_asli, "r");
    FILE *out;
    char baris[BUFFER_BARIS];
    Buku b;
    int nomor_baris = 0;
    int berhasil = 0;
    int dilewati = 0;

    if (in == NULL) {
        return -1;
    }
    out = fopen(file_tujuan, "w");
    if (out == NULL) {
        fclose(in);
        return -1;
    }

    while (fgets(baris, sizeof(baris), in) != NULL) {
        nomor_baris++;
        if (nomor_baris == 1) {
            continue;   /* baris pertama adalah header kolom */
        }
        hapus_newline(baris);

        if (parse_baris_asli(baris, &b)) {
            ganti_pipa(b.title);
            ganti_pipa(b.author);
            fprintf(out, "%s|%s|%s|%d|%d|%d\n",
                    b.ocaid, b.title, b.author, b.year, b.page_count, stok_default);
            berhasil++;
        } else {
            dilewati++;
        }
    }

    fclose(in);
    fclose(out);
    printf("Import selesai: %d buku masuk, %d baris dilewati.\n", berhasil, dilewati);
    return berhasil;
}

int muat_buku(const char *nama_file, Buku **daftar, int *jumlah)
{
    FILE *fp = fopen(nama_file, "r");
    char baris[BUFFER_BARIS];
    int kapasitas = KAPASITAS_AWAL;
    Buku *arr;
    Buku *tmp;
    Buku b;

    *daftar = NULL;
    *jumlah = 0;
    if (fp == NULL) {
        return 0;
    }
    arr = (Buku *)malloc((size_t)kapasitas * sizeof(Buku));
    if (arr == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        hapus_newline(baris);
        if (sscanf(baris, "%63[^|]|%255[^|]|%127[^|]|%d|%d|%d",
                   b.ocaid, b.title, b.author,
                   &b.year, &b.page_count, &b.stok) != 6) {
            continue;   /* lewati baris rusak / kosong */
        }
        if (*jumlah >= kapasitas) {
            tmp = (Buku *)perbesar(arr, sizeof(Buku), &kapasitas);
            if (tmp == NULL) {
                free(arr);
                fclose(fp);
                *jumlah = 0;
                return 0;
            }
            arr = tmp;
        }
        arr[*jumlah] = b;
        (*jumlah)++;
    }

    fclose(fp);
    *daftar = arr;
    return 1;
}

int simpan_buku(const char *nama_file, const Buku *daftar, int jumlah)
{
    FILE *fp = fopen(nama_file, "w");
    int i;

    if (fp == NULL) {
        return 0;
    }
    for (i = 0; i < jumlah; i++) {
        fprintf(fp, "%s|%s|%s|%d|%d|%d\n",
                daftar[i].ocaid, daftar[i].title, daftar[i].author,
                daftar[i].year, daftar[i].page_count, daftar[i].stok);
    }
    fclose(fp);
    return 1;
}

/* ---------- user ---------- */

int muat_user(const char *nama_file, User **daftar, int *jumlah)
{
    FILE *fp = fopen(nama_file, "r");
    char baris[BUFFER_BARIS];
    int kapasitas = KAPASITAS_AWAL;
    User *arr;
    User *tmp;
    User u;

    *daftar = NULL;
    *jumlah = 0;
    if (fp == NULL) {
        return 0;
    }
    arr = (User *)malloc((size_t)kapasitas * sizeof(User));
    if (arr == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        if (sscanf(baris, "%31s %31s %11s", u.username, u.password, u.role) != 3) {
            continue;
        }
        if (*jumlah >= kapasitas) {
            tmp = (User *)perbesar(arr, sizeof(User), &kapasitas);
            if (tmp == NULL) {
                free(arr);
                fclose(fp);
                *jumlah = 0;
                return 0;
            }
            arr = tmp;
        }
        arr[*jumlah] = u;
        (*jumlah)++;
    }

    fclose(fp);
    *daftar = arr;
    return 1;
}

int simpan_user(const char *nama_file, const User *daftar, int jumlah)
{
    FILE *fp = fopen(nama_file, "w");
    int i;

    if (fp == NULL) {
        return 0;
    }
    for (i = 0; i < jumlah; i++) {
        fprintf(fp, "%s %s %s\n", daftar[i].username, daftar[i].password, daftar[i].role);
    }
    fclose(fp);
    return 1;
}

/* ---------- peminjaman ---------- */

int muat_peminjaman(const char *nama_file, Peminjaman **daftar, int *jumlah)
{
    FILE *fp = fopen(nama_file, "r");
    char baris[BUFFER_BARIS];
    int kapasitas = KAPASITAS_AWAL;
    Peminjaman *arr;
    Peminjaman *tmp;
    Peminjaman p;

    *daftar = NULL;
    *jumlah = 0;
    if (fp == NULL) {
        return 0;
    }
    arr = (Peminjaman *)malloc((size_t)kapasitas * sizeof(Peminjaman));
    if (arr == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        if (sscanf(baris, "%u %31s %63s %19s %19s",
                   &p.id_peminjaman, p.username, p.ocaid,
                   p.tanggal_pinjam, p.status) != 5) {
            continue;
        }
        if (*jumlah >= kapasitas) {
            tmp = (Peminjaman *)perbesar(arr, sizeof(Peminjaman), &kapasitas);
            if (tmp == NULL) {
                free(arr);
                fclose(fp);
                *jumlah = 0;
                return 0;
            }
            arr = tmp;
        }
        arr[*jumlah] = p;
        (*jumlah)++;
    }

    fclose(fp);
    *daftar = arr;
    return 1;
}

int simpan_peminjaman(const char *nama_file, const Peminjaman *daftar, int jumlah)
{
    FILE *fp = fopen(nama_file, "w");
    int i;

    if (fp == NULL) {
        return 0;
    }
    for (i = 0; i < jumlah; i++) {
        fprintf(fp, "%u %s %s %s %s\n",
                daftar[i].id_peminjaman, daftar[i].username, daftar[i].ocaid,
                daftar[i].tanggal_pinjam, daftar[i].status);
    }
    fclose(fp);
    return 1;
}
