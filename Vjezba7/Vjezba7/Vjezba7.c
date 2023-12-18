#define _CRT_SECURE_NO_WARNINGS
#define ALLOCATION_ERROR (NULL)
#define MAX_SIZE (128)
#define MAX_LINE (1024)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//7. Napisati program koji pomocu vezanih listi(stabala) predstavlja strukturu direktorija.
//Omoguciti unos novih direktorija i pod - direktorija, ispis sadrzaja direktorija i
//povratak u prethodni direktorij.Tocnije program treba preko menija simulirati
//koristenje DOS naredbi : 1 - "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.

struct Folder;
typedef struct Folder* Dir;
typedef struct Folder {
	char name[MAX_LINE];
	Dir subdirs;
	Dir next;
	Dir parent;
}_Folder;

Dir CreateDir(char name[MAX_LINE]);
Dir Allocate();
Dir FindDir(Dir Current, char name[MAX_LINE]);

int CreateSubDir(Dir Current);
int PrintDir(Dir Current);

char* GetPath(Dir Current);

int main() {
	_Folder Head = { .name = {0}, .next = NULL ,.parent = NULL, .subdirs = NULL };
	Dir CurrentDir = NULL;
	Dir Root = CreateDir("C:");
	Head.next = Root;
	CurrentDir = Root;

	while (1) {
		int choice = 0;
		char name[MAX_LINE] = { 0 };

		printf("\n%s", GetPath(CurrentDir));
		printf("\nUnesite: 1- md, 2-dir, 3-cd dir, 4- dir, 5- close: ");

		if (scanf("%d", &choice) != 1) {
			while (getchar() != '\n');
			printf("\nInvalid input. Please enter a valid option.\n");
			continue;
		}

		switch (choice) {
		case 1:
			CreateSubDir(CurrentDir);
			break;
		case 2:
			PrintDir(CurrentDir);
			break;
		case 3:
			printf("Unesite ime foldera: ");
			scanf("%s", name);
			Dir TargetDir = FindDir(CurrentDir, name);
			if (!TargetDir) {
				printf("\n Ne postoji folder s tim imenom! ");
				break;
			}
			CurrentDir = TargetDir;
			break;
		case 4:
			if (CurrentDir->parent == NULL) {
				printf("\nYou can't exit from C");
				break;
			}
			else {
				CurrentDir = CurrentDir->parent;
				break;
			}
		case 5:
			printf("\nExiting the program.\n");
			return 0;
		default:
			printf("\nInvalid command");
		};
	}

	return 0;
}

char* GetPath(Dir Current) {
	char path[MAX_LINE];
	path[0] = '\0';
	while (Current->parent != NULL) {
		strcat(path, "/");
		strcat(path, Current->name);
		Current = Current->parent;
	}
	strcat(path, "/");
	strcat(path, "C");
	return path;
}

Dir FindDir(Dir Current, char name[MAX_LINE]) {
	Current = Current->subdirs;
	while (Current != NULL && strcmp(name, Current->name) != 0)
		Current = Current->next;
	return Current;
}

int PrintDir(Dir Current) {
	if (Current->subdirs == NULL) {
		printf("\nFolder %s is empty!", Current->name);
		return 0;
	}
	printf("\n%s ", Current->name);
	Dir Temp = Current->subdirs;
	while (Temp != NULL) {
		printf(" [%s]", Temp->name);
		Temp = Temp->next;
	}
	return 1;
}

int CreateSubDir(Dir Current) {
	Dir NewDir = NULL;
	char name[MAX_LINE] = { 0 };
	printf("Unesite ime foldera:");
	scanf("%s", name);
	NewDir = CreateDir(name);
	if (!NewDir)
		return ALLOCATION_ERROR;
	if (Current->subdirs == NULL) {
		Current->subdirs = NewDir;
	}
	else {
		NewDir->next = Current->subdirs;
		Current->subdirs = NewDir;
	}
	NewDir->parent = Current;
	return 1;
}

Dir Allocate() {
	Dir ToAllocate = NULL;
	ToAllocate = (Dir)malloc(sizeof(_Folder));
	if (!ToAllocate)
		return ALLOCATION_ERROR;
	return ToAllocate;
}

Dir CreateDir(char name[MAX_LINE]) {
	Dir NewDir = Allocate();
	if (!NewDir) {
		printf("Error allocating");
		return ALLOCATION_ERROR;
	}
	strcpy(NewDir->name, name);
	NewDir->next = NULL;
	NewDir->subdirs = NULL;
	NewDir->parent = NULL;
	return NewDir;
}