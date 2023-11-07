#define _CRT_SECURE_NO_WARNINGS

#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#define MAX_SIZE (128)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _Person;
typedef struct _Person* Poz;
typedef struct  _Person {
	char name[MAX_SIZE], surname[MAX_SIZE];
	int god;
	Poz next;
}_person;

int Add(Poz P, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na pocetak
int AddAfter(Poz P, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje nakon odredenog
int AddBefore(Poz P, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje prije odredenog
int Print(Poz P);
//Printanje cijele liste
int AddEnd(Poz P, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na kraj
Poz Find(Poz P, char surname[MAX_SIZE]);
//Trazenje po prezimenu
Poz FindPrev(Poz P, char surname[MAX_SIZE]);
//Trazenje prethodnog po prezimenu
int DeleteByPosition(Poz PrevLoc);
//Brisanje unosom pozicije prethodnika
int DeletePerson(Poz P, char surname[MAX_SIZE]);
//Brisanje unosom prezimena
Poz AllocateMemory();
//Alociranje memorije
int WriteInFile(Poz P);

int main() {
	_person Head = { .next = NULL, .name = {0},.surname = {0}, .god = 0 };

	// Add(&Head, "toni", "radanovic", 2003);
	// Add(&Head, "karlo", "trogrlic", 2003);
	// AddEnd(&Head, "petar", "metar", 2003);
	// AddBefore(&Head, "trogrlic", "sime", "jarun", 2002);
	// Print(Head.next);
	// WriteInFile(Head.next);

	int toggle = 1;
	int odabir = 0;
	char name[MAX_SIZE] = { 0 }, surname[MAX_SIZE] = { 0 }, target[MAX_SIZE] = { 0 };
	int god = 0;
	while (toggle) {
		printf("Odaberite koju radnju zelite: \n 1. Dodavanje na pocetak\n 2. Dodavanje na kraj\n 3. Dodavanje nakon\n 4. Dodavanje ispred,\n 5. Brisanje\n 6. Ispis liste\n7. Zavrsi \n");
		scanf("%d", &odabir);
		if (odabir >= 1 && odabir <= 7) {
			switch (odabir)
			{
			case 1:
				printf("format: IME PREZIME GODINA\n");
				scanf("%s %s %d", name, surname, &god);
				Add(&Head, name, surname, god);
				break;
			case 2:
				printf("format: IME PREZIME GODINA\n");
				scanf("%s %s %d", name, surname, &god);
				AddEnd(&Head, name, surname, god);
				break;
			case 3:
				printf("format: PREZIME_METE IME PREZIME GODINA\n");
				scanf("%s %s %s %d", target, name, surname, &god);
				AddAfter(&Head, target, name, surname, god);
				break;
			case 4:
				printf("format: PREZIME_METE IME PREZIME GODINA\n");
				scanf("%s %s %s %d", target, name, surname, &god);
				AddBefore(&Head, target, name, surname, god);
				break;
			case 5:
				printf("format: PREZIME\n");
				scanf("%s", target);
				DeletePerson(&Head, target);
				break;
			case 6:
				Print(Head.next);
				break;
			case 7:
				printf("Program zavrsen");
				toggle = 0;
				break;

			default:
				break;
			}
		}
		else {
			printf("Pogresan unos pokusajte opet \n");
		}
		printf("\n------------------------------------------------------------------------\n------------------------------------------------------------------------\n");
	}

	return 0;
}

int WriteInFile(Poz P) {
	FILE* file;
	file = fopen("ispis.txt", "w");
	while (P != NULL) {
		fprintf(file, "\n---------------------------------------");
		fprintf(file, "\n ime: %s\n prezime: %s\n god: %d", P->name, P->surname, P->god);
		P = P->next;
	}
}

Poz AllocateMemory() {
	Poz q = NULL;
	q = (Poz)malloc(sizeof(_person));
	if (!q) {
		printf("Neuspjesna alokacija memorije");
		return NULL;
	}
	return q;
}

int DeletePerson(Poz P, char surname[MAX_SIZE]) {
	Poz temp = NULL;
	temp = FindPrev(P, surname);
	if (temp != NULL)
		DeleteByPosition(temp);
}

int DeleteByPosition(Poz P) {
	Poz temp;
	if (P != NULL)
	{
		temp = P->next;
		P->next = temp->next;
		free(temp);
	}

	return 0;
}

Poz Find(Poz P, char surname[MAX_SIZE]) {
	while (P != NULL && strcmp(P->surname, surname) != 0) {
		P = P->next;
	}
	return P;
}

Poz FindPrev(Poz P, char surname[MAX_SIZE]) {
	while (P->next != NULL && strcmp(P->next->surname, surname) != 0) {
		P = P->next;
	}
	if (P->next == NULL)
		return NULL;
	else
		return P;
}

int AddEnd(Poz P, char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
	Poz q = NULL;
	q = AllocateMemory();
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->god = god;

	while (P->next != NULL) {
		P = P->next;
	}
	q->next = P->next;
	P->next = q;
}

int Add(Poz P, char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
	Poz q = NULL;
	q = AllocateMemory();
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->god = god;

	q->next = P->next;
	P->next = q;
}

int AddAfter(Poz P, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
	P = Find(P, targetsurname);
	Poz q = NULL;
	q = AllocateMemory();
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->god = god;

	q->next = P->next;
	P->next = q;
}

int AddBefore(Poz P, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
	P = FindPrev(P, targetsurname);
	Poz q = NULL;
	q = AllocateMemory();
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->god = god;

	q->next = P->next;
	P->next = q;
}

int Print(Poz P) {
	while (P != NULL) {
		printf("\n---------------------------------------");
		printf("\n ime: %s\n prezime: %s\n god: %d", P->name, P->surname, P->god);
		P = P->next;
	}
}