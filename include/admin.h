#ifndef ADMIN_H
#define ADMIN_H

#include "models.h"

void menu_admin(
    Buku **buku,
    int *jumlah_buku,
    const Peminjaman *peminjaman,
    int jumlah_peminjaman
);

void admin_tambah_buku(Buku **buku, int *jumlah_buku);
void admin_lihat_semua_buku(const Buku *buku, int jumlah_buku);
void admin_edit_buku(Buku *buku, int jumlah_buku);
void admin_hapus_buku(Buku **buku, int *jumlah_buku);
void admin_lihat_laporan_peminjaman(
    const Peminjaman *peminjaman,
    int jumlah_peminjaman
);

#endif
