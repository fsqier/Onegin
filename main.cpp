#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include "str_func.cpp"
#include "sort_func.cpp"

struct String {
    char* string;
    int len = 0;
};

int ReadFromFile (const char* file, char** buffer);
int TakeFileSize (int file);
int WriteToFile  (String* index, const char* file, int size, int need_empty);
int CmpStrUpBegin(const void* val1, const void* val2);
int CmpStrUpEnd  (const void* val1, const void* val2);
int CmpPtr       (const void* val1, const void* val2);

int main() {
     
    char* text = NULL;
    int read_elems = ReadFromFile("Hamlet.txt", &text);
    assert(read_elems != -1);

    text[read_elems] = '\0';

    int cnt_strings = CntChar(text, '\n', read_elems) + 1;

    String* index = (String*)calloc(cnt_strings, sizeof(String));
    assert(index != NULL);

    int ind = 1;
    index[0].string = text;
    for (int i = 0; i < read_elems; ++i) {
        if (text[i] == '\r') {
            printf("o no:(");
        }
        if (text[i] == '\n') {
            text[i] = '\0';
            index[ind].string = text + i + 1;
            index[ind - 1].len = MyStrLen(index[ind - 1].string);
            ind += 1;
        }
    }
    index[ind - 1].len = MyStrLen(index[ind - 1].string);

    //sort begin -----------------------------------------
    MyBubbleSort(index, cnt_strings, sizeof(String), CmpStrUpBegin);
    WriteToFile(index, "result_begin.txt", cnt_strings, 0);
    
    //sort end-------------------------------------------
    qsort(index, cnt_strings, sizeof(String), CmpStrUpEnd);
    WriteToFile(index, "result_end.txt", cnt_strings, 0);

    //sort ptr ------------------------------------------
    MyBubbleSort(index, cnt_strings, sizeof(String), CmpPtr);
    WriteToFile(index, "result_ptr.txt", cnt_strings, 1);

    free(text);
    free(index);

}

int ReadFromFile(const char* file, char** buffer) {
    int fd = open(file, O_RDONLY);
    assert(fd != -1);
    
    int cnt_elems = TakeFileSize(fd);

    *buffer = (char*)calloc(cnt_elems + 1, sizeof(char));
    assert(*buffer != NULL);
    int read_elems = read(fd, *buffer, cnt_elems);

    close(fd);

    return read_elems;
}

int TakeFileSize (int file) {
    struct stat statistic; 
    int ok = fstat(file, &statistic); 
    assert(ok != -1);
    return statistic.st_size;
}

int WriteToFile (String* index, const char* file, int size, int need_empty) {
    int fd = open(file, O_WRONLY);
    assert(fd != -1);

    for (int i = 0; i < size; ++i) {
        if (index[i].len == 0 && !need_empty) {
            continue;
        } 
        write(fd, index[i].string, index[i].len );
        write(fd, "\n", 1);
    }

    close(fd);
    return 1;
}

int CmpStrUpBegin(const void* val1, const void* val2) {
    const String str1 = *(const String*)val1;
    const String str2 = *(const String*)val2;

    int i = 0, j = 0;

    while (i < str1.len && j < str2.len) {
        while (i < str1.len && !isalpha(str1.string[i])){
            i++;
        }
        while (j < str2.len && !isalpha(str2.string[j])){
            j++;
        }
        if (i >= str1.len && j >= str2.len) { 
            return 0;
        }
        if (i >= str1.len) {
            return -1;
        } 
        if (j >= str2.len){
            return 1;
        }
        if (tolower(str1.string[i]) != tolower(str2.string[j])){
            return tolower(str1.string[i]) - tolower(str2.string[j]);
        }
        i++;
        j++;
    }
    return str1.len - str2.len;
}

int CmpStrUpEnd(const void* val1, const void* val2) {
    const String str1 = *(const String*)val1;
    const String str2 = *(const String*)val2;

    int i = str1.len - 1, j = str2.len - 1;

    while (i >= 0 && j >= 0) {
        while (i >= 0 && !isalpha(str1.string[i])){
            i--;
        }
        while (j >= 0 && !isalpha(str2.string[j])){
            j--;
        }
        if (i < 0 && j < 0) { 
            return 0;
        }
        if (i < 0) {
            return -1;
        } 
        if (j < 0){
            return 1;
        }
        if (tolower(str1.string[i]) != tolower(str2.string[j])){
            return tolower(str1.string[i]) - tolower(str2.string[j]);
        }
        i--;
        j--;
    }
    return str1.len - str2.len;

}

int CmpPtr (const void* val1, const void* val2) {
    const String str1 = *(const String*)val1;
    const String str2 = *(const String*)val2;
    return str1.string - str2.string;
}