//============================================================================
// Name        : ABCU.cpp
// Author      : Nick Markel
//============================================================================

#include <iostream>
#include <time.h>
#include <string>

#include "CSVparser.hpp"

using namespace std;

// structure to hold course info
struct Course {
    string courseNumber;
    string title;
    string preReqOne;
    string preReqTwo;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course aCourse);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();

    void Insert(Course course);
    void Remove(string courseNumber);
    void RemoveSubTree(Node* node);

    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    RemoveSubTree(root);
}

void BinarySearchTree::RemoveSubTree(Node* node) { // Delete nodes in tree.
    if (node != nullptr) {
        if (node->left != nullptr) {
            RemoveSubTree(node->left);
        }
        if (node->right != nullptr) {
            RemoveSubTree(node->right);
        }
        delete node;
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
    removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {

        if (current->course.courseNumber.compare(courseNumber) == 0) {
            return current->course;
        }

        if (courseNumber.compare(current->course.courseNumber) < 0) {// if course is smaller than current node then traverse left
            current = current->left;
        }

        else {// else larger so traverse right
            current = current->right;
        }

        Course course;
        return course;
    }
}

/**
 * Add acourse to some node (recursive)
 *
 * @param node Current node in tree
 * @paramcoursecourse to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    if (node->course.courseNumber.compare(course.courseNumber) > 0) {// if node is larger then add to left
        if (node->left == nullptr) {// if no left node
            node->left = new Node(course);// this node becomes left
        }

        else {// else recurse down the left node
            addNode(node->left, course);
        }
    }


    else {
        if (node->right == nullptr) {// if no right node
            node->right = new Node(course);// this node becomes right
        }

        else {
            addNode(node->right, course);// recurse down the left node
        }
    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseNumber) { //Was this forgotten in the template? I needed to research to find this as the project wouldn't run without it.
    // if this node is null then return  (avoid crashing)
    if (node == nullptr) {
        return node;
    }
    // recurse down left subtree
    if (courseNumber.compare(node->course.courseNumber) < 0) {
        node->left = removeNode(node->left, courseNumber);
    }
    else if (courseNumber.compare(node->course.courseNumber) > 0) {
        node->right = removeNode(node->right, courseNumber);
    }
    else {
        // no children this is a leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // one child to the left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // one child to the right
        else if (node->right != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseNumber);
        }
    }
    return node;
}

void BinarySearchTree::inOrder(Node* node) {

    if (node != nullptr) {//if node is not equal to null ptr

        inOrder(node->left);//InOrder not left

        cout << node->course.courseNumber << ": "
            << node->course.title << ", ";
            if (!node->course.preReqTwo.empty()) {
                cout <<  node->course.preReqOne << ", " << node->course.preReqTwo << endl;
            }
            else if (!node->course.preReqOne.empty()) {
                cout <<  node->course.preReqOne << endl;
            }
            cout << endl;

        inOrder(node->right);//InOder right
    }
}


/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ": " << course.title;

    if (!course.preReqTwo.empty()) {
        cout << "Prerequisites: " << course.preReqOne << ", " << course.preReqTwo << endl;
    }
    else if (!course.preReqOne.empty()) {
        cout << "Prerequisites: " << course.preReqOne << endl;
    }

    cout << endl;
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadCourse(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseNumber = file[i][0];
            course.title = file[i][1];
            //course.preReqOne = file[i][2];
            //course.preReqTwo = file[i][3]; 
            
            // push this course to the end
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "CSCI101";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
        courseKey = "CSCI101";
    }

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            loadCourse(csvPath, bst);

            break;

        case 2:
            bst->InOrder();
            break;

        case 3:

            cout << "Input Course ";
            cin >> courseKey;
            

            course = bst->Search(courseKey);

            
            if (!course.courseNumber.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Number " << courseKey << " not found." << endl;
            }

            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
