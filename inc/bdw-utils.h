/* Copyright (c) 2016 Ian Hernandez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef UTIL__H
#define UTIL__H

#include <stdbool.h>
#include <stdio.h>

/** Definition of the generic pointer data-type. */
typedef void * pointer;

/** Definition of the character string data-type. */
typedef char * string;

/** Struct that stores an array of elements and its length. */
typedef struct _BdwArray
{
  pointer elements; /**< Elements of the array. */
  unsigned int len; /**< Array's length. */
} BdwArray;


/**
 * Function used to concatenate the given strings.
 *
 * @param arg NULL-terminated series of strings.
 * @return The concatenation of the strings given.
 */
string bdw_utils_strconcat (const char * arg, ...);

/**
 * Function that creates a copy of the given string.
 *
 * @param arg String to be copied.
 * @return An exact copy of the given string.
 */
string bdw_utils_strcopy (const string arg);

/**
 * Function that compares two strings.
 *
 * @param arg1 One of string to be compared.
 * @param arg2 The string to be compared with arg1.
 * @return
 *    - @ref true if the strings are the same.
 *    - @ref false otherwise.
 */
bool bdw_utils_strequals (const string arg1, const string arg2);

/**
 * Function that trims a string at the first occurrence of a given character.
 *
 * @param str String to be trimmed.
 * @param delim Character used as delimiter for the trimming.
 */
void bdw_utils_strtrim (const string str, char delim);

/**
 * Function that appends a character to a given string.
 *
 * @param dest String to be processed.
 * @param c Character to be appended.
 */
void bdw_utils_strappendchr (string dest, char c);

/**
 * Function used to tokenize (or split) a given string by a given delimiter.
 *
 * @param str String to be tokenized (split).
 * @param delim String used as a delimiter for the tokenization.
 * @return The NULL-terminated array of strings result of the tokenization.
 */
string * bdw_utils_strsplit (const string str, const string delim);


/**
 * Function that obtains the length of a NULL-terminated array of elements.
 *
 * @param array Pointer to the NULL-terminated array to be processed.
 * @return The length of the given array.
 */
int bdw_utils_nullarraylen (pointer array);

/**
 * Function that creates and initializes a new BdwArray struct.
 *
 * @return A pointer to the newly created BdwArray struct.
 */
BdwArray * bdw_array_new (void);

/**
 * Function that destroys and existing BdwArray struct.
 *
 * @param self Pointer to the BdwArray struct to be destroyed.
 */
void bdw_array_destroy (BdwArray * self);


/**
 * Function that reads a line from a text file.
 *
 * @param file Text file from which the line will be read.
 * @return The line of text that was read from the file.
 */
string bdw_utils_filegetline (FILE * file);

#endif /* UTIL__H */
