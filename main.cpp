#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <windows.h>
#include "str_func.cpp"
#include "sort_func.cpp"

int ReadFromFile     (char** index, const char* file);
int CmpStringsUpBegin(void* val1, void* val2);
int CmpStringsUpEnd  (void* val1, void* val2);
int WriteToFile      (char** index, FILE* fp);

int main() {
    const int CNT_STRINGS = 6786;
    char** index = (char**)calloc(CNT_STRINGS, sizeof(char*));
    char** end = (char**)calloc(CNT_STRINGS, sizeof(char*));

    ReadFromFile(index, "Onegin.txt");

    for (int i = 0; i < CNT_STRINGS; ++i) {
        end[i] = index[i];
    }

    FILE* fp = fopen("result.txt", "w");

    MyBubbleSort(index, CNT_STRINGS, sizeof(char*), CmpStringsUpBegin);
    WriteToFile(index, fp);

    MyBubbleSort(index, CNT_STRINGS, sizeof(char*), CmpStringsUpEnd);
    WriteToFile(index, fp);

    WriteToFile(end, fp);


    fclose(fp);

}

int ReadFromFile(char** index, const char* file) {
    FILE* fp = fopen(file, "r");
    const int MAX_SIZE = 100;
    char buffer[MAX_SIZE];
    int ind = 0;
    while (fgets(buffer, MAX_SIZE, fp) != NULL) {
        index[ind] = MyStrDup(buffer);
        ++ind;
    }
    fclose(fp);
    return ind;
}

int WriteToFile (char** index, FILE* fp) {
    for (int i = 0; i < 6786; ++i) {
        fputs(index[i], fp);
    }
    return 1;
}

int CmpStringsUpBegin(void* val1, void* val2) {
    char* str1 = *(char**)val1;
    char* str2 = *(char**)val2;
    int len_str1 = MyStrLen(str1);
    int len_str2 = MyStrLen(str2);

    int i = 0, j = 0;

    while (i < len_str1 && j < len_str2) {
        while (i < len_str1 && !isalpha(str1[i])){
            i++;
        }
        while (j < len_str2 && !isalpha(str2[j])){
            j++;
        }
        if (i >= len_str1 && j >= len_str2) { 
            return 0;
        }
        if (i >= len_str1) {
            return 1;
        } 
        if (j >= len_str2){
            return -1;
        }
        if (str1[i] != str2[j]){
            return str2[j] - str1[i];
        }
        i++;
        j++;
    }
    return j - i;
}

int CmpStringsUpEnd(void* val1, void* val2) {
    char* str1 = *(char**)val1;
    char* str2 = *(char**)val2;
    int len_str1 = MyStrLen(str1);
    int len_str2 = MyStrLen(str2);

    int i = len_str1 - 1, j = len_str2 - 1;

    while (i >= 0 && j >= 0) {
        while (i >= 0 && !isalpha(str1[i])){
            i--;
        }
        while (j >= 0 && !isalpha(str2[j])){
            j--;
        }
        if (i < 0 && j < 0) { 
            return 0;
        }
        if (i < 0) {
            return 1;
        } 
        if (j < 0){
            return -1;
        }
        if (str1[i] != str2[j]){
            return str2[j] - str1[i];
        }
        i--;
        j--;
    }
    return j - i;

}