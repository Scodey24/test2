#ifndef USER_H
#define USER_H

#include "models.h"

void menu_user(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman **peminjaman,
    int *jumlah_peminjaman
);

void cari_buku(
    const Buku *buku,
    int jumlah_buku
);

void pinjam_buku(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman **peminjaman,
    int *jumlah_peminjaman
);

void kembalikan_buku(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman *peminjaman,
    int jumlah_peminjaman
);

void riwayat_peminjaman(
    const User *user_login,
    const Peminjaman *peminjaman,
    int jumlah_peminjaman
);

#endif
