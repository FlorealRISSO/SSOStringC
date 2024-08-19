# String Optimization in C: A Proof of Concept

This repository presents a proof-of-concept implementation of an optimized string handling mechanism in C, inspired by Nicholas Ormrod's talk at CppCon 2016 titled ["The Strange Details of `std::string` at Facebook"](https://www.youtube.com/watch?v=kPR8h4-qZdk). The implementation showcases how string management can be optimized by embedding small strings directly into the structure, avoiding dynamic memory allocation for short strings.

## Overview

In modern C++ implementations, such as the `std::string` in the C++ Standard Library, a technique called "Small String Optimization" (SSO) is used. The core idea is that most strings are short enough to fit directly into the structure itself, thus eliminating the need for dynamic memory allocation and improving performance. This repository provides a similar mechanism in C, demonstrating how this optimization can be implemented.

## Features

- **Small String Optimization (SSO)**: Embeds small strings directly in the structure to avoid dynamic memory allocation.
- **Dynamic Allocation for Large Strings**: Uses heap allocation for larger strings, ensuring that memory is only allocated when necessary.
- **String Management Functions**: Includes functions to create, manipulate, and delete strings, both small and large.

## Implementation Details

### Data Structures

- **`SmallString`**: Stores small strings directly within the structure.
- **`LargeString`**: Handles large strings using dynamic memory allocation.
- **`String`**: A union that can be either a `SmallString` or `LargeString`, with a mode field to differentiate between the two.

### Key Macros

- **`get_cstring(string)`**: Accesses the underlying string data.
- **`get_string_length(string)`**: Retrieves the length of the string.
- **`get_string_capacity(string)`**: Returns the capacity of the string.
- **`set_string_length(string, length)`**: Sets the length of the string.
- **`string_delete(string)`**: Frees the memory allocated for large strings.

### Example Usage

The `main` function demonstrates how to use the provided functions and macros to manage strings:

```c
int main() {
  String str1 = string_from_cstr("Hello");
  String str2 = string_from_cstr("That's a very long sentence, sorry!");

  // Print the contents of the strings
  puts(get_cstring(str1));  // Output: Hello
  puts(get_cstring(str2));  // Output: That's a very long sentence, sorry!

  // Print the type of the strings
  printf("str1 is a %s string.\n", (str1.mode == MODE_LARGE) ? "large" : "small");
  printf("str2 is a %s string.\n", (str2.mode == MODE_LARGE) ? "large" : "small");

  // Clean up memory
  string_delete(str1);
  string_delete(str2);

  return 0;
}
```

## Building and Running

To build and run the provided example:

```sh
make
./main
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## Acknowledgements

This implementation is inspired by Nicholas Ormrod's presentation at CppCon 2016. The techniques demonstrated here are a simplified version of the concepts discussed in the talk.

Feel free to reach out with any questions or feedback.
