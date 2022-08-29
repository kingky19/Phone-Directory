// Program to implement a phone directory
// Group 6: Kyle King, Brandon Wagoner, Emily Morales, Adam Lynch, Jacob Turnage

#include <iostream>
#include <fstream>
using namespace std;

const int BUFFERSIZE = 30;	// Maximum size for a name/category
const int PHONESIZE = 13;	// Maximum size for a phone number

// Structure used to define a phone directory entry
struct PDEntry
{
	char name[BUFFERSIZE];
	char phone[PHONESIZE];
	char category[BUFFERSIZE];
};

// type definition to define the data type for the list data
typedef PDEntry ListItemType;

// create the node structure
struct Node
{
	ListItemType item;
	Node* next;
};

/* The removeEntry function allows the user to remove a single entry that is
*  located in the 'Data.txt' file. The user is prompted with a numbered list of
*  the entries and are then asked to choose which numbered entry to remove.
*/
void removeEntry(Node*& head);

/* The displayForChangeRemove function is used to either change or remove an entry
*  in the 'Data.txt' file. The user would choose which action they would like to do
*  and are then prompted appropriately to alter an entry of their choosing.
*/
int displayForChangeRemove(Node* head, const char operation[]);

/* The changeEntry function allows the user, if they decide to do so, change an entry
*  located in the 'Data.txt' file. This function allows the user to pick which entry
*  they wish to change. They can change/update the name, phone number, and category of
*  an entry of their choosing.
*/
void changeEntry(Node* head);

/* The find function searches for the list in the 'Data.txt' file. This is
*  essentially used for locating the node position the user wishes to delete
*  or remove.
*/
Node* find(Node* head, int nodeNumber);

/* The addEntry function allows the user, if they choose to do so, add an entry
*  into the 'Data.txt' file. A single entry consists of a name, a phone number, and
*  a category.
*/
bool addEntry(Node*& head, Node*& lastNode, istream& input, bool fromKbd);

/* The addToList function is responsible for adding a name, phone number, and a
*  category that the user inputs into the 'Data.txt' file.
*/
bool addToList(Node*& head, Node*& lastNode, ListItemType data);

/* The displayNamesInCategory function allows the user to search through the
*  'Data.txt' file and display all of the entries of a specific category.
*/
void displayNamesInCategory(Node* head);

/* The loadFromFile function is used as a means of loading any information located
*  in the 'Data.txt' file into the terminal(program) menu. This function also
*  displays an error if the function can't load from the file.
*/
void loadFromFile(Node*& head, Node*& lastNode);

/* The exitSaveToFile function is responsible for saving any changes that the user
*  makes to 'Data.txt' file after exiting the program.
*/
void exitSaveToFile(Node* head);

/* The deleteList function cleans out the memory that has been allocated for the linked list so
*  there is not a memory leak.
*/
void deleteList(Node*& head);

/* The numberedDisplay function prints, in the program, a list of the entries
*  located in the 'Data.txt' file. This function is only used in the removeEntry and
*  changeEntry functions as a way for the user to select which entry they wish to
*  remove or change.
*/
void numberedDisplay(Node* head, bool numbers, const char categorySearch[]);

int main()
{
	Node* head = NULL;
	Node* lastNode = NULL;
	int choice;

	loadFromFile(head, lastNode);

	do
	{
		cout << "\n1. Add Entry\n";
		cout << "2. Change Entry\n";
		cout << "3. Display Names in Category\n";
		cout << "4. Display All\n";
		cout << "5. Remove Entry\n";
		cout << "6. Exit\n";
		cout << endl;
		cout << "Select: ";

		// Gets the numeric entry from the menu
		cin >> choice;

		// Makes the 'enter' key that was pressed after the numeric entry be ignored
		cin.ignore();

		switch (choice)
		{
		case 1:
			addEntry(head, lastNode, cin, true);
			break;
		case 2:
			changeEntry(head);
			break;
		case 3:
			displayNamesInCategory(head);
			break;
		case 4:
			numberedDisplay(head, false, "ALL");
			break;
		case 5:
			removeEntry(head);
			break;
		}
	} while (choice != 6);

	exitSaveToFile(head);
	deleteList(head);
	return 0;
}

void removeEntry(Node*& head)
{
	/* Display a list; items should be numbered. The program asks user which
	*  contact they wish to remove by the user selecting the entry they want
	*  to remove by entering line number. Once user chooses line number,
	*  the contact should be removed from list.
	*/

	/* Tell the computer to go through the List a
	*  certain number of times before equalling the
	*  value given by the user
	*/

	Node* temp;
	int choice = displayForChangeRemove(head, "remove");

	if (choice < 1)
	{
		printf("Not a vaild entry");
		return;
	}

	if (choice == 1)
	{
		// Make temp point to node 1
		temp = head;

		// Make head point to node 2
		head = head->next;

		// Delete node 1
		delete temp;
	}
	else
	{
		/* Make sure user doesn't enter a number that's too high
		* (above PHONESIZE, which is 13). If they do, an error message
		* is printed to the screen.
		*/

		temp = find(head, choice);
		if (temp == NULL)
		{
			printf("\nNot a vaild entry.\n");
			return;
		}

		Node* prev = find(head, choice - 1);

		temp = prev->next;
		prev->next = temp->next;
		delete temp;
	}
}

int displayForChangeRemove(Node* head, const char operation[])
{
	/* This section of the code allows the user to change and/or remove an entry
	*  in the list.
	*  'value' represents the number choice that a user selects.
	*  'operation' is used to represent if the user chooses to make a change or
	*  remove an entry.
	*/

	int value;
	numberedDisplay(head, true, "ALL");

	cout << "Which contact do you wish to " << operation << "? (Please enter the corresponding line number): ";
	cin >> value;
	cin.ignore();
	return value;
}

void changeEntry(Node* head)
{
	/* This section of the code prints out the contacts in a numbered list format
	*  then asks the user which entry they wished to change by receiving a number input
	*  based on who they wish to change. Then displays the name, phone number, and
	*  category of the selected person. Then asks which item they wish to change for
	*  that contact.
	*/

	Node* temp;
	int option;
	int choice = displayForChangeRemove(head, "change");
	if (choice < 1)
	{
		printf("\nNot a vaild entry.\n");
		return;
	}

	// Searches through the linked list to make sure the choice is possible and valid
	temp = find(head, choice);
	if (temp == NULL)
	{
		printf("\nNot a valid entry.\n");
		return;
	}

	/* The 'printf' statements print to the terminal screen the numbered list of
	*  the information of a chosen entry by the user.
	*/
	printf("1. %s\n", temp->item.name);
	printf("2. %s\n", temp->item.phone);
	printf("3. %s\n", temp->item.category);

	cout << "\nWhich item do you wish to change? (Choose 1 for Name, 2 for Phone Number, and 3 for Category): ";
	cin >> option;
	cin.ignore();
	if (option < 1 || option > 3)
	{
		printf("Not a valid entry.\n");
		return;
	}

	/* The following code below goes through the 'switch' statement. So, it essentially
	*  figures out what choice/option the user selected to make a change to.
	*/
	cout << "\nWhat do you wish to change the item to? ";

	switch (option)
	{
	case 1:
		cin.getline(temp->item.name, BUFFERSIZE);
		break;
	case 2:
		cin.getline(temp->item.phone, PHONESIZE);
		break;
	case 3:
		cin.getline(temp->item.category, BUFFERSIZE);
		break;
	}
	cout << endl;
}

Node* find(Node* head, int nodeNumber)
{
	/* This section of the code is looking for a certain node based on user
	 * input for change or remove entries.
	 */

	int x = 1;
	Node* temp;
	for (temp = head; x < nodeNumber && temp != NULL; temp = temp->next)
	{
		x++;
	}
	return temp;
}

bool addEntry(Node*& head, Node*& lastNode, istream& input, bool fromKbd)
{
	/* This part of the code is getting an input from either the user or the "data.txt" file
	*  and then calling the addToList function to add the input to the linked list.
	*/
	ListItemType data;
	if (fromKbd)
	{
		cout << "\nEnter name:\n";
	}

	// 'input' equivalent to 'cin.getline'
	input.getline(data.name, BUFFERSIZE);

	if (fromKbd)
	{
		cout << "\nEnter phone:\n";
	}
	input.getline(data.phone, PHONESIZE);

	if (fromKbd)
	{
		cout << "\nEnter category:\n";
	}
	input.getline(data.category, BUFFERSIZE);

	return addToList(head, lastNode, data);
}

bool addToList(Node*& head, Node*& lastNode, ListItemType data)
{
	/* This section of the code is creating a new node for every entry that is
	*  being put into the linked list. It returns true is it was able to add
	*  the entry and returns false is it was not able to.
	*/

	Node* temp = new Node;
	if (temp == NULL) return false;
	temp->item = data;
	temp->next = NULL;

	// When head is seen as the end of the linked list
	if (head == NULL)
	{
		head = temp;
		lastNode = temp;
	}
	else
	{
		lastNode->next = temp;
		lastNode = temp;
	}

	return true;
}

void displayNamesInCategory(Node* head)
{
	/* This section: program asks user to enter a category.
	* Once category is entered, program displays a list of names and numbers, in
	* a table-like form. Also, items should be numbered.
	*/

	char categorySearch[BUFFERSIZE];
	printf("Enter a category: \n");
	cin.getline(categorySearch, BUFFERSIZE);

	numberedDisplay(head, false, categorySearch);
}

void loadFromFile(Node*& head, Node*& lastNode)
{
	/* This section of the code tries to open the "data.txt" file. If it can not then the
	*  error message will be displayed. The while loop checks for the end of the file or if
	*  there is a line break at the end of the file.
	*/

	ifstream inFile;
	inFile.open("data.txt");

	if (!inFile)
	{
		cerr << "ERROR: input file could not be opened\n";
		return;
	}

	while (!inFile.eof() && inFile.peek() != EOF)
	{
		addEntry(head, lastNode, inFile, false);
	}

	inFile.close();
}

void exitSaveToFile(Node* head)
{
	/* When user chooses to exit, the program should open a text file ('data.txt')
	*  and write all of the contacts currently in list to text file.
	*
	*  If file cannot open for writing, the program should display an error message
	*  stating that data file cannot be opened for writing.
	*/
	ofstream outFile;
	outFile.open("data.txt");

	if (!outFile)
	{
		cerr << "ERROR: The date file could not be opened for writing. \n";
		return;
	}
	for (Node* temp = head; temp != NULL; temp = temp->next)
	{
		outFile << temp->item.name << endl;
		outFile << temp->item.phone << endl;
		outFile << temp->item.category << endl;
	}
	cout << "The file was overwritten!\n";
	outFile.close();
}


void deleteList(Node*& head)
{
	/* This sections deletes the linked list from memory so that it
	*  does not cause a memory leak. Also deletes contact when called on.
	*/

	while (head != NULL)
	{
		Node* temp = head->next;
		delete head;
		head = temp;
	}
	head = NULL;
}

void numberedDisplay(Node* head, bool numbers, const char categorySearch[])
{
	/* This part of the code goes through and prints out the numbers beside the
	*  entries for when the user wishes to change or delete an entry.
	*/

	int listNumber = 1;
	printf("\n");
	for (Node* temp = head; temp != NULL; temp = temp->next)
	{
		// Searches for the specific catagory or all of them
		if (!strcmp(temp->item.category, categorySearch) || !strcmp(categorySearch, "ALL"))
		{
			// Inserts numbers next to the contact for the delete and change entry functions
			if (numbers)
			{
				printf("%i. ", listNumber);
			}
			printf("%-35s", temp->item.name);
			printf("%-18s", temp->item.phone);
			printf("%-35s\n", temp->item.category);

			listNumber++;
		}
	}
	printf("\n");
}