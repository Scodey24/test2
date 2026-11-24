#include <stdio.h>
#include <string.h>
#include "../include/auth.h"

#define MAKS_PERCOBAAN 3

static void baca_input(const char *label, char *tujuan, int ukuran)
{
    char *nl;
    int c;

    printf("%s", label);
    if (fgets(tujuan, ukuran, stdin) == NULL) {
        tujuan[0] = '\0';
        return;
    }
    nl = strchr(tujuan, '\n');
    if (nl != NULL) {
        *nl = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* buang sisa input yang kepanjangan */
        }
    }
}

const User *cari_user_login(const User *daftar, int jumlah,
                            const char *username, const char *password)
{
    int i;

    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].username, username) == 0 &&
            strcmp(daftar[i].password, password) == 0) {
            return &daftar[i];
        }
    }
    return NULL;
}

const User *proses_login(const User *daftar, int jumlah)
{
    char username[PANJANG_USERNAME];
    char password[PANJANG_PASSWORD];
    const User *hasil;
    int percobaan;

    for (percobaan = 1; percobaan <= MAKS_PERCOBAAN; percobaan++) {
        baca_input("Username: ", username, sizeof(username));
        baca_input("Password: ", password, sizeof(password));

        hasil = cari_user_login(daftar, jumlah, username, password);
        if (hasil != NULL) {
            printf("Login berhasil. Selamat datang, %s (%s)!\n", hasil->username, hasil->role);
            return hasil;
        }
        printf("Username/password salah (%d/%d).\n", percobaan, MAKS_PERCOBAAN);
    }
    return NULL;
}
