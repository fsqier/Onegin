#include <stdio.h>
#include <assert.h>
#include <stdint.h>

int*   InputArr      (int* arr, int size);
void*  MyBubbleSort  (void* arr, size_t size, size_t size_elem, int (*Cmp)(const void* val1, const void* val2));
void   MySwap        (void* val1, void* val2, size_t len);
void   PrintArrInt   (void* arr, size_t size);
void*  MyQSort       (void* arr, size_t l, size_t r, size_t size_elem, 
                        int (*Cmp)(const void* val1, const void* val2));
size_t PartitionHoaro(void* arr, size_t l, size_t r, size_t size_elem, 
                        int (*Cmp)(const void* val1, const void* val2));
int    CmpIntUp      (void* val1, void* val2);

int* InputArr (int* arr, int size) {
    assert(arr != NULL);

    printf("введите массив размером %d : ", size);
    for(int i = 0; i < size; ++i) {
        scanf("%d", &arr[i]);
    }
    return arr;
}

void* MyBubbleSort(void* arr, size_t size, size_t size_elem, int (*Cmp)(const void* val1, const void* val2)) {
    assert(arr != NULL);

    unsigned char* buf = (unsigned char*)arr;

    for (size_t i = 0; i < size; ++i) {
        int cnt_change = 0;
        for (size_t j = 0; j < size - i - 1; ++j) {
            unsigned char* val1 = buf + j * size_elem;
            unsigned char* val2 = buf + (j + 1) * size_elem;
            if ((*Cmp)(val1, val2) > 0) {
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

size_t PartitionHoaro(void* arr, size_t size, size_t size_elem, 
                        int (*Cmp)(const void* val1, const void* val2)) {

    unsigned char* buf = (unsigned char*)arr;
    unsigned char* pivot = buf + (size / 2 - 1) * size_elem;

    size_t i = 0, j = size - 1;
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

void* MyQSort (void* arr, size_t size_arr, size_t size_elem, 
                int (*Cmp)(const void* val1, const void* val2)) {
    if (size_arr < 2) {
        return arr;
    }
    
    size_t ind = PartitionHoaro(arr, size_arr, size_elem, Cmp);

    MyQSort(arr, ind + 1, size_elem, Cmp);
    MyQSort((char*)arr + (ind + 1) * size_elem, size_arr - (ind + 1), size_elem, Cmp);
    return arr;
}

void MySwap (void* val1, void* val2, size_t len) {
    assert(val1 != NULL);
    assert(val2 != NULL);

    for (size_t i = 0; i < len / 8; ++i) {
        uint64_t buf1 = ((uint64_t*)val1)[i];
        ((uint64_t*)val1)[i] = ((uint64_t*)val2)[i];
        ((uint64_t*)val2)[i] = buf1;
    }

    int blocks = len / 8 * 8;
    if (len % 8 / 4 > 0) {
        uint32_t buf2 = ((uint32_t*)val1)[0];
        ((uint32_t*)val1 + blocks)[0] = ((uint32_t*)val2 + blocks)[0];
        ((uint32_t*)val2 + blocks)[0] = buf2;
    }

    blocks = len / 8 / 4 * 32;
    if (len % 8 % 4 / 2 > 0) {
        uint16_t buf3 = ((uint16_t*)val1)[0];
        ((uint16_t*)val1 + blocks)[0] = ((uint16_t*)val2 + blocks)[0];
        ((uint16_t*)val2 + blocks)[0] = buf3;
    }

    blocks = len / 8 / 4 / 2 * 64;
    if (len % 8 % 4 % 2 > 0) {
        uint8_t buf4 = ((uint8_t*)val1)[0];
        ((uint8_t*)val1 + blocks)[0] = ((uint8_t*)val2 + blocks)[0];
        ((uint8_t*)val2 + blocks)[0] = buf4;
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