#ifndef SORTING_H
#define SORTING_H

#include "models.h"

typedef enum {
    SORT_INSERTION = 1,
    SORT_BUBBLE = 2,
    SORT_SELECTION = 3
} SortAlgorithm;

typedef enum {
    SORT_BY_OCAID = 1,
    SORT_BY_TITLE = 2,
    SORT_BY_YEAR = 3,
    SORT_BY_PAGES = 4,
    SORT_BY_STOCK = 5
} SortField;

typedef enum {
    SORT_DESC = 0,
    SORT_ASC = 1
} SortOrder;

void sort_books(Buku *arr, int n, SortAlgorithm algo, SortField field, SortOrder order);

void insertion_sort_books(Buku *arr, int n, SortField field, SortOrder order);
void bubble_sort_books(Buku *arr, int n, SortField field, SortOrder order);
void selection_sort_books(Buku *arr, int n, SortField field, SortOrder order);

void menu_sort_books(Buku *arr, int n);

#endif