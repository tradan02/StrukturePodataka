#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _User;
typedef struct _User* UserPosition;
typedef struct _User {
	char name[1024];
	int reprints;
	char rentals[30][1024];
	UserPosition next;
	UserPosition prev;
}User;

struct _Book;
typedef struct _Book* BookPosition;
typedef struct _Book {
	char title[1024];
	char author[1024];
	int releaseYear;
	int copies;
	char borrowers[30][1024];
	BookPosition next;
	BookPosition prev;
}Book;

int ReadUsersFromFile(UserPosition Head, BookPosition Head_Book);
UserPosition AddUser(UserPosition Head, char* name);
int AvailableName(UserPosition HeadNext, char* name);
int SaveUsersToFile(UserPosition HeadNext);
int PrintUsers(UserPosition HeadNext);

int Count_Borrowers(BookPosition Book);
int Count_Book(UserPosition User);
int CopiesLeft(UserPosition UserHeadNext, char* title);

int ReadBooksFromFile(BookPosition Head);
int PrintBooks(BookPosition HeadNext);
int FindBookByYear(BookPosition BookHeadNext, UserPosition UserHeadNext, int year);
int FindBookByAuthor(BookPosition BookHeadNext, UserPosition UserHeadNext, char* author);

int BookRent(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name, int number);
int Book_Load_Add(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name, int number);
int BookReturn(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name);

UserPosition AllocateUser();
BookPosition AllocateBook();

int main() {
	User UserHead = { .name = {0}, .reprints = 0, .rentals = {0}, .next = NULL, .prev = NULL, };
	Book BookHead = { .title = {0}, .author = {0}, .releaseYear = 0, .copies = {0}, .borrowers = {0}, .next = NULL, .prev = NULL };

	ReadBooksFromFile(&BookHead);
	ReadUsersFromFile(&UserHead, &BookHead);

	int choice = 0;
	int copy = 0;
	char name[1024];
	int year;
	char author[1024];
	char title[1024];
	int copies;
	int toggle = 1;

	printf("Dobrodosli u BookHaven\n");
	while (toggle) {
		printf("\n1.Ispis knjiga abecedno\n2.Ispis korisnika abecedno\n3.Pretrazite knjigu po godini\n4.Pretrazite knjigu po autoru\n5.Unos novog korisnika\n6.Posudba knjige\n7.Vracanje knjige\n8.Spremi\n9.Kraj\n");
		scanf("%d", &choice);
		if (choice > 0 && choice < 10) {
			switch (choice)
			{
			case 1:
				PrintBooks(BookHead.next);
				break;
			case 2:
				PrintUsers(UserHead.next);
				break;
			case 3:
				printf("Enter year: ");
				scanf("%d", &year);
				FindBookByYear(BookHead.next, UserHead.next, year);
				break;
			case 4:
				printf("Enter author: ");
				scanf(" %[^\n]", author);
				FindBookByAuthor(BookHead.next, UserHead.next, author);
				break;
			case 5:
				while (1) {
					printf("Unesi korisnicko ime(ime_prezime): ");
					scanf(" %[^\n]", name);
					int available = AvailableName(UserHead.next, name);
					if (available == 1) {
						printf("Ime je zauzeto");
					}
					else {
						AddUser(&UserHead, name);
					}
					break;
				}
				break;
			case 6:
				printf("Unesi ime(ime_prezime): ");
				scanf(" %[^\n]", name);
				printf("Unesi naslov: ");
				scanf(" %[^\n]", title);
				printf("Unesi Broj kopija: ");
				scanf("%d", &copies);
				copy = BookRent(UserHead.next, BookHead.next, title, name, copies);
				if (copy == 0)
					break;
				else
					BookRent(UserHead.next, BookHead.next, title, name, copy);
				break;
			case 7:
				printf("Unesi ime(ime_prezime): ");
				scanf(" %[^\n]", name);
				printf("Unesi naslov: ");
				scanf(" %[^\n]", title);
				BookReturn(UserHead.next, BookHead.next, title, name);
				break;
			case 8:
				SaveUsersToFile(UserHead.next);
				break;
			case 9:
				printf("Dovidenja!");
				toggle = 0;
				break;
			default:
				break;
			}
		}
		else {
			printf("Unos mora biti broj od 1 do 9!\n");
		}
	}

	return 0;
}

UserPosition AddUser(UserPosition Head, char* name) {
	UserPosition newUser = NULL;
	newUser = AllocateUser();
	while (Head->next != NULL && strcmp(Head->next->name, name) < 0) {
		Head = Head->next;
	}
	strcpy(newUser->name, name);
	newUser->reprints = 0;

	newUser->next = Head->next;
	newUser->prev = Head;
	if (Head->next) {
		Head->next->prev = newUser;
	}
	Head->next = newUser;

	for (int i = 0; i < 30; i++) {
		newUser->rentals[i][0] = '\0';
	}

	return newUser;
}

UserPosition AllocateUser() {
	UserPosition AllocateUser = NULL;
	AllocateUser = (UserPosition)malloc(sizeof(User));
	if (!AllocateUser) {
		printf("Kriva alokacija memorije!");
		return NULL;
	}
	return AllocateUser;
}

int ReadUsersFromFile(UserPosition Head, BookPosition Head_Book) {
	FILE* filePointer = NULL;
	filePointer = fopen("Users.txt", "r");
	if (!filePointer) {
		printf("Error opening Users.txt!\n");
		return 1;
	}

	while (!feof(filePointer)) {
		char name[1024] = { 0 };
		int reprints;

		if (fscanf(filePointer, "%s %d", name, &reprints) != 2)
			break;

		UserPosition newUserPosition = AddUser(Head, name);
		newUserPosition->reprints = reprints;

		for (int i = 0; i < reprints; i++) {
			if (fscanf(filePointer, " %*1[/]%1023[^/]%*1[/] ", newUserPosition->rentals[i]) != 1) {
				break;
			}
			else {
				Book_Load_Add(Head->next, Head_Book->next, newUserPosition->rentals[i], name, i);
			}
		}
	}

	fclose(filePointer);
	return 0;
}

int SaveUsersToFile(UserPosition HeadNext) {
	FILE* filePointer;
	filePointer = fopen("Users.txt", "w");
	if (!filePointer) {
		printf("Pogreska kod otvaranja!");
		return 1;
	}
	while (HeadNext != NULL) {
		fprintf(filePointer, "%s %d", HeadNext->name, Count_Books(HeadNext));
		for (int i = 0; i < Count_Books(HeadNext); i++) {
			fprintf(filePointer, " /%s/ ", HeadNext->rentals[i]);
		}
		fprintf(filePointer, "\n");
		HeadNext = HeadNext->next;
	}

	fclose(filePointer);

	return 0;
}

int AvailableName(UserPosition HeadNext, char* name) {
	while (HeadNext != NULL) {
		if (strcmp(HeadNext->name, name) == 0)
			return 1;
		HeadNext = HeadNext->next;
	}
	return 0;
}

int PrintUsers(UserPosition HeadNext) {
	while (HeadNext != NULL) {
		printf("%s\n", HeadNext->name);
		for (int i = 0; i < HeadNext->reprints; i++) {
			printf("%s, ", HeadNext->rentals[i]);
		}
		printf("\n\n");
		HeadNext = HeadNext->next;
	}
	return 0;
}

BookPosition AllocateBook() {
	BookPosition AllocateBook = NULL;
	AllocateBook = (BookPosition)malloc(sizeof(Book));
	if (!AllocateBook) {
		printf("Kriva alokacija memorije!");
		return NULL;
	}
	return AllocateBook;
}

int ReadBooksFromFile(BookPosition Head) {
	FILE* filePointer;
	filePointer = fopen("Books.txt", "r");
	if (!filePointer) {
		printf("Error opening Books.txt!\n");
		return 1;
	}

	while (!feof(filePointer)) {
		char title[1024];
		char author[1024];
		int year;
		int copies;

		if (fscanf(filePointer, " %*1[/]%1023[^/]%*1[/]  %*1[/]%1023[^/]%*1[/]  %d %d", title, author, &year, &copies) != 4) {
			break;
		}

		BookPosition readBook = AllocateBook();
		strcpy(readBook->title, title);
		strcpy(readBook->author, author);
		readBook->releaseYear = year;
		readBook->copies = copies;

		for (int i = 0; i < 30; i++) {
			readBook->borrowers[i][0] = '\0';
		}

		readBook->next = Head->next;
		readBook->prev = Head;
		if (Head->next) {
			Head->next->prev = readBook;
		}
		Head->next = readBook;

		Head = Head->next;
	}

	fclose(filePointer);
	return 0;
}

int Count_Borrowers(BookPosition Book) {
	int counter = 0;
	while (counter < 30 && strcmp(Book->borrowers[counter], "") != 0)
		counter++;
	return counter;
}

int Count_Books(UserPosition User) {
	int counter = 0;
	while (counter < 30 && strcmp(User->rentals[counter], "") != 0)
		counter++;
	return counter;
}

int CopiesLeft(UserPosition UserHeadNext, char* title) {
	int counter = 0;
	while (UserHeadNext != NULL) {
		for (int i = 0; i < UserHeadNext->reprints; i++) {
			if (strcmp(UserHeadNext->rentals[i], title) == 0)
				counter++;
		}
		UserHeadNext = UserHeadNext->next;
	}
	return counter;
}

int PrintBooks(BookPosition HeadNext) {
	while (HeadNext != NULL) {
		printf("%s\n%s\n%d\n%d\n", HeadNext->title, HeadNext->author, HeadNext->releaseYear, HeadNext->copies);
		if (Count_Borrowers(HeadNext) > 0)
			printf("Korisnici koji posjeduju knjigu: \n");
		for (int i = 0; i < 10; i++) {
			if (strlen(HeadNext->borrowers[i]) > 0) {
				printf("%s ", HeadNext->borrowers[i]);
				if (i < 9 && strlen(HeadNext->borrowers[i + 1]) > 0) {
					printf(", ");
				}
			}
		}
		HeadNext = HeadNext->next;
		printf("\n");
		printf("\n");
	}
	return 0;
}

int FindBookByYear(BookPosition BookHeadNext, UserPosition UserHeadNext, int year) {
	int found = 0;
	while (BookHeadNext != NULL) {
		if (BookHeadNext->releaseYear == year) {
			printf("Title: %s, Copies Left: %d\n", BookHeadNext->title, BookHeadNext->copies - CopiesLeft(UserHeadNext, BookHeadNext->title));
			found = 1;
		}
		BookHeadNext = BookHeadNext->next;
	}
	if (found == 0)
		printf("Nema knjige iz zeljene godine!\n");
	return 0;
}

int FindBookByAuthor(BookPosition BookHeadNext, UserPosition UserHeadNext, char* author) {
	int found = 0;
	while (BookHeadNext != NULL) {
		if (strcmp(BookHeadNext->author, author) == 0) {
			printf("Title: %s, Copies Left: %d\n", BookHeadNext->title, BookHeadNext->copies - CopiesLeft(UserHeadNext, BookHeadNext->title));
			found = 1;
		}
		BookHeadNext = BookHeadNext->next;
	}
	if (found == 0)
		printf("Nema knjiga zeljenog autora!\n");
	return 0;
}

int BookRent(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name, int number) {
	int choice = 0;
	while (UserHeadNext != NULL && strcmp(UserHeadNext->name, name) != 0) {
		UserHeadNext = UserHeadNext->next;
	}
	while (BookHeadNext != NULL && strcmp(BookHeadNext->title, title) != 0) {
		BookHeadNext = BookHeadNext->next;
	}
	if (UserHeadNext == NULL) {
		printf("Nepostoji korisnik imena: %s", name);
	}
	else if (Count_Books(UserHeadNext) + number >= 5) {
		printf("Imate %d posudenih knjiga. Maksimalan broj posudenih knjiga je 5.\n", Count_Books(UserHeadNext));
	}
	else if (BookHeadNext == NULL) {
		printf("Nepostoji knjiga: %s", title);
	}
	else if (BookHeadNext->copies == 0) {
		printf("Sve posudene");
	}
	else if (BookHeadNext->copies - Count_Borrowers(BookHeadNext) < number) {
		printf("Nema dovoljno knjiga na raspolaganju.\nIma %d knjiga.\nUnesite 1 ako zelite preostele knjige, 0 ako nezelite: ", BookHeadNext->copies);
		scanf("%d", &choice);
		if (choice == 1) {
			for (int i = 0; i < BookHeadNext->copies - Count_Borrowers(BookHeadNext); i++)
				Book_Load_Add(UserHeadNext, BookHeadNext, title, name, Count_Books(UserHeadNext));
		}

		else if (choice == 0)
			return 0;
		else {
			printf("Pogresan odabit\n");
		}
	}
	else
		for (int i = 0; i < number; i++) {
			Book_Load_Add(UserHeadNext, BookHeadNext, title, name, Count_Books(UserHeadNext));
			UserHeadNext->reprints = Count_Books(UserHeadNext);
		}

	return 0;
}

int Book_Load_Add(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name, int number) {
	while (UserHeadNext != NULL && strcmp(UserHeadNext->name, name) != 0) {
		UserHeadNext = UserHeadNext->next;
	}
	while (BookHeadNext != NULL && strcmp(BookHeadNext->title, title) != 0) {
		BookHeadNext = BookHeadNext->next;
	}
	if (UserHeadNext == NULL) {
		printf("Nepostoji korisnik imena: %s", name);
	}

	else if (BookHeadNext == NULL) {
		printf("Nepostoji knjiga: %s", title);
	}

	strcpy(UserHeadNext->rentals[number], BookHeadNext->title);
	strcpy(BookHeadNext->borrowers[Count_Borrowers(BookHeadNext)], UserHeadNext->name);

	return 0;
}

int BookReturn(UserPosition UserHeadNext, BookPosition BookHeadNext, char* title, char* name) {
	int real = 0;
	while (UserHeadNext != NULL && strcmp(UserHeadNext->name, name) != 0) {
		UserHeadNext = UserHeadNext->next;
	}
	while (BookHeadNext != NULL && strcmp(BookHeadNext->title, title) != 0) {
		BookHeadNext = BookHeadNext->next;
	}
	for (int i = 0; i < UserHeadNext->reprints; i++) {
		if (strcmp(UserHeadNext->rentals[i], title) == 0) {
			real = 1;
			break;
		}
	}

	if (UserHeadNext == NULL) {
		printf("Nepostoji korisnik imena: %s\n", name);
	}
	else if (BookHeadNext == NULL) {
		printf("Nepostoji knjiga: %s\n", title);
	}
	else if (real != 1) {
		printf("Korisnik nije posudio knjigu %s!\n", title);
	}
	else {
		int i = 0;
		for (i = 0; i < Count_Borrowers(BookHeadNext); i++) {
			if (strcmp(name, BookHeadNext->borrowers[i]) == 0) {
				break;
			}
		}
		for (i = 0; i < Count_Borrowers(BookHeadNext) - 1; i++) {
			strcpy(BookHeadNext->borrowers[i], BookHeadNext->borrowers[i + 1]);
		}
		BookHeadNext->borrowers[i][0] = '\0';

		for (i = 0; i < Count_Books(UserHeadNext); i++) {
			if (strcmp(title, UserHeadNext->rentals[i]) == 0) {
				break;
			}
		}
		for (i = 0; i < Count_Books(UserHeadNext) - 1; i++) {
			strcpy(UserHeadNext->rentals[i], UserHeadNext->rentals[i + 1]);
		}
		UserHeadNext->rentals[i][0] = '\0';
		UserHeadNext->reprints = Count_Books(UserHeadNext);
	}
	return 0;
}