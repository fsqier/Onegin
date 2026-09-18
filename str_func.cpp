#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>


int    MyStrLen  (const char* str);
int    MyStrNLen (const char* str, int max_len);
int    MyPuts    (const char* str);
char*  MyStrCpy  (char* str1, const char* str2);
char*  MyStrNCpy (char* str1, const char* str2, int max_len);
char*  MyStrCat  (char* str1, const char* str2);
char*  MyStrNCat (char* str1, const char* str2, int max_len);
int    MyStrCmp  (const char* str1, const char* str2); 
int    MyStrNCmp (const char* str1, const char* str2, int max_len);
char*  MyStrStr  (char* str1, char* str2);
size_t MyGetLine (char** str, size_t* max_len, FILE* stream);
char*  MyStrDup  (const char* str); 
char*  MyStrNDup (const char* str, int max_len);
char*  MyStrChr  (char* str, int ch);
char*  MyStrNChr (char* str, int ch, int max_len);
char*  MyStrRChr (char* str, int ch);
char*  MyStrRNChr(char* str, int ch, int max_len);
void*  MyMemCpy  (void* to, const void* from, size_t len);
void*  MyMemMove (void* to, void* from, size_t len);

int MyStrLen(const char* str) {
    assert(str != NULL);

    int len = 0;
    while (str[len++] != '\0');

    return len - 1;
}

int MyStrNLen(const char* str, int max_len) {
    assert(str != NULL);

    int len = 0;
    while (str[len] != '\0' && len < max_len) {
        len += 1;
    }
    return (len < max_len ? len - 1 : max_len);

}

int MyPuts(const char* str) {
    assert(str != NULL);

    int ind = 0;
    while (str[ind] != '\0') {
        putchar(str[ind++]);
    }
    putchar('\n');

    return 1;
}

char* MyStrCpy(char* str1, const char* str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind = 0;
    while (str2[ind] != '\0') {
        str1[ind] = str2[ind];
        ++ind;
    }
    str1[ind] = '\0';

    return str1;
}

char* MyStrNCpy(char* str1, const char* str2, int max_len) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind = 0;
    while (ind < (max_len - 1) && str2[ind] != '\0') {
        str1[ind] = str2[ind];
        ++ind;
    }
    str1[ind] = str2[ind];
    ind += 1;

    while (ind < max_len) {
        str1[ind++] = '\0';
    }

    return str1;
}

char* MyStrCat(char* str1, const char* str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind1 = 0;
    while (str1[ind1++] != '\0');

    int ind2 = 0;
    while (str2[ind2] != '\0') {
        str1[ind1++] = str2[ind2++];
    }
    
    str1[ind1] = '\0';

    return str1;
}

char* MyStrNCat(char* str1, const char* str2, int max_len) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind1 = 0;
    while (str1[ind1++] != '\0');

    int ind2 = 0;
    while (str2[ind2] != '\0' && ind2 < max_len) {
        str1[ind1++] = str2[ind2++];
    }
    
    str1[ind1] = '\0';

    return str1;
}

int MyStrCmp(const char* str1, const char* str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind = 0;
    while (str1[ind] != '\0' && str2[ind] != '\0') {
        if (str1[ind] != str2[ind]) {
            return 0;
        }
        ++ind;
    }

    return (str1[ind] - str2[ind]);
} 

int MyStrNCmp(const char* str1, const char* str2, int max_len) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind = 0;
    while (str1[ind] != '\0' && str2[ind] != '\0' && ind < (max_len - 1)) {
        if (str1[ind] != str2[ind]) {
            return str1[ind] - str2[ind];
        }
        ++ind;
    }

    return (str1[ind] - str2[ind]);
}

char* MyStrStr (char* str1, char* str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ind1 = 0, ind2 = 0, save_ind = 0;
    while (str1[ind1] != '\0') {
        ind2 = 0;
        save_ind = ind1;
        while (str1[ind1] != '\0' && str1[ind1] == str2[ind2]) {
            ind2 += 1;
            ind1 += 1;
            if (str2[ind2] == '\0') {
                return (str1 + save_ind);
            }
        }
        ind1 = save_ind + 1;
    }
    return NULL;
}

size_t MyGetLine(char** str, size_t* max_len, FILE* stream) {
    assert(str != NULL);
    assert(max_len != NULL);
    assert(stream != NULL);
    if (*max_len == 0 || *str == NULL) {
        *max_len = 10;
        *str = (char*)calloc(*max_len, sizeof(char));
        if (*str == NULL) {
            return -1;
        }
    }

    int ch = fgetc(stream);
    size_t ind = 0;
    while (ch != EOF && ch != '\n') {
        if (ind >= *max_len - 1) {
            *max_len *= 2;
            char* buf = (char*)realloc(*str, *max_len);
            if (buf == NULL) {
                return -1;
            }
            *str = buf;
        }
        (*str)[ind++] = ch;
        ch = fgetc(stream);
    }

    (*str)[ind] = '\0'; 

    return ind + 1;
}

char* MyStrDup (const char* str) {
    assert(str != NULL);
    char* str_copy = (char*)calloc(MyStrLen(str) + 1, sizeof(char));
    if (str_copy == NULL) {
        return NULL;
    }
    MyMemCpy(str_copy, str, MyStrLen(str) + 1);

    return str_copy;

}
  
char* MyStrNDup(const char* str, int max_len) {
    assert(str != NULL);

    char* str_copy = (char*)calloc(max_len + 1, sizeof(char));
    if (str_copy == NULL) {
        return NULL;
    }
    MyMemCpy(str_copy, str, max_len);
    return str_copy;

}

char* MyStrChr (char* str, int ch) {
    assert(str != NULL);

    int ind = 0;
    while (str[ind] != '\0') {
        if (str[ind] == (char)ch) {
            return (str + ind);
        }
        ind++;
    }
    return NULL;
}

char* MyStrNChr (char* str, int ch, int max_len) {
    assert(str != NULL);

    int ind = 0;
    while (str[ind] != '\0' && ind < max_len) {
        if (str[ind] == (char)ch) {
            return (str + ind);
        }
        ind++;
    }
    return NULL;
}

char* MyStrRChr (char* str, int ch) {
    assert(str != NULL);

    int ind = 0;
    char* last = NULL;
    while (str[ind] != '\0') {
        if (str[ind] == (char)ch) {
            last = (str + ind);
        }
        ind++;
    }
    return last;
}

char* MyStrRNChr (char* str, int ch, int max_len) {
    assert(str != NULL);

    int ind = 0;
    char* last = NULL;
    while (str[ind] != '\0' && ind < max_len) {
        if (str[ind] == (char)ch) {
            last = (str + ind);
        }
        ind++;
    }
    return last;
}

void* MyMemCpy  (void* to, const void* from, size_t len) {    
    assert(to != NULL);
    assert(from != NULL);

    uint64_t* to_buf = (uint64_t*) to;
    const uint64_t* from_buf = (const uint64_t*) from;

    for (size_t i = 0; i < len / 8; ++i) {
        to_buf[i] = from_buf[i];
    }

    char* to_mod = (char*)( to_buf + len / 8);
    const char* from_mod = (const char*)(from_buf + len / 8 );
    for (size_t i = 0; i < len % 8; ++i) {
        to_mod[i] = from_mod[i];
    }

    return to;
}

void* MyMemMove (void* to, void* from, size_t len) {
    assert(to != NULL);
    assert(from != NULL);

    if (!(from < to && to < from + len)) {
        return MyMemCpy(to, from , len);
    }

    char* to_buf = (char*) to;
    char* from_buf = (char*) from;

    for(size_t i = 0; i < len; ++i) {
        to_buf[len - i - 1] = from_buf[len - i - 1];
    }

    return to;
}
