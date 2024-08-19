// MIT License
//
// Copyright (c) 2024 Floréal Risso
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for small string length and capacity
#define SMALL_STRING_LEN 22
#define SMALL_STRING_CAP 23

// Mode flags to differentiate between small and large strings
#define MODE_SMALL 0b0
#define MODE_LARGE 0b1

// Mask to extract capacity from the size_t type for large strings
#define LARGE_STRING_MASK (size_t)0x8000000000000000

#define get_cstring(_string)                                                     \
  ((_string).mode == MODE_LARGE ? (_string).large_string.data : (_string).small_string.data)

#define get_string_length(_string)                                               \
  ((_string).mode == MODE_LARGE ? (_string).large_string.length : SMALL_STRING_LEN - (_string).small_string.remaining_space)

#define get_string_capacity(_string)                                             \
  ((_string).mode == MODE_LARGE ? (_string).large_string.capacity & ~LARGE_STRING_MASK : SMALL_STRING_CAP)

#define set_string_length(_string, _length)                                       \
  do {                                                                            \
    if ((_string).mode == MODE_LARGE) {                                           \
      (_string).large_string.length = (_length);                                  \
    } else {                                                                      \
      (_string).small_string.remaining_space = SMALL_STRING_LEN - (_length);      \
    }                                                                             \
  } while (0)

#define string_delete(_string)                                                   \
  do {                                                                           \
    if ((_string).mode == MODE_LARGE) {                                          \
      free((_string).large_string.data);                                         \
    }                                                                            \
  } while (0)

typedef struct {
	size_t capacity;
	size_t length;
	char *data;
} LargeString;

typedef struct {
	uint8_t capacity;
	char data[SMALL_STRING_LEN];
	uint8_t remaining_space;
} SmallString;

// Union that can hold either a small or a large string
typedef union {
	uint8_t mode : 1;  // Mode to indicate if the string is small or large
	SmallString small_string;
	LargeString large_string;
} String;

String
string_create(size_t size)
{
	String str;

	// Allocate as large string if the size exceeds the small string limit
	if (size > SMALL_STRING_LEN) {
		char *data = (char *)calloc(size + 1, sizeof(char));
		str = (String) {
			.large_string = {.capacity = LARGE_STRING_MASK | (size + 1), .length = 0, .data = data}
		};
		str.mode = MODE_LARGE;
	} else {
		// Create a small string
		str = (String) {
			.small_string = {.capacity = SMALL_STRING_CAP, .data = {0}, .remaining_space = SMALL_STRING_LEN}
		};
		str.mode = MODE_SMALL;
	}

	return str;
}

String
string_from_cstr(const char *cstr)
{
	size_t len = strlen(cstr);
	String str = string_create(len);
	memcpy(get_cstring(str), cstr, len);
	set_string_length(str, len);
	return str;
}

int
main()
{
	String str1 = string_from_cstr("Hello");
	String str2 = string_from_cstr("That's a very long sentence, sorry!");

	printf("\"%s\" is a %s string.\n", get_cstring(str1), (str1.mode == MODE_LARGE) ? "large" : "small");
	printf("\"%s\" is a %s string.\n", get_cstring(str2), (str2.mode == MODE_LARGE) ? "large" : "small");

	string_delete(str1);
	string_delete(str2);

	return 0;
}

