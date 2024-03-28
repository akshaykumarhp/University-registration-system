#include "course_student_management.h"

// Implementation of Course member functions
Course::Course(string code, string title, string instructor, int capacity, vector<string> schedule, vector<string> prereqs) :
    course_code(code), title(title), instructor(instructor), max_capacity(capacity), schedule(schedule), prerequisites(prereqs) {}

string Course::get_title() const {
    return title;
}

const vector<string>& Course::get_prerequisites() const {
    return prerequisites;
}

void Course::enroll_student(const string& studentName) {
    if (enrolled_students.size() < max_capacity) {
        enrolled_students.push_back(studentName);
        cout << studentName << " enrolled in " << title << endl;
        view_schedule();
    } else {
        cout << "Sorry, " << title << " is full." << endl;
    }
}

int Course::check_available_seats() {
    return max_capacity - enrolled_students.size();
}

void Course::generate_class_schedule() {
    cout << "Schedule for " << title << ": ";
    for (const auto& slot : schedule) {
        cout << slot << ", ";
    }
    cout << endl;
}

bool Course::is_student_enrolled(const string& studentName) const {
    return find(enrolled_students.begin(), enrolled_students.end(), studentName) != enrolled_students.end();
}

void Course::view_schedule() const {
    cout << "Schedule for " << title << ":" << endl;
    cout << "Instructor: " << instructor << endl;
    cout << "Class timings: ";
    for (const auto& slot : schedule) {
        cout << slot << ", ";
    }
    cout << endl;
}

// Implementation of LectureCourse member functions
LectureCourse::LectureCourse(string code, string title, string instructor, int capacity, vector<string> schedule, string hall, vector<string> prereqs) :
    Course(code, title, instructor, capacity, schedule, prereqs), lecture_hall(hall) {}

void LectureCourse::view_schedule() const {
    Course::view_schedule();
    cout << "Lecture Hall: " << lecture_hall << endl;
}

// Implementation of SeminarCourse member functions
SeminarCourse::SeminarCourse(string code, string title, string instructor, int capacity, vector<string> schedule, vector<string> topics, vector<string> prereqs) :
    Course(code, title, instructor, capacity, schedule, prereqs), discussion_topics(topics) {}

void SeminarCourse::view_schedule() const {
    Course::view_schedule();
    cout << "Discussion Topics:" << endl;
    for (const auto& topic : discussion_topics) {
        cout << "- " << topic << endl;
    }
}

// Implementation of LabCourse member functions
LabCourse::LabCourse(string code, string title, string instructor, int capacity, vector<string> schedule, string location, string equip, vector<string> prereqs) :
    Course(code, title, instructor, capacity, schedule, prereqs), lab_location(location), equipment(equip) {}

void LabCourse::view_schedule() const {
    Course::view_schedule();
    cout << "Lab Location: " << lab_location << endl;
    cout << "Equipment: " << equipment << endl;
}

// Implementation of Student member functions
Student::Student() {}

Student::Student(string id, string n, bool grad) : student_id(id), name(n), is_graduate(grad) {}

void Student::create_profile() {
    cout << "Enter your basic details:" << endl;
    cout << "Name: ";
    getline(cin, name);

    while (true) {
        cout << "Student ID (10 digits): ";
        getline(cin, student_id);

        if (student_id.length() == 10) {
            break;
        } else {
            cout << "Invalid student ID. Student ID must be 10 digits." << endl;
        }
    }

    cout << "Are you a graduate student? (0 for Undergraduate, 1 for Graduate): ";
    cin >> is_graduate;
    cin.ignore();
    cout << "Profile created successfully!" << endl;
}

string Student::get_student_id() const {
    return student_id;
}

string Student::get_name() const {
    return name;
}

void Student::enroll_course(Course* course) {
    if (course->is_student_enrolled(name)) {
        cout << "You are already enrolled in " << course->get_title() << ". Cannot enroll again." << endl;
    } else {
        if (is_graduate) {
            if (typeid(*course) == typeid(LabCourse) || typeid(*course) == typeid(SeminarCourse) || typeid(*course) == typeid(LectureCourse)) {
                cout << "Have you taken the prerequisite undergraduate course? (yes/no): ";
                string response;
                cin >> response;
                if (response == "yes") {
                    enrolled_courses.push_back(course);
                    course->enroll_student(name);
                } else {
                    cout << "Please take the prerequisites first." << endl;
                }
            } else {
                cout << "Graduate students cannot enroll in undergraduate courses." << endl;
            }
        } else { // For undergraduate students
            if (typeid(*course) == typeid(LabCourse) || typeid(*course) == typeid(SeminarCourse) || typeid(*course) == typeid(LectureCourse)) {
                enrolled_courses.push_back(course);
                course->enroll_student(name);
            } else {
                cout << "Undergraduate students cannot enroll in graduate courses." << endl;
            }
        }
    }
}

void Student::get_enrolled_courses() {
    cout << name << "'s enrolled courses:" << endl;
    for (const auto& course : enrolled_courses) {
        cout << "- " << course->get_title() << endl;
    }
}

void Student::browse_courses(const vector<Course*>& courses) {
    cout << "Available courses:" << endl;
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << i + 1 << ". " << courses[i]->get_title() << " (Available Seats: " << courses[i]->check_available_seats() << ")" << endl;
    }
}

void Student::check_prerequisites(const vector<Course*>& courses) {
    cout << "Enter the number corresponding to the course to check prerequisites (or enter 0 to exit): ";
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= courses.size()) {
        Course* selected_course = courses[choice - 1];
        const auto& prereqs = selected_course->get_prerequisites();

        if (prereqs.empty()) {
            cout << "There are no prerequisites for " << selected_course->get_title() << "." << endl;
        } else {
            cout << "Prerequisites for " << selected_course->get_title() << ": " << endl;
            for (const auto& skill : prereqs) {
                cout << "- " << skill << endl;
            }
        }
    } else if (choice != 0) {
        cout << "Invalid choice. Please try again." << endl;
    }
}

void Student::view_enrolled_courses_schedule() const {
    if (enrolled_courses.empty()) {
        cout << "Your schedule is empty. Please enroll in courses first." << endl;
    } else {
        for (const auto& course : enrolled_courses) {
            course->view_schedule();
            cout << endl;
        }
    }
}

// Implementation of StudentManagement member functions
void StudentManagement::add_student(const Student& student) {
    students.push_back(student);
    cout << "Student profile for " << student.get_name() << " added successfully." << endl;
}

bool StudentManagement::delete_student(const string& student_id) {
    auto it = find_if(students.begin(), students.end(), [&](const Student& student) {
        return student.get_student_id() == student_id;
    });

    if (it != students.end()) {
        students.erase(it);
        cout << "Student profile with ID " << student_id << " deleted successfully." << endl;
        return true;
    } else {
        cout << "Student profile with ID " << student_id << " not found." << endl;
        return false;
    }
}

void StudentManagement::list_students() const {
    if (students.empty()) {
        cout << "No student profiles available." << endl;
        return;
    }
    cout << "List of student profiles:" << endl;
    for (const auto& student : students) {
        cout << "Student ID: " << student.get_student_id() << ", Name: " << student.get_name() << endl;
    }
}

Student* StudentManagement::find_student(const string& student_id) {
    for (auto& student : students) {
        if (student.get_student_id() == student_id) {
            return &student;
        }
    }
    return nullptr;
}

int main() {
    // Course setup
    // Course setup for undergraduate students
    vector<string> cs101Prereqs = {"CS201 or equivalent experience with algorithms and data structures"};
    vector<string> cs201Prereqs = {"CS101 or equivalent experience with Python programming"};
    vector<string> cs301Prereqs = {"Basic computer knowledge"};

    // Undergraduate courses (100 level)
    LectureCourse course1("CIS101", "Design and Analysis of Algorithms (Undergraduate)", "Dr. Christian", 50, {"Mon/Wed 10:00-11:30"}, "Room A", cs101Prereqs);
    SeminarCourse course2("CIS102", "Machine Learning (Undergraduate)", "Prof. Tom", 30, {"Tue/Thu 13:00-14:30"}, {"Regression", "Classification"}, cs201Prereqs);
    LabCourse course3("CIS103", "Operating Systems Lab (Undergraduate)", "Dr. Robert", 20, {"Fri 09:00-12:00"}, "Lab 1", "Linux Systems", cs301Prereqs);

    // Graduate courses (200 level)
    vector<string> grad_cs101Prereqs = {"Undergraduate Design and Analysis of Algorithms course or equivalent experience"};
    vector<string> grad_cs201Prereqs = {"Undergraduate Machine Learning course or equivalent experience"};
    vector<string> grad_cs301Prereqs = {"Undergraduate Operating Systems Lab course or equivalent experience"};

    LectureCourse grad_course1("CIS201", "Advanced Algorithms (Graduate)", "Dr. Smith", 40, {"Tue/Thu 10:00-11:30"}, "Room B", grad_cs101Prereqs);
    SeminarCourse grad_course2("CIS202", "Deep Learning (Graduate)", "Dr. Johnson", 25, {"Mon/Wed 13:00-14:30"}, {"Neural Networks", "Recurrent Neural Networks"}, grad_cs201Prereqs);
    LabCourse grad_course3("CIS203", "Distributed Systems Lab (Graduate)", "Dr. Williams", 15, {"Thu 14:00-17:00"}, "Lab 2", "Cloud Platforms", grad_cs301Prereqs);

    vector<Course*> available_courses = {&course1, &course2, &course3, &grad_course1, &grad_course2, &grad_course3};

    // Student management instance creation
    StudentManagement studentManager;

    int option;
    do {
        cout << "\nMenu:\n"
             << "1. Create and add student profile\n"
             << "2. Delete student profile\n"
             << "3. List all student profiles\n"
             << "4. Browse available courses\n"
             << "5. Check course prerequisites\n"
             << "6. Enroll in a course\n"
             << "7. View enrolled courses\n"
             << "8. View schedule for enrolled courses\n"
             << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;
        cin.ignore(); // To consume the newline character left in the input stream

        switch (option) {
            case 1: {
                string name, student_id;
                bool is_graduate;
                cout << "Enter student name: ";
                getline(cin, name);
                cout << "Enter student ID (10 digits): ";
                getline(cin, student_id);
                cout << "Are you a graduate student? (0 for Undergraduate, 1 for Graduate): ";
                cin >> is_graduate;
                cin.ignore(); // Clear input buffer

                Student newStudent(student_id, name, is_graduate);
                studentManager.add_student(newStudent);
                break;
            }
            case 2: {
                string student_id;
                cout << "Enter student ID to delete: ";
                getline(cin, student_id);
                studentManager.delete_student(student_id);
                break;
            }
            case 3: {
                studentManager.list_students();
                break;
            }
            case 4: {
                string student_id;
                cout << "Enter your student ID: ";
                getline(cin, student_id);
                Student* student = studentManager.find_student(student_id);
                if (student != nullptr) {
                    student->browse_courses(available_courses);
                } else {
                    cout << "Student ID not found." << endl;
                }
                break;
            }
            case 5: {
                cout << "Available courses for prerequisites check:" << endl;
                for (size_t i = 0; i < available_courses.size(); ++i) {
                    cout << i + 1 << ". " << available_courses[i]->get_title() << endl;
                }

                int courseChoice;
                cout << "Enter the number of the course to check its prerequisites: ";
                cin >> courseChoice;
                if (courseChoice > 0 && courseChoice <= available_courses.size()) {
                    Course* selected_course = available_courses[courseChoice - 1];
                    const auto& prereqs = selected_course->get_prerequisites();

                    if (prereqs.empty()) {
                        cout << "There are no prerequisites for " << selected_course->get_title() << "." << endl;
                    } else {
                        cout << "Prerequisites for " << selected_course->get_title() << ": " << endl;
                        for (const auto& prereq : prereqs) {
                            cout << "- " << prereq << endl;
                        }
                    }
                } else {
                    cout << "Invalid choice." << endl;
                }
                cin.ignore(); // Clear input buffer
                break;
            }
            case 6: {
                string student_id;
                cout << "Enter your student ID: ";
                getline(cin, student_id);
                Student* student = studentManager.find_student(student_id);
                if (student != nullptr) {
                    student->browse_courses(available_courses);

                    int courseChoice;
                    cout << "Enter the number of the course you want to enroll in: ";
                    cin >> courseChoice;
                    if (courseChoice > 0 && courseChoice <= available_courses.size()) {
                        student->enroll_course(available_courses[courseChoice - 1]);
                    } else {
                        cout << "Invalid choice." << endl;
                    }
                } else {
                    cout << "Student ID not found." << endl;
                }
                break;
            }
            case 7: {
                string student_id;
                cout << "Enter your student ID: ";
                getline(cin, student_id);
                Student* student = studentManager.find_student(student_id);
                if (student != nullptr) {
                    student->get_enrolled_courses();
                } else {
                    cout << "Student ID not found." << endl;
                }
                break;
            }
            case 8: {
                string student_id;
                cout << "Enter your student ID: ";
                getline(cin, student_id);
                Student* student = studentManager.find_student(student_id);
                if (student != nullptr) {
                    student->view_enrolled_courses_schedule();
                } else {
                    cout << "Student ID not found." << endl;
                }
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (option != 0);

    return 0;
}