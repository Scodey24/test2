#ifndef MODELS_H
#define MODELS_H

#define PANJANG_OCAID    64
#define PANJANG_JUDUL    256
#define PANJANG_PENGARANG 128
#define PANJANG_USERNAME 32
#define PANJANG_PASSWORD 32
#define PANJANG_ROLE     12
#define PANJANG_TANGGAL  20
#define PANJANG_STATUS   20

#define STOK_DEFAULT 3

typedef struct {
    char ocaid[PANJANG_OCAID];
    char title[PANJANG_JUDUL];
    char author[PANJANG_PENGARANG];
    int year;
    int page_count;
    int stok;
} Buku;

typedef struct {
    char username[PANJANG_USERNAME];
    char password[PANJANG_PASSWORD];
    char role[PANJANG_ROLE];            /* "admin" atau "user" */
} User;

typedef struct {
    unsigned int id_peminjaman;
    char username[PANJANG_USERNAME];
    char ocaid[PANJANG_OCAID];
    char tanggal_pinjam[PANJANG_TANGGAL];   /* DD-MM-YYYY */
    char status[PANJANG_STATUS];            /* "DIPINJAM" atau "KEMBALI" */
} Peminjaman;

#endif
