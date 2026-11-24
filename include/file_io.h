#ifndef FILE_IO_H
#define FILE_IO_H

#include "models.h"

#define FILE_BUKU_ASLI  "../data/Books.txt"        /* dataset master dari soal */
#define FILE_BUKU       "../data/books_data.txt"   /* file operasional (ada kolom stok) */
#define FILE_USER       "../data/user.txt"
#define FILE_PEMINJAMAN "../data/peminjaman.txt"

/* Semua fungsi muat/simpan: return 1 jika berhasil, 0 jika gagal.
   Array hasil muat_* dialokasikan dengan malloc -> pemanggil wajib free(). */

int file_ada(const char *nama_file);

/* Konversi Books.txt asli -> books_data.txt (format pipa + stok).
   Return jumlah buku yang berhasil diimpor, atau -1 jika file gagal dibuka. */
int import_buku_asli(const char *file_asli, const char *file_tujuan, int stok_default);

int muat_buku(const char *nama_file, Buku **daftar, int *jumlah);
int simpan_buku(const char *nama_file, const Buku *daftar, int jumlah);

int muat_user(const char *nama_file, User **daftar, int *jumlah);
int simpan_user(const char *nama_file, const User *daftar, int jumlah);

int muat_peminjaman(const char *nama_file, Peminjaman **daftar, int *jumlah);
int simpan_peminjaman(const char *nama_file, const Peminjaman *daftar, int jumlah);

#endif
