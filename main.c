#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLEN 22
#define SCAP 23

#define IS_SMALL 0b0
#define IS_BIG 0b1

#define BIG_MASK (size_t)0x8000000000000000

#define cstring(_string)                                                       \
  _string.mode == IS_BIG ? _string.big_string.data : _string.small_string.data

typedef struct {
  size_t cap;
  size_t len;
  char *data;
} BigString;

typedef struct {
  uint8_t cap;
  char data[SLEN];
  uint8_t remaining;
} SmallString;

typedef union {
  uint8_t mode : 1;
  SmallString small_string;
  BigString big_string;
} String;

String str_new(size_t size) {
  if (size > SLEN) {
    char *data = (char *)calloc(size, sizeof(char));
    return (String){
        .big_string = {.cap = BIG_MASK | size + 1, .len = 0, .data = data}};
  }
  return (String){
      .small_string = {.cap = SCAP, .data = {0}, .remaining = SLEN}};
}

void str_del(String str) {
  if (str.mode == IS_BIG) {
    free(str.big_string.data);
  }
};

size_t get_len(String str) {
  if (str.mode == IS_BIG) {
    return str.big_string.len;
  }
  return SLEN - str.small_string.remaining;
}

size_t get_cap(String str) {
  if (str.mode == IS_BIG) {
    return str.big_string.cap & BIG_MASK;
  }
  return SLEN - str.small_string.remaining;
}

String str_from(const char *chars) {
  size_t len = strlen(chars);
  String str = str_new(len);
  memcpy(cstring(str), chars, len);
  return str;
}

int main() {}
