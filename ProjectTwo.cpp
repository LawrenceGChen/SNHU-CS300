#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Course class to store course information
class Course {
private:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

public:
    Course() {}
    
    Course(string number, string title) {
        courseNumber = number;
        courseTitle = title;
    }

    void addPrerequisite(string prereq) {
        prerequisites.push_back(prereq);
    }

    string getCourseNumber() const { return courseNumber; }
    string getCourseTitle() const { return courseTitle; }
    vector<string> getPrerequisites() const { return prerequisites; }
};

// Class to manage the course data structure and operations
class CourseManager {
private:
    vector<Course> courses;

    // Helper function to split string by delimiter
    vector<string> split(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    // Load data from file
    bool loadDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return false;
    }

    courses.clear(); // Clear existing courses before loading new ones
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        
        vector<string> parts = split(line, ',');
        if (parts.size() >= 2) {
            // Trim whitespace from course number and title
            string courseNum = parts[0];
            string courseTitle = parts[1];
            courseNum.erase(remove_if(courseNum.begin(), courseNum.end(), ::isspace), courseNum.end());
            courseTitle.erase(0, courseTitle.find_first_not_of(" "));
            courseTitle.erase(courseTitle.find_last_not_of(" ") + 1);
            
            Course course(courseNum, courseTitle);
            
            // Add prerequisites if they exist
            for (size_t i = 2; i < parts.size(); i++) {
                string prereq = parts[i];
                // Trim whitespace from prerequisite
                prereq.erase(remove_if(prereq.begin(), prereq.end(), ::isspace), prereq.end());
                if (!prereq.empty()) {
                    course.addPrerequisite(prereq);
                }
            }
            courses.push_back(course);
        }
    }
    file.close();
    return true;
}

    // Print sorted course list
    void printSortedCourses() {
        if (courses.empty()) {
            cout << "No courses loaded. Please load course data first." << endl;
            return;
        }

        vector<Course> sortedCourses = courses;
        sort(sortedCourses.begin(), sortedCourses.end(),
            [](const Course& a, const Course& b) {
                return a.getCourseNumber() < b.getCourseNumber();
            });

        cout << "\nAlphanumeric List of All Courses:" << endl;
        cout << "--------------------------------" << endl;
        for (const Course& course : sortedCourses) {
            cout << course.getCourseNumber() << ": " << course.getCourseTitle() << endl;
        }
    }

    // Print course information
    void printCourseInformation(const string& courseNumber) {
        for (const Course& course : courses) {
            if (course.getCourseNumber() == courseNumber) {
                cout << "\nCourse Number: " << course.getCourseNumber() << endl;
                cout << "Course Title: " << course.getCourseTitle() << endl;
                cout << "Prerequisites: ";
                vector<string> prereqs = course.getPrerequisites();
                if (prereqs.empty()) {
                    cout << "None" << endl;
                }
                else {
                    for (const string& prereq : prereqs) {
                        cout << prereq << " ";
                    }
                    cout << endl;
                }
                return;
            }
        }
        cout << "Course not found." << endl;
    }
};

// Main function with menu implementation
int main() {
    CourseManager manager;
    int choice = 0;
    string filename;
    string courseNumber;

    while (choice != 9) {
        cout << "\nWelcome to the Course Planner\n" << endl;
        cout << "1. Load Course Data" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course Information" << endl;
        cout << "9. Exit\n" << endl;
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter filename: ";
                getline(cin >> ws, filename);
                if (manager.loadDataFromFile(filename)) {
                    cout << "Course data loaded successfully." << endl;
                }
                break;

            case 2:
                manager.printSortedCourses();
                break;

            case 3:
                cout << "Enter course number: ";
                cin >> courseNumber;
                manager.printCourseInformation(courseNumber);
                break;

            case 9:
                cout << "Thank you for using the Course Planner!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}