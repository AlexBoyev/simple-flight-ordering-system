#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

enum boolean {
	FALSE, TRUE
} bool;

typedef struct passenger {
	char id[10];
	char name[31];
	char last[31];
} passenger;

typedef struct seat {
	int raw;
	int snum;
	char AT;
	int YesNo;
	char id[10];
} seat;

typedef struct plane {
	int allsit;
	int Araw;
	seat **seat;
} plane;

enum boolean ifPassangerExist(char *, char*);
enum boolean contains(char *, char **, int);
void planeReport(plane *, char **, int, char *);
char **split(char[], int);
plane *createPlane(int, int);
int isspace(int);
int readFile(char *);
char *readInput(int);
char *readInputNumbers(int);
int ReadLine(char *);
int writePassenger(char *, passenger *);
passenger *createPassanger(char *, char *, char *);
passenger *getPassangerByID(char *, char *);
void getPassngers(char *, int);
int insertPassanger(char *, plane *, char *, int);
void printPlane(plane *);
void printEmptyPlaces(plane *);
void freeSeat(seat* s){
	free(s);
}

void freePlane(plane* p){
	int i;
	for (i = 0; i < p->allsit; i++) {
		seat *curr = p->seat[i];
		freeSeat(curr);
	}
	free(p->seat);
	free(p);


}
int main() {

	char* path = "passengers.txt";

	int sighnedPassangersA = 0;
	int sighnedPassangersT = 0;
	int numOfPassangers_integer = 0;
	int numberOfseats_integer = 0;
	int numberofRowsinAclass_integer = 0;


	printf("Please enter the TOTAL amount of passengers that going to be on the plane: ");
	char *numOfPassangers = readInputNumbers(10);
	numOfPassangers_integer = atoi(numOfPassangers);
	getPassngers(path, numOfPassangers_integer);
	free(numOfPassangers);
	while (numberOfseats_integer < 20) {
		printf(
				"Please enter the TOTAL amount of seats that going to be on the plane: (must be at least 20) ");
		char *numberofSeats = readInputNumbers(10);
		numberOfseats_integer = atoi(numberofSeats);
		if (numberOfseats_integer < 20)
			printf("Wrong value! please try again.\n");
	}

	while (!(numberofRowsinAclass_integer >= 1
			&& numberofRowsinAclass_integer <= 3)) {
		printf("Please enter TOTAL A class rows: (must be 1-3) ");
		char *numberofRowsinAclass = readInputNumbers(10);
		numberofRowsinAclass_integer = atoi(numberofRowsinAclass);
		if (!(numberofRowsinAclass_integer >= 1
				&& numberofRowsinAclass_integer <= 3))
			printf("Wrong value! please try again.\n");
	}
	printf("\n");
	plane *p = createPlane(numberOfseats_integer, numberofRowsinAclass_integer);
	int totalSeatsInA = (numberofRowsinAclass_integer * 4);
	int totalSeatsInT = (numberOfseats_integer - totalSeatsInA);

	while (1) {
		printf("\n*****Welcome to SCE flight booking company*****\n\n");
		printf("To book a flight press 1.\n");
		printf("To check your order status and history press 2.\n");
		printf("To check available seats press 3.\n");
		printf("To exit press 4.\n\n");
		printf("***********************************************\n");
		char* ids[10];
		char *class = NULL;
		int weCanAdd = 0;
		int j;
		scanf("%d", &j);
		switch (j) {
		case 1:
			printf("Please enter desired class: (A or T) ");
			char *class = readInput(3);
			printf("Please enter amount of passengers: ");
			char *numOfPassangers = readInputNumbers(10);
			int numOfPassangers_integer = atoi(numOfPassangers);
			if (strcmp(class, "A") == 0) {
				if ((totalSeatsInA - sighnedPassangersA
						- numOfPassangers_integer) >= 0) {
					weCanAdd = 1;
				} else {
					printf("\nSorry, there is no vacancy in class A.\n");
				}
			}
			if (strcmp(class, "T") == 0) {
				if ((totalSeatsInT - sighnedPassangersT
						- numOfPassangers_integer) >= 0) {
					weCanAdd = 1;
				} else {
					printf("\nSorry, there is no vacancy in class T.\n");
				}
			}

			if (weCanAdd == 1) {
				while (numOfPassangers_integer > 0) {
					int nextFree = 0;
					if (strcmp(class, "A") == 0)
						nextFree = sighnedPassangersA;
					else
						nextFree = (totalSeatsInA + sighnedPassangersT);
					printf("Please insert passenger ID: ");
					char *id = readInputNumbers(10);
					if (ifPassangerExist(path, id) == TRUE) {
						insertPassanger(path, p, id, nextFree);
						if (strcmp(class, "A") == 0)
							sighnedPassangersA += 1;
						else
							sighnedPassangersT += 1;

						numOfPassangers_integer--;
					} else {
						printf("Wrong ID number! try again.\n");
					}
				}
				printf("\nYou have successfully booked a reservation!\n");
				printf("\n");
				printPlane(p);
			}
			break;
		case 2:
			printf("Please enter amount of passengers: ");
			char *numOfPassangersToPrint = readInputNumbers(10);
			int numOfPassangersToPrint_integer = atoi(numOfPassangersToPrint);
			free(numOfPassangersToPrint);

			int i;
			for (i = 0; i < numOfPassangersToPrint_integer; i++) {
				printf("Please insert passenger ID: ");
				char *c = readInputNumbers(10);
				ids[i] = malloc(sizeof(c));
				strcpy(ids[i], c);
			}
			planeReport(p, ids, numOfPassangersToPrint_integer, path);
			break;
		case 3:
			printEmptyPlaces(p);
			break;
		case 4:
			printf("Thank you for flying with SCE.\n");
			freePlane(p);
			exit(0);
			break;

		}
	}

	return 0;
}



void printEmptyPlaces(plane * p){
	printf("\n");
	printf("Category Row Place \n");
	printf("================== \n");
	int i;
	int j =0;
		for (i = 0; i < p->allsit; i++) {
			seat *curr = p->seat[i];
			if(curr->YesNo==1){
				j++;
				printf("%c        %d    %d \n",  curr->AT,curr->raw,curr->snum);
			}
		}

	printf("==================\n");
	printf("Total Free Seats: %d \n",j);

}



enum boolean ifPassangerExist(char *filename, char* id) {
	FILE *file = fopen(filename, "r");
	char** lineSplited;
	if (file != NULL) {
		char line[31];
		while (fgets(line, sizeof line, file) != NULL) {
			lineSplited = split(line, 3);
			if (strcmp(lineSplited, id) == 0) {
				fclose(file);
				return TRUE;
			}
		}
		fclose(file);
		return FALSE;

	} else {
		fclose(file);
		return FALSE;
	}
}

void printPlane(plane *p) {
	int i;
	for (i = 0; i < p->allsit; i++) {
		seat *curr = p->seat[i];
		printf("%d %d %c %d %s \n", curr->YesNo, curr->raw, curr->AT,
				curr->snum, curr->id);
	}

}

enum boolean contains(char *id, char *ids[10], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (strcmp(ids[i], id) == 0)
			return TRUE;
	}
	return FALSE;
}

void planeReport(plane *p, char **ids, int size, char *filename) {


	printf("Category	Row	Place	Passport_ID		FirstName	LastName\n");
	int i;

	int size_Pass = 0;


	for (i = 0; i < p->allsit; i++) {
		seat *curr = p->seat[i];
		if (strcmp(curr->id, "XXXXXXXXX") != 0
				&& contains(curr->id, ids, size) == TRUE&&size_Pass<size) {
			passenger *curr_pass = getPassangerByID(filename, curr->id);
			size_Pass++;
			printf("   %c	         %d 	  %d 	 %s                 %s          %s 		\n", curr->AT, curr->raw,
					curr->snum, curr->id, curr_pass->name, curr_pass->last);
			free(curr_pass);
		}
	}

}

passenger *getPassangerByID(char *filename, char *id) {
	FILE *reads;
	reads = fopen(filename, "r");
	while (!feof(reads)) {
		struct passenger *n = (struct passenger*) malloc(
				sizeof(struct passenger));
		fscanf(reads, "%s %s %s \n", (n->id), (n->name), (n->last));
		if (strcmp(n->id, id) == 0) {
			fclose(reads);
			return n;
		}
		free(n);
	}
	fclose(reads);
	return NULL;
}

int insertPassanger(char *filename, plane *p, char *id, int nextFreeSeat) {
	struct passenger *n;
	if (ifPassangerExist(filename, id) == TRUE) {
		n = getPassangerByID(filename, id);
		free(n);
	} else {
		printf("Passenger doent exist,try again.");
		return 0;
	}
	strcpy(p->seat[nextFreeSeat]->id, id);
	p->seat[nextFreeSeat]->YesNo = 0;
	return 0;
}

void getPassngers(char *path, int num) {
	while (num > 0) {
		printf("Insert passenger ID: ");
		char *id = readInputNumbers(10);
		printf("Insert passenger name: ");
		char *name = readInput(31);
		printf("Insert passenger last name: ");
		char *lastName = readInput(31);
		passenger *v = createPassanger(id, name, lastName);
		writePassenger(path, v);
		num--;
		free(id);
		free(name);
		free(lastName);


	}
}
enum boolean isAnumber(char* num, int length) {
	return TRUE;
}
passenger* createPassanger(char *id, char *name, char *last) {
	struct passenger *object = malloc(sizeof(struct passenger));

	strcpy(object->id, id);
	strcpy(object->name, name);
	strcpy(object->last, last);
	return object;

}

int writePassenger(char *path, passenger *e) {
	FILE *file = fopen(path, "a");
	if (file != NULL) {
		fprintf(file, "%s %s %s \n", e->id, e->name, e->last);
		fclose(file);
	}
	return 0;
}

int readFile(char *path) {
	FILE *reads;
	reads = fopen(path, "r");
	if (reads == NULL) {
		printf("Failed to open file!\n");
		return 1;
	}
	while (!feof(reads)) {
		struct passenger *n = (struct passenger*) malloc(
				sizeof(struct passenger));
		fscanf(reads, "%s %s %s \n", &(n->id), &(n->name), &(n->last));
		printf(reads, "%s %s %s \n", (n->id), (n->name), (n->last));
	}
	fclose(reads);
	return 0;
}

char *readInput(int max) {
	char *name = (char*) malloc(max);
	int j;
	while (1) {
		int c = getchar();
		if (c == EOF)
			break;
		if (!isspace(c)) {
			ungetc(c, stdin);
			break;
		}
	}
	int i = 0;
	while (1) {
		int c = getchar();
		if (isspace(c) || c == EOF) {
			name[i] = 0;
			break;
		}
		if (c >= 'a' && c <= 'z')
			c = c - ('a' - 'A');
		name[i] = c;
		if (i == max - 1) {
			max += max;
			name = (char*) realloc(name, max);
		}
		i++;

	}

		return name;
	}

	char **split(char str[], int delemiter) {
		int i;
		char *test[sizeof(strtok(str, " "))];
		if (sizeof(test) > 0) {
			test[0] = strtok(str, " ");
			for (i = 1; i < delemiter; i++) {
				test[i] = strtok(NULL, " ");
			}
		}
		return *test;
	}

	int ReadLine(char *filename) {
		FILE *file = fopen(filename, "r");
		char *lineSplited;
		if (file != NULL) {
			char line[31];
			while (fgets(line, sizeof line, file) != NULL) {
				lineSplited = split(line, 3);
			}
			fclose(file);
		}
		return 0;
	}

	plane *createPlane(int totalSeats, int firstClass) {
		struct plane *object = malloc(sizeof(struct plane));
		object->allsit = totalSeats;
		object->Araw = firstClass;
		object->seat = (seat*) malloc(sizeof(struct seat*) * totalSeats);
		int j = 6, i = 0;
		int totalFirstClassSeats = firstClass * 4;

		for (i = 0; i < totalFirstClassSeats; i++) {
			object->seat[i] = (seat*) malloc(sizeof(struct seat));
			seat *currSeat = object->seat[i];
			currSeat->raw = (i / 4 + 1);
			currSeat->snum = (i % 4 + 1);
			currSeat->AT = 'A';
			currSeat->YesNo = 1;
			strcpy(currSeat->id, "XXXXXXXXX");
		}
		for (j, i = totalFirstClassSeats; i < totalSeats; i++, j++) {
			object->seat[i] = (seat*) malloc(sizeof(struct seat));
			seat *currSeat = object->seat[i];
			currSeat->raw = (j / 6 + firstClass);
			currSeat->snum = (j % 6 + 1);
			currSeat->AT = 'T';
			currSeat->YesNo = 1;
			strcpy(currSeat->id, "XXXXXXXXX");
		}
		for (i = 0; i < totalSeats; i++) {
			seat* curr = object->seat[i];
			printf("%d %d %c %d %s \n", curr->YesNo, curr->raw, curr->AT,
					curr->snum, curr->id);
		}
		return object;
	}

	char *readInputNumbers(int max) {
		char *name = (char*) malloc(max);
		while (1) {
			int c = getchar();
			if (c == EOF)
				break;
			if (!isspace(c)) {
				ungetc(c, stdin);
				break;
			}
		}
		int i = 0;
		while (1) {
			int c = getchar();
			if (isspace(c) || c == EOF) {
				name[i] = 0;
				break;
			}
			if (c >= 'a' && c <= 'z')
				c = c - ('a' - 'A');
			name[i] = c;
			if (i == max - 1) {
				max += max;
				name = (char*) realloc(name, max);
			}
			i++;
		}
		if (isAnumber(name, i) == TRUE) {
			return name;
		} else {
			printf("Please Enter a Valid NUmber..\n");
			return readInputNumbers(max);
		}
	}
