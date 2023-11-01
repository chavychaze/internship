#include <stdlib.h>
#include <string.h>
#include <iostream>

enum Node
{
	CURRENT,
	NEXT
};

/* Initializes list */
void StringListInit(char*** list)
{
	*list = (char**)malloc(sizeof(char*));
	(*list)[CURRENT] = nullptr;
}

/* Returns the number of items in the list. */
int StringListSize(char*** const list)
{
	size_t size = 0;
	while ((*list)[size] != nullptr)
		++size;

	return size;
}

/* Inserts value at the end of the list. */
void StringListAdd(char*** list, char* const str)
{
	size_t size = StringListSize(list);
	*list = (char**)realloc(*list, (size + 2) * sizeof(char*));
	(*list)[size] = (char*)malloc(strlen(str) + 1);
	if ((*list)[size] != nullptr)
	{
		strcpy_s((*list)[size], strlen(str) + 1, str);
		(*list)[size + 1] = nullptr;
	}
	// Handle memory allocation failure
	else
	{
		throw "Invalid argument - StringListAdd";
	}
}

/* Print the list of strings */
void StringListDisplay(char** list)
{
	int size = StringListSize(&list);
	for (int i = 0; i < size; ++i)
	{
		std::cout << list[i] << std::endl;
	}
	std::cout << "----" << std::endl;
}

/* Destroy list and set pointer to NULL. */
void StringListDestroy(char*** list)
{
	if (*list == nullptr)
		throw "Invalid argument - StringListDestroy";

	char** current = *list;

	while (*current != nullptr)
	{
		char* next = current[NEXT];
		free(*current);
		*current = nullptr;
		current = &next;
	}

	free(*list);
	*list = nullptr;
}

/* Removes all occurrences of str in the list. */
void StringListRemove(char** list, char* const str)
{
	int size = StringListSize(&list);
	for (int i = 0; i < size; ++i)
	{
		if (strcmp(list[i], str) == 0)
		{
			free(list[i]);
			for (int j = i; j < size - 1; ++j)
			{
				list[j] = list[j + 1];
			}
			list[size - 1] = nullptr;
			break;
		}
	}
}

/* Returns the index position of the first occurrence of str in the list. */
int StringListIndexOf(char** list, const char* str)
{
	if (list == nullptr || str == nullptr)
		throw "Invalid argument - StringListIndexOf";

	for (int index = 0; list[index] != nullptr; ++index) {
		if (strcmp(list[index], str) == 0) {
			return index;
		}
	}

	return -1;
}

/* Removes duplicate entries from the list. */
void StringListRemoveDuplicates(char** list)
{
	if (list == nullptr || *list == nullptr)
		throw "Invalid argument - StringListRemoveDuplicates";

	char** current = list;
	char** previous = nullptr;

	while (*current != nullptr)
	{
		char** compare = current;

		while (*compare != nullptr)
		{
			if (*previous != nullptr && strcmp(*current, *compare) == 0)
			{
				// Duplicate found, remove the current entry
				*previous = compare[NEXT];
				free(compare);
				char** temp = compare;
				*compare = nullptr; // Set the pointer to nullptr to avoid potential issues
				free(temp);
			}
			else
			{
				previous = compare;
				compare = &compare[NEXT];
			}
		}

		previous = current;
		current = &(current)[NEXT];
	}
}

/* Replaces every occurrence of the before, in each of the string lists's strings, with after. */
void StringListReplaceInStrings(char** list, const char* before, const char* after)
{
	if (list == nullptr || before == nullptr || after == nullptr)
		throw "Invalid argument - StringListReplaceInStrings";

	char** current = list;

	while (*current != nullptr)
	{
		if (strcmp(*current, before) == 0)
		{
			free(*current);
			*current = (char*)malloc(strlen(after) + 1);
			if (*current == nullptr)
				throw "Invalid argument - StringListReplaceInStrings - replace current[current]";

			// Copy the 'after' string into the current position
			strcpy_s(*current, strlen(after) + 1, after);
		}

		current = &(current)[NEXT];
	}
}

/* Sorts the list of strings in ascending order */
void StringListSort(char** list)
{
	if (list == nullptr || *list == nullptr)
		throw "Invalid argument - StringListSort";

	char** first_node = list;
	char** second_node = nullptr;
	bool swapped;

	do
	{
		swapped = false;
		second_node = &first_node[NEXT];

		while (second_node != nullptr)
		{
			if (strcmp(first_node[CURRENT], second_node[CURRENT]) > 0)
			{
				// Swap strings
				char* temp = first_node[CURRENT];
				first_node[CURRENT] = second_node[CURRENT];
				second_node[CURRENT] = temp;
				swapped = true;
			}

			second_node = &second_node[NEXT];
		}

		if (swapped)
		{
			first_node = list;
		}
	} while (swapped);
}