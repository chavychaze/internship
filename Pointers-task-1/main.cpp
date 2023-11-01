#include "LinkedList.h"

int main() {
	char firstElemen[] = "FirstElement";
	char secondElement[] = "SecondElement";
	char thirdElement[] = "ThirdElement";
	char fourthElement[] = "FourthElement";
	char elementReplace[] = "element";
	char search[] = "search";

	char** list;

	StringListInit(&list);
	StringListAdd(&list, firstElemen);
	StringListRemove(list, firstElemen);
	StringListDisplay(list);
	StringListAdd(&list, secondElement);
	StringListAdd(&list, thirdElement);
	StringListDisplay(list);
	StringListRemove(list, secondElement);
	StringListDisplay(list);
	std::cout << StringListSize(&list) << std::endl;
	StringListAdd(&list, fourthElement);
	StringListDisplay(list);
	std::cout << StringListSize(&list) << std::endl;
	std::cout << StringListIndexOf(list, search) << std::endl;
	StringListAdd(&list, secondElement);
	StringListAdd(&list, thirdElement);
	StringListSort(list);
	StringListReplaceInStrings(list, thirdElement, elementReplace);
	StringListDisplay(list);
	StringListRemoveDuplicates(list);
	StringListDisplay(list);
	StringListDestroy(&list);
	return 0;
}