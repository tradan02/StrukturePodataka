#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Student Stud;
struct Student {
	char ime[30];
	char prezime[30];
	int bodovi;
};

int CountStudents(FILE* file);
int InputStudents(FILE* file, int n, Stud x[]);
int Print(Stud x[], int n);
float rel_br_bod(int n);

int main() {
	FILE* f;
	f = fopen("stud.txt", "r");
	int n = CountStudents(f);
	fclose(f);
	Stud* x = (Stud*)malloc(sizeof(Stud) * n);

	f = fopen("stud.txt", "r");
	if (InputStudents(f, n, x) == 0) printf("Nema studenata");
	fclose(f);
	Print(x, n);
	free(x);
	return 0;
}

float rel_br_bod(int n) {
	return (float)n / 30 * 100;
}

int Print(Stud x[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("\n%s %s %d %.2f", x[i].ime, x[i].prezime, x[i].bodovi, rel_br_bod(x[i].bodovi));
	}
	return 0;
}

int CountStudents(FILE* file) {
	int br = 1;
	int brchar = 0;
	char s = '\0';
	while (s != EOF) {
		brchar++;
		s = fgetc(file);
		if (s == '\n')
			br++;
	}
	if (brchar == 1) br = 0;
	return br;
}

int InputStudents(FILE* file, int n, Stud x[]) {
	if (n < 1) return 0;
	int i;
	for (i = 0; i < n; i++) {
		fscanf(file, "%s %s %d", x[i].ime, x[i].prezime, &x[i].bodovi);
	}
	return 1;
}