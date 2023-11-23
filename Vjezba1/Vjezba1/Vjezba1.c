#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE (108)
#define MAX_SIZE (1028)
#define ERROR (NULL)
#define ErrorAllocating (NULL)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Napisati program koji prvo procita koliko redaka ima datoteka, tj.koliko ima studenata
//zapisanih u datoteci.Nakon toga potrebno je dinamicki alocirati prostor za niz struktura
//studenata(ime, prezime, bodovi) i ucitati iz datoteke sve zapise.Na ekran ispisati ime,
//prezime, apsolutni i relativni broj bodova.
//Napomena: Svaki redak datoteke sadrzi ime i prezime studenta, te broj bodova na kolokviju.
//relatvan_br_bodova = br_bodova / max_br_bodova * 100

struct _Student;
typedef struct _Student* List;
typedef struct _Student {
	char name[MAX_LINE], surname[MAX_LINE];
	int score;
}Student;

int ReadAndLoadStudents(int NumberOfStudents, List ListOfStudents);
int CountFromFile();
int PrintStudentList(int NumberOfStudents, List ListOfStudents);
float RelativeScore(int score);

List AllocateList(int NumberOfStudents);

int main() {
	int NumberOfStudents = CountFromFile();
	if (NumberOfStudents == 0) {
		printf("\nNema studenata u dadoteci, izlaz iz programa");
	}
	List ListOfStudents = NULL;
	ListOfStudents = AllocateList(NumberOfStudents);
	if (!ListOfStudents)
		return ERROR;

	ReadAndLoadStudents(NumberOfStudents, ListOfStudents);
	PrintStudentList(NumberOfStudents, ListOfStudents);
	free(ListOfStudents);
	return 0;
}

float RelativeScore(int score) {
	return (float)score / 50 * 100;
}

int PrintStudentList(int NumberOfStudents, List ListOfStudents) {
	int i = 0;
	for (i = 0; i < NumberOfStudents; i++) {
		printf("\nName: %s\nSurname: %s\nScore: %d\nRelscore: %.1f\n\n\n\n", ListOfStudents[i].name, ListOfStudents[i].surname, ListOfStudents[i].score, RelativeScore(ListOfStudents[i].score));
	}
	return 0;
}

int ReadAndLoadStudents(int NumberOfStudents, List ListOfStudents) {
	FILE* filepointer = NULL;
	filepointer = fopen("Studenti.txt", "r");
	if (!filepointer)
		return ERROR;

	int i = 0;

	for (i = 0; i < NumberOfStudents; i++) {
		if (fscanf(filepointer, "%s %s %d", ListOfStudents[i].name, ListOfStudents[i].surname, &ListOfStudents[i].score) != 3) {
			printf("Error reading data from file.\n");
			fclose(filepointer);
			return ERROR;
		}
	}
	fclose(filepointer);
	return 0;
}

List AllocateList(int NumberOfStudents) {
	List ListOfStudents = NULL;
	ListOfStudents = (List)malloc(sizeof(Student) * NumberOfStudents);
	if (!ListOfStudents)
		return ErrorAllocating;
	return ListOfStudents;
}

int CountFromFile() {
	FILE* filepointer = NULL;
	filepointer = fopen("Studenti.txt", "r");
	if (!filepointer)
		return ERROR;
	int counter = 0;
	char buffer[MAX_LINE] = { 0 };

	if (fgetc(filepointer) == EOF)
		return 0;

	while (!feof(filepointer)) {
		fgets(buffer, MAX_LINE, filepointer);
		counter++;
	}
	fclose(filepointer);
	return counter;
}