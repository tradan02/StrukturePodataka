#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define EMPTY_LISTS (-2)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int coefficient;
	int exponent;
	Position next;
	Position prev;
}Element;

int AddElementToStart(Position Head, int coefficient, int exponent);
int AddElementToEnd(Position Head, int coefficient, int exponent);
int PrintList(Position HeadNext);
int PrintLists(Position HeadNext1, Position HeadNext2);
int AddElementBeforeByValue(Position Head, int target_coefficient, int target_exponent, int coefficient, int exponent);
int AddElementBeforeByPosition(Position Head, Position Target, int coefficient, int exponent);
int AddElementAfterByValue(Position Head, int target_coefficient, int target_exponent, int coefficient, int exponent);
int AddElementAfterByPosition(Position Head, Position Target, int coefficient, int exponent);
int chartoint(char c);
int ReadFromFile(Position Head1, Position Head2, char* filename);
int PolyAddition(Position Head1, Position Head2, Position PoliRes);
int AddElementSorted(Position Head, int coefficient, int exponent);
int PolyMultiplication(Position Result, Position head1next, Position head2next);

Position AllocateMemory();
Position FindElementByValue(Position Head, int coefficient, int exponent);

int main() {
	Element headPoly1 = { .coefficient = 0, .exponent = 0, .next = NULL ,.prev = NULL };
	Element headPoly2 = { .coefficient = 0, .exponent = 0, .next = NULL ,.prev = NULL };
	Element PoliRes1 = { .coefficient = 0, .exponent = 0, .next = NULL ,.prev = NULL };
	Element PoliRes2 = { .coefficient = 0, .exponent = 0, .next = NULL ,.prev = NULL };

	ReadFromFile(&headPoly1, &headPoly2, "polynomes.txt");
	PrintLists(headPoly1.next, headPoly2.next);
	PolyAddition(headPoly1.next, headPoly2.next, &PoliRes1);
	PolyMultiplication(&PoliRes2, headPoly1.next, headPoly2.next);
	PrintList(PoliRes1.next);

	return 0;
}

int AddElementSorted(Position Head, int coefficient, int exponent) {
	Position OriginalHead = Head;
	Position NewPoly = NULL;
	NewPoly = AllocateMemory();

	if (!NewPoly)
		return 0;

	NewPoly->coefficient = coefficient;
	NewPoly->exponent = exponent;

	while (Head != NULL && Head->exponent < exponent) {
		Head = Head->next;
	}

	if (Head == NULL)
		AddElementToEnd(OriginalHead, coefficient, exponent);

	AddElementBeforeByPosition(OriginalHead, Head, coefficient, exponent);

	return 0;
}

int PolyMultiplication(Position Result, Position head1next, Position head2next)
{
	if (head1next != NULL || head2next != NULL) {
		for (Position currentPoly1 = head1next; currentPoly1 != NULL; currentPoly1 = currentPoly1->next) {
			for (Position currentPoly2 = head2next; currentPoly2 != NULL; currentPoly2 = currentPoly2->next) {
				AddElementSorted(Result, currentPoly1->coefficient * currentPoly2->coefficient, currentPoly1->exponent + currentPoly2->exponent);
			}
		}
		return 1;
	}
	return 0;
}

int PolyAddition(Position Head1, Position Head2, Position PoliRes) {
	Position currHead1 = Head1;
	Position currHead2 = Head2;
	Position Result = PoliRes;
	Position remainingPoly = NULL;

	while (currHead1 != NULL && currHead2 != NULL) {
		if (currHead1->exponent == currHead2->exponent) {
			AddElementAfterByPosition(PoliRes, Result, currHead1->coefficient + currHead2->coefficient, currHead1->exponent);
			currHead1 = currHead1->next;
			currHead2 = currHead2->next;
			Result = Result->next;
		}
		else if (currHead1->exponent < currHead2->exponent) {
			AddElementAfterByPosition(PoliRes, Result, currHead1->coefficient, currHead1->exponent);
			currHead1 = currHead1->next;
			Result = Result->next;
		}
		else {
			AddElementAfterByPosition(PoliRes, Result, currHead2->coefficient, currHead2->exponent);
			currHead2 = currHead2->next;
			Result = Result->next;
		}
	}
	if (currHead1 == NULL) {
		remainingPoly = currHead2;
	}
	else {
		remainingPoly = currHead1;
	}
	while (remainingPoly != NULL) {
		AddElementAfterByPosition(PoliRes, Result, remainingPoly->coefficient, remainingPoly->exponent);
		remainingPoly = remainingPoly->next;
		Result = Result->next;
	}

	return 0;
}

int ReadFromFile(Position Head1, Position Head2, char* filename) {
	FILE* filepointer;
	filepointer = fopen(filename, "r");
	if (!filepointer) {
		printf("Neuspješno otvaranje");
		return 0;
	}
	int counter = 0;
	for (counter = 0; counter < 2; counter++) {
		char buffer[MAX_LINE] = { 0 };
		fgets(buffer, MAX_LINE, filepointer);
		int i = 0;
		char prev = '\0';
		while (buffer[i + 1] != '\0') {
			prev = buffer[i];
			if (buffer[i + 1] == 'x') {
				if (counter == 0) {
					AddElementSorted(Head1, chartoint(prev), chartoint(buffer[i + 3]));
				}
				else {
					AddElementSorted(Head2, chartoint(prev), chartoint(buffer[i + 3]));
				}
			}
			i++;
		}
	}
	return 1;
}

int chartoint(char c) {
	return c - '0';
}

int AddElementBeforeByValue(Position Head, int target_coefficient, int target_exponent, int coefficient, int exponent) {
	Position target = FindElementByValue(Head, target_coefficient, target_exponent);
	if (!target) {
		printf("Neuspješno dodavanje");
	}
	else {
		AddElementBeforeByPosition(Head, target, coefficient, exponent);
	}
}

int AddElementBeforeByPosition(Position Head, Position Target, int coefficient, int exponent) {
	if (!Target) {
		return 0;
	}
	Position NewPoly = NULL;
	NewPoly = AllocateMemory();

	if (!NewPoly)
		return 0;

	NewPoly->coefficient = coefficient;
	NewPoly->exponent = exponent;

	NewPoly->prev = Target->prev;
	NewPoly->next = Target;
	Target->prev->next = NewPoly;
	Target->prev = NewPoly;
}

int AddElementAfterByValue(Position Head, int target_coefficient, int target_exponent, int coefficient, int exponent) {
	Position target = FindElementByValue(Head, target_coefficient, target_exponent);
	if (!target) {
		printf("Neuspješno dodavanje");
	}
	else {
		AddElementAfterByPosition(Head, target, coefficient, exponent);
	}
}

int AddElementAfterByPosition(Position Head, Position Target, int coefficient, int exponent) {
	if (!Target) {
		printf("Unjeta je kriva pozicija");
		return 0;
	}
	Position NewPoly = NULL;
	NewPoly = AllocateMemory();

	if (!NewPoly)
		return 0;

	NewPoly->coefficient = coefficient;
	NewPoly->exponent = exponent;

	if (Target->next != NULL)
		Target->next->prev = NewPoly;
	NewPoly->prev = Target;
	NewPoly->next = Target->next;
	Target->next = NewPoly;
}

Position FindElementByValue(Position Head, int coefficient, int exponent) {
	while (Head != NULL && !(Head->coefficient == coefficient && Head->exponent == exponent)) {
		Head = Head->next;
	}
	if (Head == NULL)
		printf("Nije Pronađen");
	return Head;
}

Position AllocateMemory() {
	Position ElementForAllocation = NULL;
	ElementForAllocation = (Position)malloc(sizeof(Element));
	if (!ElementForAllocation) {
		printf("Neuspjesna alokacija memorije");
		return NULL;
	}
	return ElementForAllocation;
}

int PrintList(Position HeadNext) {
	while (HeadNext != NULL) {
		printf("\n%d  : %d \n", HeadNext->coefficient, HeadNext->exponent);
		HeadNext = HeadNext->next;
	}
	return 0;
}

int PrintLists(Position HeadNext1, Position HeadNext2) {
	while (HeadNext1 != NULL) {
		printf("%d:%d ", HeadNext1->coefficient, HeadNext1->exponent);
		HeadNext1 = HeadNext1->next;
	}
	printf("\n");
	while (HeadNext2 != NULL) {
		printf("%d:%d ", HeadNext2->coefficient, HeadNext2->exponent);
		HeadNext2 = HeadNext2->next;
	}
	return 0;
}

int AddElementToEnd(Position Head, int coefficient, int exponent) {
	Position NewPoly = NULL;
	NewPoly = AllocateMemory();

	if (!NewPoly)
		return 0;

	NewPoly->coefficient = coefficient;
	NewPoly->exponent = exponent;

	while (Head->next != NULL) {
		Head = Head->next;
	}

	NewPoly->next = NULL;
	NewPoly->prev = Head;
	Head->next = NewPoly;

	return 0;
}

int AddElementToStart(Position Head, int coefficient, int exponent) {
	Position NewPoly = NULL;
	NewPoly = AllocateMemory();

	if (!NewPoly)
		return 0;

	NewPoly->coefficient = coefficient;
	NewPoly->exponent = exponent;

	if (Head->next == NULL && Head->prev == NULL) {
		NewPoly->prev = Head;
		NewPoly->next = NULL;
		Head->next = NewPoly;
	}
	else {
		NewPoly->prev = Head;
		Head->next->prev = NewPoly;
		NewPoly->next = Head->next;
		Head->next = NewPoly;
	}

	return 0;
}