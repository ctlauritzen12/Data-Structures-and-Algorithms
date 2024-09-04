//============================================================================
// Name        : Course.cpp
// Author      : Chris Lauritzen
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project 2 CS300 Reads CSV and prints appropiate information
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


/* define a structure to hold course information
* and a vector to hold prerequisites */
struct Course {

    string courseNum; // unique identifier
    string courseName;
    vector<string> preReqs;
};

// Internal structure for tree node
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
    Node(Course aCourse) :
        Node() {

        this->course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */

class CourseBinaryTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printSampleSchedule(Node* node);
    void printCourseInfo(Node* node, string course);

public:
    CourseBinaryTree();
    virtual ~CourseBinaryTree();
    void RemoveBinaryTree(Node* node);
    void PrintSampleSchedule();
    void PrintCourseInfo(string courseNum);
    int NumPrerequisiteCourses(Course course);
    void Insert(Course course);
};

CourseBinaryTree::CourseBinaryTree() {
    root = nullptr;
}

//CourseBinaryTree Deconstructor recursive
CourseBinaryTree::~CourseBinaryTree() {
    RemoveBinaryTree(root);
}

/*Delete the Binary Search Tree*/
void CourseBinaryTree::RemoveBinaryTree(Node* node) {
    if (node) {
        RemoveBinaryTree(node->left);
        RemoveBinaryTree(node->right);
        delete node;
    }
}

//Add a node to the Binary Tree
void CourseBinaryTree::addNode(Node* node, Course course) {
    if (node != nullptr && node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->left == nullptr)
            node->left = new Node(course);
        else
            this->addNode(node->left, course);
    }
    else if (node != nullptr && node->course.courseNum.compare(course.courseNum) < 0) {
        if (node->right == nullptr) {
            node->right = new Node(course);
            return;
        }
        else {
            this->addNode(node->right, course);
        }
    }
}
//Insert a node in to CourseBinaryTree
void CourseBinaryTree::Insert(Course course) {

    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}
// recursive use for printSamleSchedule
void CourseBinaryTree::PrintSampleSchedule() {
    this->printSampleSchedule(root);
}

void CourseBinaryTree::printSampleSchedule(Node* node) {
    if (node != nullptr) {
        printSampleSchedule(node->left);
        cout << node->course.courseNum << ", " << node->course.courseName << endl;
        printSampleSchedule(node->right);
    }
}
int CourseBinaryTree::NumPrerequisiteCourses(Course course) {
    int count = 0;
    for (unsigned int i = 0; i < course.preReqs.size(); i++)
        if (course.courseName.length() > 0) {
            count++;
        }
    return count;
}

void CourseBinaryTree::PrintCourseInfo(string courseNum) {
    this->printCourseInfo(root, courseNum);
}

void CourseBinaryTree::printCourseInfo(Node* curr, string courseNum) {

    while (curr != nullptr) {

        if (curr->course.courseNum.compare(courseNum) == 0) {
            cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;

            unsigned int size = NumPrerequisiteCourses(curr->course);
            cout << "Prerequsites: ";
            for (unsigned int i = 0; i < size; i++) {
                cout << curr->course.preReqs.at(i);

                if (i != size - 1);
                cout << ", ";
                if (size == 0)
                    cout << "No Prerequistes required.";

            }
            return;
        }
        else if (courseNum.compare(curr->course.courseNum) < 0)
            curr = curr->left;
        else
            curr = curr->right;
    }
    cout << "Course " << courseNum << " not found" << endl;
}


//Load Courses from cin choice for csv file
bool loadCourses(string cvsPath, CourseBinaryTree* courses) {

    // Open course file, get/ separate each line & insert into BST

    ifstream courseFile(cvsPath);
    if (courseFile.is_open()) {

        while (!courseFile.eof()) {

            // A vector will hold each csv that a course contains

            vector<string> courseInfo;
            string courseData;
            getline(courseFile, courseData);
            while (courseData.length() > 0) {

                // Get substring of each course data, add to vector & delete 

                unsigned int comma = courseData.find(',');
                if (comma < 100) {
                    courseInfo.push_back(courseData.substr(0, comma));
                    courseData.erase(0, comma + 1);

                }
                // Add the last course after the final comma

                else {
                    courseInfo.push_back(courseData.substr(0, courseData.length()));
                    courseData = "";
                }
            }
            // Load the separated values into a course, 
            //insert into Binary Search Tree and close file

            unsigned int i = 0;
            Course course;
            course.courseNum = courseInfo[0];
            course.courseName = courseInfo[1];

            for (unsigned int i = 2; i < courseInfo.size(); i++) {
                course.preReqs.push_back(courseInfo[i]);
            }
            courses->Insert(course);
        }
        courseFile.close();
        return true;
    }


    return false;
}

// command line handler
int main(int argc, char* argv[]) {
    string cvsPath;
    string courseId;

    switch (argc) {
    case 2:
        cvsPath = string(argv[1]);
        break;
    case 3:
        cvsPath = string(argv[1]);
        courseId = string(argv[2]);
        break;
    default:
        cvsPath = "ABC University.txt";
        break;
    }

    CourseBinaryTree* courses;
    courses = new CourseBinaryTree();

    int choice = 0;
    bool success = 1;

    while (choice != 4) {
        cout << "Welcome to the course planner.\n" << endl;
        cout << "1. Load Courses" << endl;
        cout << "2. Display all Courses" << endl;
        cout << "3. Find Course" << endl;
        cout << "4. Exit" << endl;
        cout << endl;
        cout << "What would like to do? ";
        cin >> choice;
        while (cin.fail())
        {
            cout << "Please enter an integer characters are not an option" << endl;
            // clear error state
            cin.clear();
            // discard 'bad' character(s)
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Please enter a valid choice: ";
            cin >> choice;
        }

        switch (choice) {

        case 1:

            loadCourses(cvsPath, courses);
            break;

        case 2:

            cout << "Here is a sample schedule: \n\n";
            courses->PrintSampleSchedule();
            cout << endl;
            break;
            cout << endl;

        case 3:

            if (courses != nullptr && success) {
                if (courseId.length() == 0) {
                    cout << "What course do you want to know about? ";
                    cin >> courseId;
                    for (auto& userChoice : courseId) userChoice = toupper(userChoice);
                }
                courses->PrintCourseInfo(courseId);
                cout << endl;
                courseId = "";
                break;
            }
            else
                cout << "Load courses first - option 1\n" << endl;
            courseId = "";
            break;

        case 4:
            cout << "Thank you for using the course planner!" << endl;

            exit;
            break;

        default:
            cout << choice << " is not a valid option\n" << endl;
            break;
        }
    }
}
