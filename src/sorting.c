#include <stdio.h>
#include <string.h>
#include "../include/sorting.h"

static int compare_books(const Buku *a, const Buku *b, SortField field, SortOrder order) {
    int cmp = 0;

    switch (field) {
        case SORT_BY_OCAID:
            cmp = strcmp(a->ocaid, b->ocaid);
            break;
        case SORT_BY_TITLE:
            cmp = strcmp(a->title, b->title);
            break;
        case SORT_BY_YEAR:
            if (a->year < b->year) cmp = -1;
            else if (a->year > b->year) cmp = 1;
            else cmp = 0;
            break;
        case SORT_BY_PAGES:
            if (a->page_count < b->page_count) cmp = -1;
            else if (a->page_count > b->page_count) cmp = 1;
            else cmp = 0;
            break;
        case SORT_BY_STOCK:
            if (a->stok < b->stok) cmp = -1;
            else if (a->stok > b->stok) cmp = 1;
            else cmp = 0;
            break;
        default:
            cmp = strcmp(a->ocaid, b->ocaid);
            break;
    }

    return (order == SORT_ASC) ? cmp : -cmp;
}

static void swap_books(Buku *a, Buku *b) {
    Buku temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort_books(Buku *arr, int n, SortField field, SortOrder order) {
    if (arr == NULL || n <= 1) return;

    for (int i = 1; i < n; i++) {
        Buku key = arr[i];
        int j = i - 1;

        while (j >= 0 && compare_books(&arr[j], &key, field, order) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bubble_sort_books(Buku *arr, int n, SortField field, SortOrder order) {
    if (arr == NULL || n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare_books(&arr[j], &arr[j + 1], field, order) > 0) {
                swap_books(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void selection_sort_books(Buku *arr, int n, SortField field, SortOrder order) {
    if (arr == NULL || n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        int target_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (compare_books(&arr[j], &arr[target_idx], field, order) < 0) {
                target_idx = j;
            }
        }
        if (target_idx != i) {
            swap_books(&arr[i], &arr[target_idx]);
        }
    }
}

void sort_books(Buku *arr, int n, SortAlgorithm algo, SortField field, SortOrder order) {
    if (arr == NULL || n <= 1) return;

    switch (algo) {
        case SORT_INSERTION:
            insertion_sort_books(arr, n, field, order);
            break;
        case SORT_BUBBLE:
            bubble_sort_books(arr, n, field, order);
            break;
        case SORT_SELECTION:
            selection_sort_books(arr, n, field, order);
            break;
        default:
            insertion_sort_books(arr, n, field, order);
            break;
    }
}

void menu_sort_books(Buku *arr, int n) {
    if (arr == NULL || n <= 0) {
        printf("\n[Info] Katalog buku masih kosong.\n");
        return;
    }

    int algo_in = 1, field_in = 1, order_in = 1;

    printf("\n=== PENGURUTAN KATALOG BUKU ===\n");
    printf("Pilih Algoritma:\n");
    printf("1. Insertion Sort\n");
    printf("2. Bubble Sort\n");
    printf("3. Selection Sort\n");
    printf("Pilihan: ");
    if (scanf("%d", &algo_in) != 1) {
        while (getchar() != '\n');
        algo_in = 1;
    }

    printf("\nPilih Atribut:\n");
    printf("1. ID Buku (ocaid)\n");
    printf("2. Judul Buku (title)\n");
    printf("3. Tahun Terbit (year)\n");
    printf("4. Jumlah Halaman (page_count)\n");
    printf("5. Stok Buku (stok)\n");
    printf("Pilihan: ");
    if (scanf("%d", &field_in) != 1) {
        while (getchar() != '\n');
        field_in = 1;
    }

    printf("\nPilih Urutan:\n");
    printf("1. Ascending (A-Z / Terkecil ke Terbesar)\n");
    printf("0. Descending (Z-A / Terbesar ke Terkecil)\n");
    printf("Pilihan: ");
    if (scanf("%d", &order_in) != 1) {
        while (getchar() != '\n');
        order_in = 1;
    }
    while (getchar() != '\n');

    SortAlgorithm algo = (algo_in >= 1 && algo_in <= 3) ? (SortAlgorithm)algo_in : SORT_INSERTION;
    SortField field = (field_in >= 1 && field_in <= 5) ? (SortField)field_in : SORT_BY_OCAID;
    SortOrder order = (order_in == 0) ? SORT_DESC : SORT_ASC;

    sort_books(arr, n, algo, field, order);
    printf("\n[Sukses] Katalog berhasil diurutkan.\n");
}