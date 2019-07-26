#include <iostream>
using namespace std;

void showMenu()
{
	cout << "1.search" << endl;
	cout << "2.view record" << endl;
	cout << "3.quit" << endl;
}

int  getInput()
{
	cout << "enter selection" << endl;
	int input;
	cin >> input;
	return input;
}

void processSelection(int option)
{
	switch (option)
	{
	case 1:
	{
		cout << "searching..." << endl;
	}
	case 2:
	{
		cout << "viewing..." << endl;
	}
	case 3:
	{
		cout << "quiting..." << endl;
	}
	default:
	{
		cout << "please select an item frm the menu" << endl;
	}
	break;
	}
}


int main()
{
	showMenu();
	int selection = getInput();
	processSelection(selection);
	return 0;
}
