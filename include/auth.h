#ifndef AUTH_H
#define AUTH_H

#include "models.h"

/* Cek username+password. Return pointer ke User yang cocok, atau NULL. */
const User *cari_user_login(const User *daftar, int jumlah,
                            const char *username, const char *password);

/* Tampilkan form login (maks 3 percobaan). Return User yang login, atau NULL. */
const User *proses_login(const User *daftar, int jumlah);

#endif
