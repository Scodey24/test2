#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "file_io.h"
#include "auth.h"
#include "admin.h"
#include "user.h"

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    Buku *books = NULL;
    User *users = NULL;
    Peminjaman *loans = NULL;

    int book_count = 0;
    int user_count = 0;
    int loan_count = 0;

    if (!file_ada(FILE_BUKU)) {
        if (import_buku_asli(FILE_BUKU_ASLI, FILE_BUKU, STOK_DEFAULT) < 0) {
            printf("[Warning] %s tidak ditemukan, mulai dengan katalog kosong.\n", FILE_BUKU_ASLI);
        }
    }

    if (!muat_buku(FILE_BUKU, &books, &book_count)) {
        printf("[Warning] Gagal memuat %s.\n", FILE_BUKU);
    }
    if (!muat_user(FILE_USER, &users, &user_count)) {
        printf("[Warning] Gagal memuat %s.\n", FILE_USER);
    }
    if (!muat_peminjaman(FILE_PEMINJAMAN, &loans, &loan_count)) {
        printf("[Warning] Gagal memuat %s.\n", FILE_PEMINJAMAN);
    }

    int main_choice = 0;
    do {
        printf("\n============================================\n");
        printf("   SISTEM MANAJEMEN PERPUSTAKAAN (CLI)\n");
        printf("============================================\n");
        printf("1. Login\n");
        printf("2. Keluar\n");
        printf("Pilih menu: ");

        if (scanf("%d", &main_choice) != 1) {
            clear_stdin();
            printf("[Error] Masukkan angka 1 atau 2.\n");
            continue;
        }
        clear_stdin();

        if (main_choice == 1) {
            const User *active_user = proses_login(users, user_count);
            if (active_user != NULL) {
                if (strcmp(active_user->role, "admin") == 0) {
                    menu_admin(&books, &book_count, loans, loan_count);
                } else if (strcmp(active_user->role, "user") == 0) {
                    menu_user(active_user, books, book_count, &loans, &loan_count);
                } else {
                    printf("[Warning] Role '%s' tidak dikenali.\n", active_user->role);
                }
            }
        } else if (main_choice == 2) {
            simpan_buku(FILE_BUKU, books, book_count);
            simpan_user(FILE_USER, users, user_count);
            simpan_peminjaman(FILE_PEMINJAMAN, loans, loan_count);
            printf("\nTerima kasih telah menggunakan sistem. Sampai jumpa!\n");
        } else {
            printf("[Error] Pilihan tidak valid.\n");
        }
    } while (main_choice != 2);

    free(books);
    free(users);
    free(loans);

    return 0;
}