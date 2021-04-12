/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */

#include <iostream>
#include <string>
#include <fstream> 
#include <utility>
#include <iomanip>
#include "DVD.h"
#include "BSTree.h"

using namespace std;

// Process Inventory Entry
void parseInventoryEntry(string input, string &title, int &available, int &rented) {
    // Initialize pointer that will scan through the input string
    int idx = 0, n = int(input.size()) - (int(input[int(input.size())-1]) == 13);
    
    // Read and process movie title
    while (idx < n && input[idx] != ',') {
        if (input[idx] != '"') title += input[idx];
        idx++;
    }
    
    // Read and process number of available copies 
    string num = "";
    idx++;
    while (idx < n && input[idx] != ',') {
        num += input[idx];
        idx++;
    }
    available = stoi(num);
    
    // Read and process number of rented copies
    num = "";
    idx++;
    while (idx < n) {
        num += input[idx];
        idx++;
    }
    rented = stoi(num);
}

// Process Transaction entry and check for validity
bool validTransaction(string input, string &operation, string &title, int &val) {
    // Initialize variable that will scan through the input string
    int idx = 0, n = int(input.size()) - (int(input[int(input.size())-1]) == 13);
    
    // If input is blank then input is not valid
    if (idx >= n) return false;
    
    // Read and process operation
    while (idx < n && input[idx] != ' ') {
        operation += input[idx];
        idx++;
    }
    
    // If invalid operation is encountered, input is not valid
    if (operation != "add" && operation != "remove" && operation != "rent" && operation != "return") return false;
    
    // Read and process title 
    idx++;
    
    // If there is no input afterward, input is invalid
    if (idx >= n) return false;
    
    // Variable to mark status of reading input title
    bool title_start = false, title_end = false;
    
    while (idx < n && input[idx] != ',') {
        // If an apostrophe is reached 
        if (input[idx] == '"') {
            // If reading title haven't started, then start reading title
            if (!title_start) title_start = true;
            // Else, then reading title process has ended
            else title_end = true;
        }
        // Else
        if (input[idx] != '"') {
            // If reading title process is not initiated or is already ended, then the input will be invalid
            if (title_start && !title_end) title += input[idx];
            else return false;
        }
        idx++;
    }
    
    // If operation is "rent" or "return", input is invalid if there is still data after input
    idx++;
    if (operation == "rent" || operation == "return") return idx >= n;
    
    // If operation is not "rent" or "return" but does not have anything else after it then input is invalid
    else if (idx >= n) return false;
    
    // Read and process amount of added (or removed) copies
    string num = "";
    while (idx < n) {
        // If non-numeric character is found, input is invalid
        if (input[idx] < 48 || input[idx] >= 58) return false;
        num += input[idx];
        idx++;
    }
    val = stoi(num);
    
    // At this point, input can be considered as valid
    return true;
}

// Print input to error.log if input is invalid
void outputError(string input, bool &foundError) {
    // Create ofstream object for error.log
    ofstream erofs;
    if (!foundError) {
        erofs.open("error.log");
        foundError = true;
    }
    else erofs.open("error.log", ios_base::app);
    
    // Print error to file 
    erofs << input << '\n';
    
    // Close file
    erofs.close();
}

// Read inventory file and populate BST
void processInventory(BST<DVD>* &inventory, string file) {
    // Open inventory file
    ifstream ifs(file);
    
    // If file can be opened
    if (ifs.is_open()) {
        // Process input by line
        string input;
        while (getline(ifs, input)) {
            if (input == "") break;
            string title = "";
            int available, rented;
            // Parse data from input
            parseInventoryEntry(input, title, available, rented);
            
            // Insert new movie into BST
            inventory->Insert(DVD(title, available, rented));
        }
        // Close file
        ifs.close();
    }
}

// Support "add" operation
void addMovie(BST<DVD>* &tree, string title, int val) {
    // Look for the title in the BST
    DVD target(title, val, 0);
    pair<Node<DVD>*, Node<DVD>*> temp = tree->Search(target);
    
    // If title does not exist, then insert title into BST
    if (temp.second == nullptr) tree->Insert(temp.first, target);
    else {
        // If title exists, update amount of available copies
        int curr = temp.second->getPayload()->getAvailable();
        temp.second->getPayload()->setAvailable(curr + val);
    }
}

// Support "remove" operation
void removeMovie(BST<DVD>* &tree, string input, string title, int val, bool &foundError) {
    // Look for title in the BST
    DVD target(title, val, 0);
    pair<Node<DVD>*, Node<DVD>*> temp = tree->Search(target);
    
    // If title does not exist, print input to error.log
    if (temp.second == nullptr) {
        outputError(input, foundError);
        return;
    }
    
    // If title exists, then update number of available copies or delete title from the BST (if applicable)
    int curr = temp.second->getPayload()->getAvailable();
    if (curr != val || temp.second->getPayload()->getRented()) temp.second->getPayload()->setAvailable(curr - val);
    else tree->Delete(target);
}

// Suport "rent" operation
void rentMovie(BST<DVD>* &tree, string input, string title, bool &foundError) {
    // Look for title in the BST
    DVD target(title, 0, 0);
    pair<Node<DVD>*, Node<DVD>*> temp = tree->Search(target);
    
    // If title does not exist, print input to error.log
    if (temp.second == nullptr) {
        outputError(input, foundError);
        return;
    }
    
    // If title exists, update number of rented and available copies
    int curr_available = temp.second->getPayload()->getAvailable(), curr_rent = temp.second->getPayload()->getRented();
    temp.second->getPayload()->setAvailable(curr_available - 1);
    temp.second->getPayload()->setRented(curr_rent + 1);
}

void returnMovie(BST<DVD>* &tree, string input, string title, bool &foundError) {
    // Look for title in the BST
    DVD target(title, 0, 0);
    pair<Node<DVD>*, Node<DVD>*> temp = tree->Search(target);
    
    // If title does not exist, print input to error.log
    if (temp.second == nullptr) {
        outputError(input, foundError);
        return;
    }
    
    // If title exists, update number of rented and available copies
    int curr_available = temp.second->getPayload()->getAvailable(), curr_rent = temp.second->getPayload()->getRented();
    temp.second->getPayload()->setAvailable(curr_available + 1);
    temp.second->getPayload()->setRented(curr_rent - 1);
}

// Process transaction file
void processTransaction(BST<DVD>* &inventory, string file, bool &foundError) {
    // Open transaction log
    ifstream ifs(file);
    
    // If file can be opened
    if (ifs.is_open()) {
        // Process input by line
        string input;
        while (getline(ifs, input)) {
            if (input == "") break;
            string operation = "", title = "";
            int val;
            // Determine if input is valid
            bool possible = validTransaction(input, operation, title, val);
            
            // If input is invalid, output line to error.log
            if (!possible) {
                outputError(input, foundError);
                continue;
            }
            
            // If input is valid, carry out the neccessary operation 
            if (operation == "add") addMovie(inventory, title, val);
            else if (operation == "remove") removeMovie(inventory, input, title, val, foundError);
            else if (operation == "rent") rentMovie(inventory, input, title, foundError);
            else returnMovie(inventory, input, title, foundError);
           
        }
        ifs.close();
    }
}

// Get the longest title (or number) for the purpose of styling in the report
int getMaxLength(Node<DVD>* root, bool getTitle) {
    // Get the longest title (or number) in the left subtree
    int left = (root->getLeft() ? getMaxLength(root->getLeft(), getTitle) : 0);
    
    // Get the longest title (or number) in the right subtree
    int right = (root->getRight() ? getMaxLength(root->getRight(), getTitle) : 0);
    
    // Get length of title (or number) of current node
    int curr = getTitle ? int((root->getPayload()->getTitle()).size()) : int(to_string(root->getPayload()->getAvailable()).size());
    
    // Return max value between left subtree, right subtree, and current node
    return max(max(left, right), curr);
}

void printReport(Node<DVD>* curr, int max_title_len, int max_available_len) {
    // If current node is nullptr, then do nothing
    if (curr == nullptr) return;
    
    // If current node has left child, print everything in left subtree first
    if (curr->getLeft()) printReport(curr->getLeft(), max_title_len, max_available_len);
    
    // Print data from current node
    cout << curr->getPayload()->getTitle() << setw(max_title_len - int((curr->getPayload()->getTitle()).size())) << '\t';
    cout << curr->getPayload()->getAvailable() << setw(max_available_len - int(to_string(curr->getPayload()->getAvailable()).size())) << '\t';
    cout << curr->getPayload()->getRented() << '\n';
    
    // If current node has right child, print everything in right subtree 
    if (curr->getRight()) printReport(curr->getRight(), max_title_len, max_available_len);
}


int main() {
    // Prompt for necessary filename
	string inventory_file, transaction_log;
    cout << "Enter inventory file name: ";
    cin >> inventory_file;
    cout << "Enter transaction log file: ";
    cin >> transaction_log;
    
    // Create BST
    BST<DVD>* inventory_tree = new BST<DVD>();
    
    // Process Inventory file
    processInventory(inventory_tree, inventory_file);
    
    // If tree is not empty
    if (inventory_tree->getRoot()) {
        // Process transaction log
        bool foundError = false;
        processTransaction(inventory_tree, transaction_log, foundError);
    
        // If tree is not empty, then print report
        if (inventory_tree->getRoot()) {
            int max_title_len = getMaxLength(inventory_tree->getRoot(), true), max_available_len = getMaxLength(inventory_tree->getRoot(), false);
            printReport(inventory_tree->getRoot(), max_title_len, max_available_len);
        }
        
        // Deallocate BST
        delete inventory_tree;
    }
    // If tree is empty, it means that inventory file cannot be founded
    else cout << "Inventory not found" << '\n';
    return 0;
}

