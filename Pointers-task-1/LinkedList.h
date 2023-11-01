#pragma once

#include <stdlib.h>
#include <string.h>
#include <iostream>

/* Initializes list */
void StringListInit(char*** list);

/* Returns the number of items in the list. */
int StringListSize(char*** const list);

/* Inserts value at the end of the list. */
void StringListAdd(char*** list, char* const str);

/* Destroy list and set pointer to NULL. */
void StringListDestroy(char*** list);

/* Removes all occurrences of str in the list. */
void StringListRemove(char** list, char* const str);

/* Returns the index position of the first occurrence of str in the list. */
int StringListIndexOf(char** list, const char* str);

/* Removes duplicate entries from the list. */
void StringListRemoveDuplicates(char** list);

/* Replaces every occurrence of the before, in each of the string lists's strings, with after. */
void StringListReplaceInStrings(char** list, const char* before, const char* after);

/* Sorts the list of strings in ascending order */
void StringListSort(char** list);

/* Print the list of strings */
void StringListDisplay(char** list);