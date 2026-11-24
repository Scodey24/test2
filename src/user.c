#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"
#include "file_io.h"
#include "user.h"

static void bersihkan_input(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static void input_teks(const char *pesan, char *teks, int ukuran)
{
    printf("%s", pesan);

    if (fgets(teks, ukuran, stdin) == NULL) {
        teks[0] = '\0';
        return;
    }

    teks[strcspn(teks, "\n")] = '\0';

    if (strchr(teks, '\0') == teks + ukuran - 1) {
        bersihkan_input();
    }
}

static int validasi_tanggal_dd_mm_yyyy(const char *tgl)
{
    int hari = 0, bulan = 0, tahun = 0;
    char extra;

    if (tgl == NULL || strlen(tgl) != 10) {
        return 0;
    }

    if (tgl[2] != '-' || tgl[5] != '-') {
        return 0;
    }

    if (sscanf(tgl, "%2d-%2d-%4d%c", &hari, &bulan, &tahun, &extra) != 3) {
        return 0;
    }

    if (bulan < 1 || bulan > 12) {
        return 0;
    }

    if (hari < 1 || hari > 31) {
        return 0;
    }

    if (tahun < 1900 || tahun > 2100) {
        return 0;
    }

    return 1;
}

static void tampilkan_buku(const Buku *buku, int jumlah_buku)
{
    int i;

    printf("\n========== DAFTAR BUKU ==========\n");

    if (jumlah_buku == 0) {
        printf("Belum ada buku tersedia.\n");
        return;
    }

    printf("%-15s %-30s %-15s %-8s %-8s %-8s\n",
           "ID", "JUDUL", "PENULIS",
           "TAHUN", "HALAMAN", "STOK");

    for (i = 0; i < jumlah_buku; i++) {
        printf("%-15s %-30.30s %-15.15s %-8d %-8d %-8d\n",
               buku[i].ocaid,
               buku[i].title,
               buku[i].author,
               buku[i].year,
               buku[i].page_count,
               buku[i].stok);
    }
}

static int cari_indeks_buku(
    const Buku *buku,
    int jumlah_buku,
    const char *ocaid)
{
    int i;

    for (i = 0; i < jumlah_buku; i++) {
        if (strcmp(buku[i].ocaid, ocaid) == 0) {
            return i;
        }
    }

    return -1;
}

static int cari_indeks_peminjaman(
    const Peminjaman *peminjaman,
    int jumlah_peminjaman,
    unsigned int id)
{
    int i;

    for (i = 0; i < jumlah_peminjaman; i++) {
        if (peminjaman[i].id_peminjaman == id) {
            return i;
        }
    }

    return -1;
}

static unsigned int id_berikutnya(
    const Peminjaman *peminjaman,
    int jumlah_peminjaman)
{
    unsigned int terbesar = 0;
    int i;

    for (i = 0; i < jumlah_peminjaman; i++) {
        if (peminjaman[i].id_peminjaman > terbesar) {
            terbesar = peminjaman[i].id_peminjaman;
        }
    }

    return terbesar + 1;
}

/* =====================================
1. FITUR CARI BUKU
   ===================================== */

void cari_buku(const Buku *buku, int jumlah_buku)
{
    char kata_kunci[PANJANG_JUDUL];
    int i;
    int ditemukan = 0;

    printf("\n========== CARI BUKU ==========\n");

    input_teks("Masukkan ID atau Judul Buku: ",
               kata_kunci, sizeof(kata_kunci));

    for (i = 0; i < jumlah_buku; i++) {

        if (strcmp(buku[i].ocaid, kata_kunci) == 0 ||
            strstr(buku[i].title, kata_kunci) != NULL) {

            printf("\nBuku ditemukan!\n");
            printf("ID       : %s\n", buku[i].ocaid);
            printf("Judul    : %s\n", buku[i].title);
            printf("Penulis  : %s\n", buku[i].author);
            printf("Tahun    : %d\n", buku[i].year);
            printf("Halaman  : %d\n", buku[i].page_count);
            printf("Stok     : %d\n", buku[i].stok);

            ditemukan = 1;
        }
    }

    if (!ditemukan) {
        printf("Buku tidak ditemukan.\n");
    }
}

/* =====================================
2. FITUR PINJAM BUKU
   ===================================== */

void pinjam_buku(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman **peminjaman,
    int *jumlah_peminjaman)
{
    char ocaid[PANJANG_OCAID];
    char tanggal[PANJANG_TANGGAL];

    int indeks;
    Peminjaman *temp;
    Peminjaman transaksi;

    printf("\n========== PEMINJAMAN BUKU ==========\n");

    tampilkan_buku(buku, jumlah_buku);

    input_teks("\nMasukkan ID Buku: ",
               ocaid, sizeof(ocaid));

    indeks = cari_indeks_buku(
        buku, jumlah_buku, ocaid
    );

    if (indeks == -1) {
        printf("Buku tidak ditemukan.\n");
        return;
    }

    if (buku[indeks].stok <= 0) {
        printf("Maaf, stok buku habis.\n");
        return;
    }

    input_teks("Tanggal Pinjam (DD-MM-YYYY): ",
               tanggal, sizeof(tanggal));

    if (!validasi_tanggal_dd_mm_yyyy(tanggal)) {
        printf("Format tanggal tidak valid. Gunakan DD-MM-YYYY (contoh: 24-11-2026).\n");
        return;
    }

    transaksi.id_peminjaman =
        id_berikutnya(*peminjaman, *jumlah_peminjaman);

    strcpy(transaksi.username, user_login->username);
    strcpy(transaksi.ocaid, buku[indeks].ocaid);
    strcpy(transaksi.tanggal_pinjam, tanggal);
    strcpy(transaksi.status, "DIPINJAM");

    /* Menambah kapasitas array peminjaman */
    temp = (Peminjaman *)realloc(
        *peminjaman,
        (*jumlah_peminjaman + 1) * sizeof(Peminjaman)
    );

    if (temp == NULL) {
        printf("Gagal menambahkan transaksi.\n");
        return;
    }

    *peminjaman = temp;

    (*peminjaman)[*jumlah_peminjaman] = transaksi;
    (*jumlah_peminjaman)++;

    buku[indeks].stok--;

    if (!simpan_buku(FILE_BUKU, buku, jumlah_buku) ||
        !simpan_peminjaman(FILE_PEMINJAMAN,
                           *peminjaman, *jumlah_peminjaman)) {

        printf("Peringatan: Gagal menyimpan data ke file.\n");
        printf("Periksa kembali file penyimpanan.\n");
        return;
    }

    printf("\nPeminjaman berhasil!\n");
    printf("ID Peminjaman : %u\n", transaksi.id_peminjaman);
    printf("ID Buku       : %s\n", buku[indeks].ocaid);
    printf("Judul         : %s\n", buku[indeks].title);
    printf("Tanggal       : %s\n", tanggal);
    printf("Status        : DIPINJAM\n");
    printf("Stok tersisa  : %d\n", buku[indeks].stok);
}

/* =====================================
3. FITUR KEMBALIKAN BUKU
   ===================================== */

void kembalikan_buku(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman *peminjaman,
    int jumlah_peminjaman)
{
    unsigned int id;
    int indeks_pinjam;
    int indeks_buku;

    printf("\n========== PENGEMBALIAN BUKU ==========\n");

    riwayat_peminjaman(
        user_login, peminjaman, jumlah_peminjaman
    );

    printf("\nMasukkan ID Peminjaman: ");

    if (scanf("%u", &id) != 1) {
        printf("Input ID tidak valid.\n");
        bersihkan_input();
        return;
    }

    bersihkan_input();

    indeks_pinjam = cari_indeks_peminjaman(
        peminjaman, jumlah_peminjaman, id
    );

    if (indeks_pinjam == -1) {
        printf("Data peminjaman tidak ditemukan.\n");
        return;
    }

    if (strcmp(peminjaman[indeks_pinjam].username,
               user_login->username) != 0) {

        printf("Transaksi bukan milik akun Anda.\n");
        return;
    }

    if (strcmp(peminjaman[indeks_pinjam].status,
               "DIPINJAM") != 0) {

        printf("Buku sudah dikembalikan sebelumnya.\n");
        return;
    }

    indeks_buku = cari_indeks_buku(
        buku,
        jumlah_buku,
        peminjaman[indeks_pinjam].ocaid
    );

    if (indeks_buku == -1) {
        printf("Data buku tidak ditemukan.\n");
        return;
    }

    strcpy(peminjaman[indeks_pinjam].status, "KEMBALI");

    buku[indeks_buku].stok++;

    if (!simpan_buku(FILE_BUKU, buku, jumlah_buku) ||
        !simpan_peminjaman(FILE_PEMINJAMAN,
                           peminjaman, jumlah_peminjaman)) {

        printf("Peringatan: Gagal menyimpan data ke file.\n");
        printf("Periksa kembali file penyimpanan.\n");
        return;
    }

    printf("\nBuku berhasil dikembalikan!\n");
    printf("ID Peminjaman : %u\n", id);
    printf("ID Buku       : %s\n",
           buku[indeks_buku].ocaid);
    printf("Status        : KEMBALI\n");
    printf("Stok sekarang : %d\n",
           buku[indeks_buku].stok);
}

/* =====================================
4. FITUR RIWAYAT PEMINJAMAN
   ===================================== */

void riwayat_peminjaman(
    const User *user_login,
    const Peminjaman *peminjaman,
    int jumlah_peminjaman)
{
    int i;
    int ditemukan = 0;

    printf("\n========== RIWAYAT PEMINJAMAN ==========\n");

    printf("Username: %s\n\n", user_login->username);

    printf("%-8s %-15s %-15s %-15s\n",
           "ID", "ID BUKU", "TANGGAL", "STATUS");

    for (i = 0; i < jumlah_peminjaman; i++) {

        if (strcmp(peminjaman[i].username,
                   user_login->username) == 0) {

            printf("%-8u %-15s %-15s %-15s\n",
                   peminjaman[i].id_peminjaman,
                   peminjaman[i].ocaid,
                   peminjaman[i].tanggal_pinjam,
                   peminjaman[i].status);

            ditemukan = 1;
        }
    }

    if (!ditemukan) {
        printf("Belum ada riwayat peminjaman.\n");
    }
}

/* =====================================
5. MENU USER
   ===================================== */

void menu_user(
    const User *user_login,
    Buku *buku,
    int jumlah_buku,
    Peminjaman **peminjaman,
    int *jumlah_peminjaman)
{
    int pilihan = 0;

    do {
        printf("\n================================\n");
        printf("          MENU USER             \n");
        printf("================================\n");
        printf("Selamat datang, %s\n",
               user_login->username);

        printf("1. Cari Buku\n");
        printf("2. Pinjam Buku\n");
        printf("3. Kembalikan Buku\n");
        printf("4. Riwayat Peminjaman\n");
        printf("5. Logout\n");

        printf("Pilih menu: ");

        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid.\n");
            bersihkan_input();
            pilihan = -1;
            continue;
        }

        bersihkan_input();

        switch (pilihan) {

            case 1:
                cari_buku(buku, jumlah_buku);
                break;

            case 2:
                pinjam_buku(
                    user_login,
                    buku,
                    jumlah_buku,
                    peminjaman,
                    jumlah_peminjaman
                );
                break;

            case 3:
                kembalikan_buku(
                    user_login,
                    buku,
                    jumlah_buku,
                    *peminjaman,
                    *jumlah_peminjaman
                );
                break;

            case 4:
                riwayat_peminjaman(
                    user_login,
                    *peminjaman,
                    *jumlah_peminjaman
                );
                break;

            case 5:
                printf("Berhasil logout.\n");
                break;

            default:
                printf("Pilihan tidak tersedia.\n");
                break;
        }

    } while (pilihan != 5);
}