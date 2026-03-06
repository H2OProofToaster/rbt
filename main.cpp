#include "structs.cpp"
#include <string>
#include <fstream>

using namespace std;

int main() {

  bool running = true;
  BST* tree = new BST();
  
  //Do actions
  while (running) {

    cout << "What is your action? (ADD, READ, DELETE, SEARCH, PRINT, QUIT)" << endl;
    string action;
    cin >> action;

    if (action == "ADD" or action == "a") {

      //Add
      cout << "Add what number? ";
      int num;
      cin >> num;

      cout << "Adding..." << endl;
      
      tree->insert(num);

      cout << "Done" << endl;
    }
    else if (action == "READ" or action == "r") {

      //Read
      cout << "What file? (Files are not guaranteed to have unique values between them, QUIT to stop reading) ";

      string fileName;
      cin >> fileName;

      if (fileName != "QUIT" or fileName != "q") { 
	cout << "Reading..." << endl;
	
	ifstream readFile(fileName);
	if (readFile.is_open()) {
	  
	  int v;
	  while (readFile >> v) { tree->insert(v); }
	}
	readFile.close();
	
	cout << "Done" << endl;
      }
    }
    else if (action == "DELETE" or action == "d") {

      //Delete
      cout << "Remove what number? ";
      int num;
      cin >> num;

      cout << "Deleting..." << endl;

      tree->remove(num);

      cout << "Done" << endl;
    }
    else if (action == "SEARCH" or action == "s") {

      //Search
      cout << "Search what number? ";
      int num;
      cin >> num;

      cout << "Searching..." << endl;

      Node* search = tree->search(num);
      if (search->data != -1) { cout << "Found" << endl; }
      else { cout << "Not found" << endl; }
    }
    else if (action == "PRINT" or action == "p") {

      //Print
      cout << "Printing..." << endl << endl;

      tree->print();

      cout << endl << "Done" << endl;
    }
    else if (action == "QUIT" or action == "q") {

      delete tree;
      running = false;
    }
    else { cout << "Not an action" << endl; }

    action.clear();
  }
  
  return 0;
}
