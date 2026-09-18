#include <stdio.h>
#include <assert.h>
#include <stdint.h>

int*   InputArr      (int* arr, int size);
void*  MyBubbleSort  (void* arr, size_t size, size_t size_elem, int (*Cmp)(void* val1, void* val2));
void   MySwap        (void* val1, void* val2, size_t len);
void   PrintArrInt   (void* arr, size_t size);
void*  MyQSort       (void* arr, size_t l, size_t r, size_t size_elem, int (*Cmp)(void* val1, void* val2));
size_t PartitionHoaro(void* arr, size_t l, size_t r, size_t size_elem, int (*Cmp)(void* val1, void* val2));
int    CmpIntUp      (void* val1, void* val2);

int* InputArr (int* arr, int size) {
    assert(arr != NULL);

    printf("¬ведите эелементы массива в количестве %d штук: ", size);
    for(int i = 0; i < size; ++i) {
        scanf("%d", &arr[i]);
    }
    return arr;
}

void* MyBubbleSort(void* arr, size_t size, size_t size_elem, int (*Cmp)(void* val1, void* val2)) {
    assert(arr != NULL);

    unsigned char* buf = (unsigned char*)arr;

    for (size_t i = 0; i < size; ++i) {
        int cnt_change = 0;
        for (size_t j = 0; j < size - i - 1; ++j) {
            unsigned char* val1 = buf + j * size_elem;
            unsigned char* val2 = buf + (j + 1) * size_elem;
            if ((*Cmp)(val1, val2) < 0) {
                MySwap(val1, val2, size_elem);
                cnt_change += 1;
            }
        }
        if (!cnt_change) {
            return arr;
        }
    }
    return arr;
}

size_t PartitionHoaro(void* arr, size_t l, size_t r, size_t size_elem, int (*Cmp)(void* val1, void* val2)) {

    unsigned char* buf = (unsigned char*)arr;
    unsigned char* pivot = buf + (l + (r - l + 1) / 2) * size_elem;

    size_t i = l, j = r - 1;
    while (i <= j) {
        unsigned char * ptri = buf + i * size_elem;
        while ((Cmp(ptri, pivot) < 0)) {
            ++i;
            ptri += size_elem;
        } 
        unsigned char* ptrj = buf + j * size_elem;
        while ((Cmp(pivot, ptrj) < 0)) {
            --j;
            ptrj -= size_elem;
        }
        if (i >= j) break;
        MySwap(ptri, ptrj, size_elem);
        if (ptri == pivot) {
            pivot = ptrj;
        } else if (ptrj == pivot) {
            pivot = ptri;
        }
        ++i;
        --j;
    } 
    return j;

}

void* MyQSort (void* arr, size_t l, size_t r, size_t size_elem, int (*Cmp)(void* val1, void* val2)) {
    if (l + 1 >= r) {
        return arr;
    }
    
    size_t ind = PartitionHoaro(arr, l, r, size_elem, Cmp);

    MyQSort(arr, l, ind, size_elem, Cmp);
    MyQSort(arr, ind + 1, r, size_elem, Cmp);
    return arr;
}

void MySwap (void* val1, void* val2, size_t len) {
    assert(val1 != NULL);
    assert(val2 != NULL);

    uint64_t buf1;
    for (size_t i = 0; i < len / 8;) {
        buf1 = ((uint64_t*)val1)[i];
        ((uint64_t*)val1)[i] = ((uint64_t*)val2)[i];
        ((uint64_t*)val2)[i] = buf1;
    }

    uint32_t buf2;
    int blocks = len / 8 * 8;
    for (size_t i = 0; i < len % 8 / 4; ++i) {
        buf2 = ((uint32_t*)val1)[i];
        ((uint32_t*)val1 + blocks)[i] = ((uint32_t*)val2 + blocks)[i];
        ((uint32_t*)val2 + blocks)[i] = buf2;
    }

    uint16_t buf3;
    blocks = len / 8 / 4 * 32;
    for (size_t i = 0; i < len % 8 % 4 / 2; ++i) {
        buf3 = ((uint16_t*)val1)[i];
        ((uint16_t*)val1 + blocks)[i] = ((uint16_t*)val2 + blocks)[i];
        ((uint16_t*)val2 + blocks)[i] = buf3;
    }

    uint8_t buf4;
    blocks = len / 8 / 4 / 2 * 64;
    for (size_t i = 0; i < len % 8 % 4 % 2; ++i) {
        buf4 = ((uint8_t*)val1)[i];
        ((uint8_t*)val1 + blocks)[i] = ((uint8_t*)val2 + blocks)[i];
        ((uint8_t*)val2 + blocks)[i] = buf4;
    }
    
}

int CmpIntUp (void* val1, void* val2) {
    const int* val1_buf = (const int*)val1;
    const int* val2_buf = (const int*)val2;

    return *val2_buf - *val1_buf;
}

void PrintArrInt (void* arr, size_t size) {
    assert(arr != NULL);

    int* buf = (int*) arr;
    for(size_t i = 0; i < size; ++i) {
        printf("%d ", buf[i]);
    }
    printf("\n");
}