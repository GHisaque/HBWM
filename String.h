#pragma once

#ifndef GIP_STRING_H
#define GIP_STRING_H

#include <stdlib.h>
#include <stdbool.h>
#include "./Types.h"

typedef struct {
    const char* text;
} String;

typedef struct {
    String items[2];
} DblStr;

static inline const char* ccs_from_char(const char input) {
    return (const char*)input;
}

static inline uint String_Get_Length(const char* str) {
    if (!str) return 0;
    uint i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

static inline uint String_Get_Length_String(String str) {
    return String_Get_Length(str.text);
}

static inline String String_Join_String(String str1, String str2) {
    static char scratch[4][256];
    static int slot = 0;

    char* buf = scratch[slot];
    slot = (slot + 1) % 4;

    uint len1 = 0;
    while (str1.text[len1] != '\0' && len1 < 255) {
        buf[len1] = str1.text[len1];
        len1++;
    }

    uint len2 = 0;
    while (str2.text[len2] != '\0' && (len1 + len2) < 255) {
        buf[len1 + len2] = str2.text[len2];
        len2++;
    }

    buf[len1 + len2] = '\0';
    return (String){buf};
}

static inline String String_Join(String str1, const char* str2) {
    static char scratch[4][256];
    static int slot = 0;

    char* buf = scratch[slot];
    slot = (slot + 1) % 4;

    uint len1 = 0;
    while (str1.text[len1] != '\0' && len1 < 255) {
        buf[len1] = str1.text[len1];
        len1++;
    }

    uint len2 = 0;
    while (str2[len2] != '\0' && (len1 + len2) < 255) {
        buf[len1 + len2] = str2[len2];
        len2++;
    }

    buf[len1 + len2] = '\0';
    return (String){buf};
}

static inline bool String_Contains(const char* target, String src) {
    if (!target || !src.text) return false;

    uint target_len = String_Get_Length(target);
    uint src_len = String_Get_Length_String(src);

    if (target_len == 0) return true;
    if (target_len > src_len) return false;

    for (uint i = 0; i <= src_len - target_len; i++) {
        uint j = 0;
        while (j < target_len && src.text[i + j] == target[j]) {
            j++;
        }
        if (j == target_len) return true;
    }

    return false;
}

static inline bool String_Contains_String(String target, String src) {
    return String_Contains(target.text, src);
}

static inline String* String_Split(const char splitter, String src) {
    const char* ref = src.text;
    String* result = {0};
    uint i = 0;
    uint n = String_Get_Length_String(src);
    uint li = 0;

    while (i < n) {
        const char c = src.text[i];
        String temp;

        if (c == splitter) {
            result[li++] = temp;
            temp.text = "";
        } else {
            temp = String_Join(temp, ccs_from_char(c));
        }

        i++;
    }

    return result;
}

static inline DblStr String_Split_From_Index_Point(const char target, uint offset, String src) {
    DblStr result = {0};
    
    static char split_scratch[2][256];
    static int pair_slot = 0;
    
    char* buf0 = split_scratch[pair_slot];
    char* buf1 = split_scratch[(pair_slot + 1) % 2];
    pair_slot = (pair_slot + 1) % 2;

    uint n = 0;
    while (src.text[n] != '\0' && n < 255) {
        n++;
    }
    
    uint match_count = 0;
    bool split_triggered = false;
    uint len0 = 0;
    uint len1 = 0;

    for (uint i = 0; i < n; i++) {
        const char c = src.text[i];

        if (c == target && !split_triggered) {
            if (match_count == offset) {
                split_triggered = true;
                continue;
            }
            match_count++;
        }

        if (!split_triggered) {
            if (len0 < 255) buf0[len0++] = c;
        } else {
            if (len1 < 255) buf1[len1++] = c;
        }
    }

    buf0[len0] = '\0';
    buf1[len1] = '\0';

    result.items[0] = (String){buf0};
    result.items[1] = (String){buf1};

    return result;
}

static inline uint String_Get_List_Length(String* list) {
    uint i = 0;

    while (list[i].text != NULL) i++;

    return i;
}

#endif