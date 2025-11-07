#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <string>

class Student {
private:
    int rollNo;
    std::string name;
    float marks;

public:
    Student() : rollNo(0), name(""), marks(0.0f) {}
    Student(int r, std::string n, float m) : rollNo(r), name(n), marks(m) {}

    void display() const {
        std::cout << std::setw(10) << rollNo
                  << std::setw(20) << name
                  << std::setw(10) << marks << "\n";
    }

    int getRollNo() const { return rollNo; }
    std::string getName() const { return name; }
    float getMarks() const { return marks; }

    static void header() {
        std::cout << std::setw(10) << "Roll No"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Marks\n";
        std::cout << std::string(40, '-') << "\n";
    }
};

void loadStudents(std::vector<Student>& students, const std::string& filename) {
    std::ifstream file(filename);
    int roll;
    std::string name;
    float marks;

    while (file >> roll >> std::ws && std::getline(file, name, ',') && file >> marks) {
        students.emplace_back(roll, name, marks);
    }
    file.close();
}

void saveStudents(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& s : students) {
        file << s.getRollNo() << " " << s.getName() << "," << s.getMarks() << "\n";
    }
    file.close();
}

void displayAll(const std::vector<Student>& students) {
    Student::header();
    for (const auto& s : students) s.display();
}

void displayTopper(const std::vector<Student>& students) {
    auto top = *std::max_element(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getMarks() < b.getMarks();
        });
    std::cout << "\nTopper:\n";
    Student::header();
    top.display();
}

void displayAverage(const std::vector<Student>& students) {
    float total = std::accumulate(students.begin(), students.end(), 0.0f,
        [](float sum, const Student& s) {
            return sum + s.getMarks();
        });
    std::cout << "\nAverage Marks: " << total / students.size() << "\n";
}

void sortByMarks(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getMarks() > b.getMarks();
        });
}

void searchByName(const std::vector<Student>& students) {
    std::string query;
    std::cin.ignore();
    std::cout << "Enter name to search: ";
    std::getline(std::cin, query);

    bool found = false;
    Student::header();
    for (const auto& s : students) {
        if (s.getName().find(query) != std::string::npos) {
            s.display();
            found = true;
        }
    }
    if (!found) std::cout << "No matching student found.\n";
}

void exportToCSV(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream out(filename);
    out << "Roll No,Name,Marks\n";
    for (const auto& s : students) {
        out << s.getRollNo() << "," << s.getName() << "," << s.getMarks() << "\n";
    }
    out.close();
    std::cout << "Data exported to " << filename << "\n";
}

void addStudent(std::vector<Student>& students) {
    int roll;
    std::string name;
    float marks;
    std::cin.ignore();
    std::cout << "Enter Roll No: ";
    std::cin >> roll;
    std::cin.ignore();
    std::cout << "Enter Name: ";
    std::getline(std::cin, name);
    std::cout << "Enter Marks: ";
    std::cin >> marks;
    students.emplace_back(roll, name, marks);
    saveStudents(students, "students.txt");
    std::cout << "Student added.\n";
}

void removeStudent(std::vector<Student>& students) {
    int roll;
    std::cout << "Enter Roll No to remove: ";
    std::cin >> roll;
    auto it = std::remove_if(students.begin(), students.end(),
        [roll](const Student& s) {
            return s.getRollNo() == roll;
        });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveStudents(students, "students.txt");
        std::cout << "Student removed.\n";
    } else {
        std::cout << "Roll No not found.\n";
    }
}

int main() {
    std::vector<Student> students;
    loadStudents(students, "students.txt");

    int choice;
    do {
        std::cout << "\n--- Student Record Analyzer ---\n";
        std::cout << "1. Display All Records\n";
        std::cout << "2. Display Topper\n";
        std::cout << "3. Display Average Marks\n";
        std::cout << "4. Sort by Marks (Descending)\n";
        std::cout << "5. Search by Name\n";
        std::cout << "6. Export to CSV\n";
        std::cout << "7. Add New Record\n";
        std::cout << "8. Remove Record by Roll No\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: displayAll(students); break;
            case 2: displayTopper(students); break;
            case 3: displayAverage(students); break;
            case 4: sortByMarks(students); displayAll(students); break;
            case 5: searchByName(students); break;
            case 6: exportToCSV(students, "exported_students.csv"); break;
            case 7: addStudent(students); break;
            case 8: removeStudent(students); break;
            case 0: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}