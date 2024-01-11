#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node Node;
typedef Node* Position;

struct Node {
	int element;
	Position left;
	Position right;
};

Position insertElement(Position root, int newElement);
int replaceElements(Position root);
int writeInOrderToFile(Position root, FILE* file);

int main() {
	Position root = NULL;

	root = insertElement(root, 2);
	int elements[] = { 5, 7, 8, 11, 1, 4, 2, 3, 7 };
	int numElements = sizeof(elements) / sizeof(elements[0]);

	for (int i = 0; i < numElements; i++) {
		root = insertElement(root, elements[i]);
	}

	FILE* file = fopen("data.txt", "w");

	writeInOrderToFile(root, file);
	fprintf(file, "\n");

	replaceElements(root);

	writeInOrderToFile(root, file);

	fclose(file);

	Position randRoot = NULL;

	srand(time(NULL));

	int x = 0;

	for (int i = 0; i < 8; i++) {
		x = rand() % 81 + 10;
		randRoot = insertElement(randRoot, x);
	}

	return 0;
}

Position insertElement(Position root, int newElement) {
	if (root == NULL) {
		Position newNode = (Position)malloc(sizeof(Node));
		if (newNode == NULL) {
			printf("ERROR allocating memory\n");
			return NULL;
		}

		newNode->element = newElement;
		newNode->left = newNode->right = NULL;
		return newNode;
	}

	if (newElement >= root->element) {
		root->left = insertElement(root->left, newElement);
	}
	else {
		root->right = insertElement(root->right, newElement);
	}

	return root;
}

int replaceElements(Position root) {
	if (root == NULL) {
		return 0;
	}

	int originalElement = root->element;
	root->element = replaceElements(root->left) + replaceElements(root->right);
	originalElement += root->element;

	return originalElement;
}

int writeInOrderToFile(Position root, FILE* file) {
	if (file == NULL) {
		printf("ERROR opening file\n");
		return 1;
	}

	if (root == NULL) {
		return 0;
	}

	writeInOrderToFile(root->left, file);
	fprintf(file, "%d, ", root->element);
	writeInOrderToFile(root->right, file);

	return 0;
}