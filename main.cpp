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
    size_t len = 0;
};

struct Text {
    String* index;
    char* buffer;
};

int     GetText      (const char* file, Text* text);
int     ReadFromFile (const char* file, char** buffer);
int     GetFileSize  (int file);
String* GetStrings   (char** buffer, int cnt_strings, int read_elems);
int     WriteToFile  (String* index, const char* file, int size, int need_empty);
int     CmpStrUpBegin(const void* val1, const void* val2);
int     CmpStrUpEnd  (const void* val1, const void* val2);
int     CmpPtr       (const void* val1, const void* val2);

int main() {

    Text text = {NULL, NULL};
    int cnt_strings = GetText("Hamlet.txt", &text);

    //sort begin -----------------------------------------
    MyBubbleSort(text.index, cnt_strings, sizeof(String), CmpStrUpBegin);
    WriteToFile(text.index, "result_begin.txt", cnt_strings, 0);
    
    //sort end-------------------------------------------
    qsort(text.index, cnt_strings, sizeof(String), CmpStrUpEnd);
    WriteToFile(text.index, "result_end.txt", cnt_strings, 0);

    //sort ptr ------------------------------------------
    MyBubbleSort(text.index, cnt_strings, sizeof(String), CmpPtr);
    WriteToFile(text.index, "result_ptr.txt", cnt_strings, 1); 

    free(text.buffer);
    free(text.index);

}

int GetText (const char* file, Text* text) {
    int read_elems = ReadFromFile("Hamlet.txt", &(*text).buffer);
    assert(read_elems != -1);

    int cnt_strings = CntChar((*text).buffer, '\n', read_elems) + 1;

    (*text).index = GetStrings(&(*text).buffer, cnt_strings, read_elems); 
    return cnt_strings;
}

int ReadFromFile(const char* file, char** buffer) {
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        printf("Don't find %s\n", file);
        return -1;
    }
    
    int cnt_elems = GetFileSize(fd);

    *buffer = (char*)calloc(cnt_elems + 1, sizeof(char));
    assert(*buffer != NULL);
    int read_elems = read(fd, *buffer, cnt_elems);

    close(fd);

    (*buffer)[read_elems] = '\0';

    return read_elems;
}

int GetFileSize (int file) {
    struct stat statistic; 
    int ok = fstat(file, &statistic); 
    assert(ok != -1);
    return statistic.st_size;
}

String* GetStrings (char** buffer, int cnt_strings, int read_elems) {
    String* index = (String*)calloc(cnt_strings, sizeof(String));
    assert(index != NULL);

    int ind = 1;
    index[0].string = *buffer;
    for (int i = 0; i < read_elems; ++i) {
        if ((*buffer)[i] == '\r') {
            printf("o no:(");
        }
        if ((*buffer)[i] == '\n') {
            (*buffer)[i] = '\0';
            index[ind].string = *buffer + i + 1;
            index[ind - 1].len = MyStrLen(index[ind - 1].string);
            ind += 1;
        }
    }
    index[ind - 1].len = MyStrLen(index[ind - 1].string);
    return index;
}

int WriteToFile (String* index, const char* file, int size, int need_empty) {
    int fd = open(file, O_WRONLY);
    if (fd == -1) {
        printf("Don't find %s", file);
        return -1;
    }

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
        if (i >= str1.len || j >=str2.len) {
            return (i >= str1.len ? (j >= str2.len ? 0 : -1) : 1);
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
        if (i < 0 || j < 0) {
            return (i < 0 ? (j < 0 ? 0 : -1) : 1);
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