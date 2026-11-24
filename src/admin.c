
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/admin.h"
#include "../include/file_io.h"
#include "../include/sorting.h"

/* Membersihkan sisa input */
static void bersihkan_input(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Fungsi input teks */
static void input_teks(const char *pesan, char *tujuan, int ukuran)
{
    printf("%s", pesan);

    if (fgets(tujuan, ukuran, stdin) == NULL) {
        tujuan[0] = '\0';
        return;
    }

    if (strchr(tujuan, '\n') == NULL) {
        bersihkan_input();
    } else {
        tujuan[strcspn(tujuan, "\n")] = '\0';
    }
}

/* Fungsi input angka */
static int input_angka(const char *pesan, int *hasil)
{
    printf("%s", pesan);

    if (scanf("%d", hasil) != 1) {
        bersihkan_input();
        return 0;
    }

    bersihkan_input();
    return 1;
}

/* Mencari indeks buku berdasarkan OCAID */
static int cari_indeks_buku(
    const Buku *buku,
    int jumlah_buku,
    const char *ocaid
)
{
    int i;

    for (i = 0; i < jumlah_buku; i++) {
        if (strcmp(buku[i].ocaid, ocaid) == 0) {
            return i;
        }
    }

    return -1;
}

/* 1. CREATE: Tambah Buku */
void admin_tambah_buku(Buku **buku, int *jumlah_buku)
{
    Buku baru;
    Buku *tmp;

    printf("\n===== TAMBAH BUKU =====\n");

    input_teks("ID Buku (ocaid) : ", baru.ocaid, sizeof(baru.ocaid));

    if (baru.ocaid[0] == '\0') {
        printf("ID buku tidak boleh kosong.\n");
        return;
    }

    if (cari_indeks_buku(*buku, *jumlah_buku, baru.ocaid) != -1) {
        printf("ID buku '%s' sudah ada.\n", baru.ocaid);
        return;
    }

    input_teks("Judul Buku      : ", baru.title, sizeof(baru.title));

    if (baru.title[0] == '\0') {
        strcpy(baru.title, "-");
    }

    input_teks("Penulis         : ", baru.author, sizeof(baru.author));

    if (baru.author[0] == '\0') {
        strcpy(baru.author, "-");
    }

    if (!input_angka("Tahun Terbit    : ", &baru.year) || baru.year <= 0) {
        printf("Tahun terbit tidak valid.\n");
        return;
    }

    if (!input_angka("Jumlah Halaman  : ", &baru.page_count) || baru.page_count <= 0) {
        printf("Jumlah halaman tidak valid.\n");
        return;
    }

    if (!input_angka("Stok            : ", &baru.stok) || baru.stok < 0) {
        printf("Stok tidak valid.\n");
        return;
    }

    tmp = (Buku *)realloc(
        *buku,
        (size_t)(*jumlah_buku + 1) * sizeof(Buku)
    );

    if (tmp == NULL) {
        printf("Gagal mengalokasikan memori buku baru.\n");
        return;
    }

    *buku = tmp;
    (*buku)[*jumlah_buku] = baru;
    (*jumlah_buku)++;

    if (simpan_buku(FILE_BUKU, *buku, *jumlah_buku)) {
        printf("\nBuku berhasil ditambahkan dan disimpan!\n");
    } else {
        printf("\nPeringatan: Buku ditambahkan di memori, tetapi gagal disimpan.\n");
    }
}

/* 2. READ: Tampilkan Semua Buku */
void admin_lihat_semua_buku(const Buku *buku, int jumlah_buku)
{
    int i;

    printf("\n===== DAFTAR BUKU =====\n");

    if (buku == NULL || jumlah_buku <= 0) {
        printf("Belum ada data buku.\n");
        return;
    }

    for (i = 0; i < jumlah_buku; i++) {
        printf("\nID      : %s\n", buku[i].ocaid);
        printf("Judul   : %s\n", buku[i].title);
        printf("Penulis : %s\n", buku[i].author);
        printf("Tahun   : %d\n", buku[i].year);
        printf("Halaman : %d\n", buku[i].page_count);
        printf("Stok    : %d\n", buku[i].stok);
    }
}

/* 3. UPDATE: Ubah Data Buku */
void admin_edit_buku(Buku *buku, int jumlah_buku)
{
    char id[PANJANG_OCAID];
    char buffer[PANJANG_JUDUL];
    int idx, angka;

    printf("\n===== UBAH DATA BUKU =====\n");

    input_teks("Masukkan ID buku (ocaid) yang ingin diubah: ", id, sizeof(id));

    idx = cari_indeks_buku(buku, jumlah_buku, id);

    if (idx == -1) {
        printf("\nBuku tidak ditemukan.\n");
        return;
    }

    printf(
        "Data saat ini -> Judul: %s | Penulis: %s | Tahun: %d | Halaman: %d | Stok: %d\n",
        buku[idx].title,
        buku[idx].author,
        buku[idx].year,
        buku[idx].page_count,
        buku[idx].stok
    );

    printf("(Tekan ENTER jika tidak ingin mengubah field teks)\n");

    input_teks("Judul baru      : ", buffer, sizeof(buffer));

    if (buffer[0] != '\0') {
        strncpy(buku[idx].title, buffer, PANJANG_JUDUL - 1);
        buku[idx].title[PANJANG_JUDUL - 1] = '\0';
    }

    input_teks("Penulis baru    : ", buffer, sizeof(buffer));

    if (buffer[0] != '\0') {
        strncpy(buku[idx].author, buffer, PANJANG_PENGARANG - 1);
        buku[idx].author[PANJANG_PENGARANG - 1] = '\0';
    }

    if (input_angka("Tahun baru      : ", &angka) && angka > 0) {
        buku[idx].year = angka;
    }

    if (input_angka("Halaman baru    : ", &angka) && angka > 0) {
        buku[idx].page_count = angka;
    }

    if (input_angka("Stok baru       : ", &angka) && angka >= 0) {
        buku[idx].stok = angka;
    }

    if (simpan_buku(FILE_BUKU, buku, jumlah_buku)) {
        printf("\nData buku berhasil diubah!\n");
    } else {
        printf("\nPeringatan: Gagal menyimpan perubahan.\n");
    }
}

/* 4. DELETE: Hapus Buku */
void admin_hapus_buku(Buku **buku, int *jumlah_buku)
{
    char id[PANJANG_OCAID];
    int idx, i;

    printf("\n===== HAPUS BUKU =====\n");

    input_teks("Masukkan ID buku (ocaid) yang ingin dihapus: ", id, sizeof(id));

    idx = cari_indeks_buku(*buku, *jumlah_buku, id);

    if (idx == -1) {
        printf("\nBuku tidak ditemukan.\n");
        return;
    }

    for (i = idx; i < *jumlah_buku - 1; i++) {
        (*buku)[i] = (*buku)[i + 1];
    }

    (*jumlah_buku)--;

    if (*jumlah_buku > 0) {
        Buku *tmp = (Buku *)realloc(
            *buku,
            (size_t)(*jumlah_buku) * sizeof(Buku)
        );

        if (tmp != NULL) {
            *buku = tmp;
        }
    } else {
        free(*buku);
        *buku = NULL;
    }

    if (simpan_buku(FILE_BUKU, *buku, *jumlah_buku)) {
        printf("\nBuku berhasil dihapus!\n");
    } else {
        printf("\nPeringatan: Gagal memperbarui file buku.\n");
    }
}

/* 5. LAPORAN PEMINJAMAN */
void admin_lihat_laporan_peminjaman(
    const Peminjaman *peminjaman,
    int jumlah_peminjaman
)
{
    int i;

    printf("\n===== LAPORAN PEMINJAMAN =====\n");

    if (peminjaman == NULL || jumlah_peminjaman <= 0) {
        printf("Belum ada data transaksi.\n");
        return;
    }

    for (i = 0; i < jumlah_peminjaman; i++) {
        printf("\nID Transaksi : %u\n", peminjaman[i].id_peminjaman);
        printf("Peminjam     : %s\n", peminjaman[i].username);
        printf("ID Buku      : %s\n", peminjaman[i].ocaid);
        printf("Tanggal      : %s\n", peminjaman[i].tanggal_pinjam);
        printf("Status       : %s\n", peminjaman[i].status);
    }
}

/* 6. MENU ADMIN */
void menu_admin(
    Buku **buku,
    int *jumlah_buku,
    const Peminjaman *peminjaman,
    int jumlah_peminjaman
)
{
    int pilihan = 0;

    do {
        printf("\n===== MENU ADMIN =====\n");
        printf("1. Tambah Buku\n");
        printf("2. Tampilkan Buku\n");
        printf("3. Ubah Buku\n");
        printf("4. Hapus Buku\n");
        printf("5. Urutkan Buku (Sorting)\n");
        printf("6. Laporan Peminjaman\n");
        printf("0. Keluar\n");

        if (!input_angka("Pilih menu: ", &pilihan)) {
            printf("\nPilihan tidak valid!\n");
            pilihan = -1;
            continue;
        }

        switch (pilihan) {
            case 1:
                admin_tambah_buku(buku, jumlah_buku);
                break;

            case 2:
                admin_lihat_semua_buku(*buku, *jumlah_buku);
                break;

            case 3:
                admin_edit_buku(*buku, *jumlah_buku);
                break;

            case 4:
                admin_hapus_buku(buku, jumlah_buku);
                break;

            case 5:
                menu_sort_books(*buku, *jumlah_buku);
                admin_lihat_semua_buku(*buku, *jumlah_buku);
                break;

            case 6:
                admin_lihat_laporan_peminjaman(
                    peminjaman,
                    jumlah_peminjaman
                );
                break;

            case 0:
                printf("\nKeluar dari menu admin.\n");
                break;

            default:
                printf("\nPilihan tidak valid!\n");
                break;
        }

    } while (pilihan != 0);
}
