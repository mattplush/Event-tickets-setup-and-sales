/*
Matthew Plush

Description:
This program prompts a user for the correct admin pin. If the
correct pin is not entered in 4 or less tries, the program ends.
If the correct pin is entered, the user is prompted to enter the 
names and prices for the seating sections, as well as the perentage 
going to charity.

If the admin function returns true, the pricing details are
displayed. And a user is prompted to purchase tickets. If the user
enters quit during the seat selection, they are prompted to enter
the pin in 4 or less attempts, and a sales summary is displayed.
*/

// ------ PRESS 'Q' TO QUIT!! ---------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#define NEW_MIN_PRICE 10 // min price for seating
#define NEW_MAX_PRICE 500 // max price for seating
#define ATTEMPTS 4 // allowed attempts for entering pin
#define COLUMNS 11 // max string length for 2D string arrays, and most strings
#define STRING_LENGTH 100 // max string length for file paths
#define EVENT "the Denver Nuggets game"
#define PIN "4932"
#define MAX_PERCENTAGE 30 // maximum charity percentage (inclusive)
#define MIN_PERCENTAGE 9.99 // minimum charity percentage (exclusive)
#define NUM_PACKAGES 4 // number of different packages
const char* CONFIRMATIONS = "YN"; // yes or no values
const char QUIT = 'Q'; // quit value
const char* PACKAGE_NUMS = "1234"; //seating packages string
const char* NUMBERS = "0123456789"; //numbers string for validating zip code

const char PACKAGES[NUM_PACKAGES][COLUMNS] = { "Single Pack","Double Pack","Triple Pack","Family Pack" };

typedef struct section
{
	char name[COLUMNS];
	double price;
	int numSold;
	struct section* nextPtr;

}SectionNode;

bool adminSetup(SectionNode** startPtr, double* charityPercentage);
void displayPricing(SectionNode** startPtr, const double charityPercentage);
void displayPurchases(SectionNode** startPtr, const double charityPercentage);
bool findElement(const char* input, const char* selections, char* output);
void purchaseTickets(SectionNode** startPtr, const double charityPercentage, double* charityTotal);
char yesOrNo();
bool validateDouble(const char* buff, double* validDbl, const double minimum, const double maximum);
bool validateZipCode(const char* buff, int* validInt);
SectionNode* search(SectionNode** head, char* target);
bool isPinValid();
void formatString(char* string);


int main(void) {
	double charityPercentage = 0;
	double charityTotal = 0;
	SectionNode* startPtr = NULL;

	if (adminSetup(&startPtr, &charityPercentage)) {
		purchaseTickets(&startPtr, charityPercentage, &charityTotal);
		displayPurchases(&startPtr, charityPercentage);
	}
	free(startPtr);
	startPtr = NULL;
	puts("\nExiting program...");

}




/* This method returns true if the user succesfully entered
the pin and set up the pricing. If the user failed to enter
the correct pin, the method returns false.*/
bool adminSetup(SectionNode** startPtr, double* charityPercentage) {

	void insertNode(SectionNode * *headPtr, const char* sectName, const double sectPrice);
	void enterCharityPercentage(double* charityPercentage);
	void enterSectionName(char* name);
	void enterSectionPrice(const char* name, double* price);
	bool adminSet = false;
	char name[COLUMNS] = { " " };
	double price = 0;


	if (isPinValid()) {

		do {
			enterSectionName(name);

			if (search(startPtr, name) == NULL) {
				enterSectionPrice(name, &price);
				insertNode(startPtr, name, price);
			}
			else {
				puts("Section already exists!");
			}

			fputs("Do you want to enter another section, ", stdout);

		} while (yesOrNo() == CONFIRMATIONS[0]);

		enterCharityPercentage(charityPercentage);
		adminSet = true;
	}

	return adminSet;

}
/* incrementing numSold by the package num and returning the sale (price * packageNum) */
double calculateSale(SectionNode* sectionPtr, int package) {

	sectionPtr->numSold += package;
	return sectionPtr->price * package;
} // calculateSale

/* Displaying the available seating packages. */
void displayPackages() {

	for (size_t i = 0; i < NUM_PACKAGES; i++) {
		size_t num = i + 1;
		printf("\n%zu: ", num);

		for (size_t j = 0; j < COLUMNS; j++) {
			printf("%c", PACKAGES[i][j]);
		}
	}
} // displayPackages

/* Displays the selected charity percentage as well as the prices
for each seating section.*/
void displayPricing(SectionNode** startPtr, const double charityPercentage) {

	SectionNode* currentPtr = NULL;

	printf("\nYou can purchase tickets to %s and %.2lf%% of your ticket purchase will go to charity.\n", EVENT, charityPercentage);
	printf("\n%-25sPrice per person\n", "Section");

	if (startPtr != NULL)
	{
		//set currentPtr to first node in list
		currentPtr = *startPtr;

		//loop through the list until the last node points to null
		while (currentPtr != NULL)
		{
			printf("%-25s$%.2lf\n", currentPtr->name, currentPtr->price);
			currentPtr = currentPtr->nextPtr;
		}
	}

} //displayPricing

/* Displays the price, number sold, sales total, and charity total
for each seating section.*/
void displayPurchases(SectionNode** startPtr, const double charityPercentage) {

	double totalSales = 0;
	double totalCharity = 0;
	double price = 0;
	unsigned int numSold = 0;
	double currentSales = 0;
	double currentCharity = 0;
	SectionNode* currentPtr = NULL;

	

		if (startPtr != NULL)
		{
			printf("\nSummary of ticket sales where %.2lf%% goes to charity\n", charityPercentage);
			printf("\n%-20s%-20s%-20s%-20s%s\n", "Level", "Price", "Sold", "Sales Total", "Charity Total");

			currentPtr = *startPtr;

			while (currentPtr != NULL)
			{
				price = currentPtr->price;
				numSold = currentPtr->numSold;
				currentSales = price * numSold;
				currentCharity = currentSales * charityPercentage / 100;
				totalSales += currentSales;
				totalCharity += currentCharity;
				printf("%-20s", currentPtr->name);
				printf("$%-19.2f", price);
				printf("%-20.2u", numSold);
				printf("$%-19.2f", currentSales);
				printf("$%-19.2f\n", currentCharity);
				currentPtr = currentPtr->nextPtr;
			}

		}

		printf("\nTotal ticket sales: $%.2lf\n", totalSales);
		printf("Total raised for charity: $%.2lf\n", totalCharity);

} //displayPurchases

/* Displays a receipt of the current sale.*/
void displayReceipt(char* section, int package, double sale, double charityDonation) {


		printf("\n%s\n", EVENT);
		puts("Charity Ticket Receipt");
		printf("%s : ", section);

		for (size_t i = 0; i < COLUMNS; i++) {
			printf("%c", PACKAGES[package - 1][i]);
		}

		printf("\nTotal Cost: $%.2lf", sale);
		printf("\nDonation to Charity: $%.2lf\n", charityDonation);
	

} //displayReceipt

/* Prompts the user to enter the percentage going to charity,
within the given min and max.*/
void enterCharityPercentage(double* charityPercentage) {

	bool valid = false;
	char buffer[COLUMNS] = { " " };

	do {
		printf("\nEnter the percentage of sales that will go to charity\n");
		fgets(buffer, COLUMNS, stdin);
		formatString(buffer);
		if (validateDouble(buffer, charityPercentage, MIN_PERCENTAGE, MAX_PERCENTAGE)) {
			valid = true;
		}

	} while (valid == false);

} //enterCharityPercentage

/* Prompts the user to enter a name, any string */
void enterSectionName(char* name) {

	fputs("Enter the seat level name: ", stdout);
	fgets(name, COLUMNS, stdin);
	formatString(name);
} //enterSectionName

/* Prompts the user to enter a valid price, within the min and max */
void enterSectionPrice(const char* name, double* price) {

	bool valid = false;
	char buffer[COLUMNS] = { " " };

	do {
		printf("Enter price for %s: ", name);
		fgets(buffer, COLUMNS, stdin);
		formatString(buffer);

		if (validateDouble(buffer, price, NEW_MIN_PRICE, NEW_MAX_PRICE)) {
			valid = true;
		}

	} while (valid == false);

} //enterSectionPrice

/* Prompts the user to enter 5 digit zipcode, any 5 char string
with only numbers */
void enterZipCode(double sale) {

	bool valid = false;
	char buffer[COLUMNS] = { " " };
	int zipcode = 0;
	do {
		printf("\nYour cost is $%.2f Enter your 5 digit zip code\n", sale);
		fgets(buffer, COLUMNS, stdin);
		formatString(buffer);

		if (validateZipCode(buffer, &zipcode)) {
			valid = true;
		}

	} while (valid == false);
} //enterZipCode


/* This function capitalizes the first letter of a
string, and lowers everything else. It also removes
the \n char */
void formatString(char* string) {

	size_t stringLength = strlen(string);
	string[0] = toupper(string[0]); // capitalizing first letter

	// lowering every other letter
	for (size_t i = 1; i < stringLength - 1; i++) {
		string[i] = tolower(string[i]);
	}
	// removing \n from the end of string if it is present
	if (string[stringLength - 1] == '\n') {
		string[stringLength - 1] = '\0';
	}

} //formatString


/* Returns true if a selections string contains a given char, and sets
the output to that char. */
bool findElement(const char* input, const char* selections, char* output) {

	bool found = false;
	char element = ' '; //variable to hold char version of input variable

	// if the input string is only 1 char, store that char in element
	if (strlen(input) == 1) {
		element = input[0];
		// If element is in the selections string, set output to element and found to true
		if (strchr(selections, element) != NULL) {
			*output = element;
			found = true;
		}
	}
	return found;
} //findElement

/* Creates a node with given name and price. Stores that node by price,
in ascending order. */
void insertNode(SectionNode** headPtr, const char* sectName, const double sectPrice)
{
	SectionNode* newPtr = malloc(sizeof(SectionNode));

	if (newPtr != NULL)
	{
		strcpy(newPtr->name, sectName); //setting node's name to name
		newPtr->price = sectPrice; // setting node's price to price
		newPtr->numSold = 0; //initializing numSold to 0
		newPtr->nextPtr = NULL;

		SectionNode* previousPtr = NULL;
		SectionNode* currentPtr = *headPtr;

		//inserting nodes with highest price first
		while (currentPtr != NULL && newPtr->price > currentPtr->price)
		{
			//go to next node
			previousPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
		}

		// if NULL, inserting as 1st node!
		if (previousPtr == NULL)
		{
			*headPtr = newPtr;
		}
		// currentPtr pointing to node AFTER insertion point
		else
		{
			//previous ptr needs to point to current node
			previousPtr->nextPtr = newPtr;
		}

		// Connect new node into list
		newPtr->nextPtr = currentPtr;

	}
	//null pointer from memory allocation
	else
	{
		printf("No memory to create node for %s\n", sectName);
	}

} // insertNode

/* Returns true if user enter correct pin in ATTEMPTS or less tries. */
bool isPinValid() {

	int attempts = 0;
	char pin[COLUMNS] = { " " };
	bool keepGoing = true;
	bool access = false;
	/* Loop will execute until attempts = ATTEMPTS, or input = PIN */
	while (keepGoing == true) {
		if (attempts < ATTEMPTS) {
			printf("\nEnter admin pin to set up charity ticket sales\n");
			fgets(pin, COLUMNS, stdin);
			formatString(pin);

			if (strcmp(pin, PIN) == 0) {
				access = true;
				keepGoing = false;
			}
			else {
				attempts++;
			}
		}
		else {
			access = false;
			keepGoing = false;
		}
	}
	return access;
} //isPinValid

/* This function prompts the user to enter their seat level. If a valid
selection is entered, the user is asked to confirm their selection. Next,
the user is prompted to enter a seating package. If a valid package is
entered, the user is asked to confirm the package.

The user is then asked to enter a valid 5-digit zipcode, and is asked if
they would like a receipt. The user is prompted to enter the pin if they
select quit during the seat level selection. If the correct pin is entered,
the charity is calculated and the function ends.*/
void purchaseTickets(SectionNode** startPtr, const double charityPercentage, double* charityTotal) {

	double calculateSale(SectionNode * sectionPtr, int package);
	void displayPackages();
	int selectPackage();
	void enterZipCode(double sale);
	void displayReceipt(char* section, int package, double sale, double charityDonation);

	char input[COLUMNS] = { " " };
	bool keepGoing = true;
	SectionNode* sectionPtr = NULL;

	//Initially opening the receipts file at start of sales
	

		while (keepGoing) {
			displayPricing(startPtr, charityPercentage);
			puts("\nEnter your seat level, or press Q to (Q)uit");
			fgets(input, COLUMNS, stdin);
			formatString(input);

			//if seating section exists
			if ((sectionPtr = search(startPtr, input)) != NULL) {
				printf("Is this selection[%s] correct, ", sectionPtr->name);
				if (yesOrNo() == CONFIRMATIONS[0]) {
					displayPackages();
					int package = selectPackage(); //returns a package number after the user enters valid input
					double sale = calculateSale(sectionPtr, package); // returns the value of the current sale
					enterZipCode(sale);
					printf("Do you want a receipt, ");
					if (yesOrNo() == CONFIRMATIONS[0]) {
						displayReceipt(sectionPtr->name, package, sale, (sale * charityPercentage / 100));
					}
				}
			}
			// If the input is 1 char long and the first index == Q
			else if (strlen(input) == 1 && input[0] == QUIT) {
				//I calculate the total charity once the user selects CONFIRMATIONS[4] (Quit)
				if (isPinValid()) {
					keepGoing = false;
				}
			}
		}
	
	
} //purchaseTickets

/* This function searches for, and returns a node with the matching name */
SectionNode* search(SectionNode** head, char* target)
{
	SectionNode* current = *head;  // Setting current node to head of list
	SectionNode* answer = NULL;  // Value to be returned, initially set to NULL
	bool found = false; // Will be set to true if name is found

	while (current != NULL && found != true)
	{
		//if node's name == target, set found to true and answer to the current node
		if (strcmp(current->name, target) == 0) {
			answer = current;
			found = true;
		}
		current = current->nextPtr;
	}
	return answer;

} // search

/* This function prompts the user to enter a package number. If a
valid package number is entered, the user is asked if their selection
is correct. If the user enters yes, the package number is returned as
an integer.*/
int selectPackage() {

	char buffer[COLUMNS] = { " " };
	bool valid = false;
	char package = ' ';

	do {
		puts("\n\nEnter 1, 2, 3 or 4 to select your package");
		fgets(buffer, COLUMNS, stdin);
		formatString(buffer);

		if (findElement(buffer, PACKAGE_NUMS, &package)) {

			printf("Is this selection[%c] correct, ", package);

			if (yesOrNo() == CONFIRMATIONS[0]) {
				valid = true;
			}
		}
	} while (valid == false);

	int selection = package - '0'; // converting the selection character to an int
	return selection;
} // selectPackage

/* This function returns true if a valid double value between
minimum and maximum was entered. */
bool validateDouble(const char* buff, double* validDbl, const double minimum, const double maximum)
{
	char* end;
	char buffCopy[COLUMNS];
	strcpy(buffCopy, buff); //creating copy of buff so i can modify it.

	double dblTest = strtod(buffCopy, &end);
	bool valid = false;
	//if end == buff, then no double values were read and the inital string was stored in end
	if (end == buffCopy) {
		valid = false;
	}
	//if end contains anything besides '\n', then there are extra characters after the integer
	else if ('\0' != *end) {
		valid = false;
	}

	else if (dblTest < minimum || dblTest > maximum) {

		valid = false;

	}
	else {
		*validDbl = dblTest; //set the valid double to the value of dblTest
		valid = true;
	}
	return valid;

} // validateDouble

/* This function returns true if a valid zipcode was entered. So any 5 digit integer.*/
bool validateZipCode(const char* zipCode, int* validInt)
{
	bool valid = false;
	size_t zipLength = strlen(zipCode); //storing length of the input

	// if the input is 5 chars long and only contains 0 - 9, return true
	if (zipLength == 5 && strspn(zipCode, NUMBERS) == zipLength) {
		valid = true;
	}

	return valid;

} // validateZipCode

/* This function prompts (y)es or (n)o? until the user enters
a valid confirmation. Once a valid input is entered, the toupper
function is used on the input, and the value is returned.*/
char yesOrNo() {

	char input[COLUMNS] = { " " };
	bool valid = false;
	char item = ' ';

	do {
		puts("(y)es or (n)o?");
		fgets(input, COLUMNS, stdin);
		formatString(input);
		if (findElement(input, CONFIRMATIONS, &item)) {
			valid = true;
		}
	} while (valid == false);

	return item;

} // yesOrNo

