#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <windows.h>
#include "str_func.cpp"
#include "sort_func.cpp"

struct Line {
    char* line;
    size_t len = 0;
};

struct Text {
    Line* lines;
    char* buffer;
};

int   GetText      (const char* file, Text* text);
int   ReadFromFile (const char* file, char** buffer);
int   GetFileSize  (int file);
Line* GetStrings   (char** buffer, int cnt_strings, int read_elems);
int   WriteToFile  (Line* lines, const char* file, int size, int need_empty);
int   CmpStrUpBegin(const void* val1, const void* val2);
int   CmpStrUpEnd  (const void* val1, const void* val2);
int   CmpPtr       (const void* val1, const void* val2);

int main () {

    setlocale(LC_ALL, "Russian");
    

    Text text = {NULL, NULL};
    const int max_file_name = 300;
    char s[max_file_name];
    printf("¬ведите им€ файла(не более %d символов): ", max_file_name);
    scanf("%s", s);
    int cnt_strings = GetText(s, &text);

    //sort begin -----------------------------------------
    MyBubbleSort(text.lines, cnt_strings, sizeof(Line), CmpStrUpBegin);
    printf("¬ведите им€ файла куда запишетс€ сортировка по началу(не более %d символов): ", max_file_name);
    scanf("%s", s);
    WriteToFile(text.lines, s, cnt_strings, 0);
    printf("—ортировка по началу строки в файле успешно выполнена!\n");

    //sort end   -----------------------------------------
    MyQSort(text.lines, cnt_strings, sizeof(Line), CmpStrUpEnd);
    printf("¬ведите им€ файла куда запишетс€ сортировка по концу(не более %d символов): ", max_file_name);
    scanf("%s", s);
    WriteToFile(text.lines, s, cnt_strings, 0);
    printf("—ортировка по концу строки в файле успешно выполнена!\n");

    //sort ptr   -----------------------------------------
    qsort(text.lines, cnt_strings, sizeof(Line), CmpPtr);
    printf("¬ведите им€ файла куда запишетс€ исходный текст(не более %d символов): ", max_file_name);
    scanf("%s", s);
    WriteToFile(text.lines, "result_ptr.txt", cnt_strings, 1);
    printf("»сходный текст записан успешно получен<3\n");

    free(text.buffer);
    free(text.lines);

}

int GetText (const char* file, Text* text) {
    int read_elems = ReadFromFile(file, &(*text).buffer);
    assert(read_elems != -1);

    int cnt_strings = CntChar((*text).buffer, '\n', read_elems) + 1;

    (*text).lines = GetStrings(&(*text).buffer, cnt_strings, read_elems);
    return cnt_strings;
}

int ReadFromFile (const char* file, char** buffer) {
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

Line* GetStrings (char** buffer, int cnt_strings, int read_elems) {
    Line* lines = (Line*)calloc(cnt_strings, sizeof(Line));
    assert(lines != NULL);

    int ind = 1;
    lines[0].line = *buffer;
    for (int i = 0; i < read_elems; ++i) {
        if ((*buffer)[i] == '\r') {
            printf("o no:(");
        }
        if ((*buffer)[i] == '\n') {
            (*buffer)[i] = '\0';
            lines[ind].line = *buffer + i + 1;
            lines[ind - 1].len = lines[ind].line - lines[ind - 1].line - 1;
            ind += 1;
        }
    }
    lines[ind - 1].len = MyStrLen(lines[ind - 1].line);
    return lines;
}

int WriteToFile (Line* lines, const char* file, int size, int need_empty) {
    int fd = open(file, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        printf("Don't find %s", file);
        return -1;
    }

    for (int i = 0; i < size; ++i) {
        if (lines[i].len == 0 && !need_empty) {
            continue;
        }
        write(fd, lines[i].line, lines[i].len );
        write(fd, "\n", 1);
    }

    close(fd);
    return 1;
}

int CmpStrUpBegin (const void* val1, const void* val2) {
    const Line str1 = *(const Line*)val1;
    const Line str2 = *(const Line*)val2;

    int i = 0, j = 0;

    while (i < str1.len && j < str2.len) {
        while (i < str1.len && !isalpha(toupper(str1.line[i]))){
            i++;
        }
        while (j < str2.len && !isalpha(toupper(str2.line[j]))){
            j++;
        }
        if (i >= str1.len || j >=str2.len) {
            return (i >= str1.len ? (j >= str2.len ? 0 : -1) : 1);
        }
        if (tolower(str1.line[i]) != tolower(str2.line[j])){
            return tolower(str1.line[i]) - tolower(str2.line[j]);
        }
        i++;
        j++;
    }
    return str1.len - str2.len;
}

int CmpStrUpEnd (const void* val1, const void* val2) {
    const Line str1 = *(const Line*)val1;
    const Line str2 = *(const Line*)val2;

    int i = str1.len - 1, j = str2.len - 1;

    while (i >= 0 && j >= 0) {
        while (i >= 0 && !isalpha(toupper(str1.line[i]))){
            i--;
        }
        while (j >= 0 && !isalpha(toupper(str2.line[j]))){
            j--;
        }
        if (i < 0 || j < 0) {
            return (i < 0 ? (j < 0 ? 0 : -1) : 1);
        }
        if (toupper(str1.line[i]) != toupper(str2.line[j])){
            return toupper(str1.line[i]) - toupper(str2.line[j]);
        }
        i--;
        j--;
    }
    return str1.len - str2.len;

}

int CmpPtr (const void* val1, const void* val2) {
    const Line str1 = *(const Line*)val1;
    const Line str2 = *(const Line*)val2;
    return str1.line - str2.line;
}
