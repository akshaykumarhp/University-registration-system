#ifndef COURSE_STUDENT_MANAGEMENT_H
#define COURSE_STUDENT_MANAGEMENT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Course {
protected:
    string course_code;
    string title;
    string instructor;
    int max_capacity;
    vector<string> schedule;
    vector<string> enrolled_students;
    vector<string> prerequisites;

public:
    Course(string code, string title, string instructor, int capacity, vector<string> schedule, vector<string> prereqs);
    string get_title() const;
    const vector<string>& get_prerequisites() const;
    virtual void enroll_student(const string& studentName);
    int check_available_seats();
    void generate_class_schedule();
    bool is_student_enrolled(const string& studentName) const;
    virtual void view_schedule() const;
};

class LectureCourse : public Course {
private:
    string lecture_hall;

public:
    LectureCourse(string code, string title, string instructor, int capacity, vector<string> schedule, string hall, vector<string> prereqs);
    void view_schedule() const override;
};

class SeminarCourse : public Course {
private:
    vector<string> discussion_topics;

public:
    SeminarCourse(string code, string title, string instructor, int capacity, vector<string> schedule, vector<string> topics, vector<string> prereqs);
    void view_schedule() const override;
};

class LabCourse : public Course {
private:
    string lab_location;
    string equipment;

public:
    LabCourse(string code, string title, string instructor, int capacity, vector<string> schedule, string location, string equip, vector<string> prereqs);
    void view_schedule() const override;
};

class Student {
private:
    string student_id;
    string name;
    bool is_graduate;
    vector<Course*> enrolled_courses;

public:
    Student();
    Student(string id, string n, bool grad);
    void create_profile();
    string get_student_id() const;
    string get_name() const;
    void enroll_course(Course* course);
    void get_enrolled_courses();
    void browse_courses(const vector<Course*>& courses);
    void check_prerequisites(const vector<Course*>& courses);
    void view_enrolled_courses_schedule() const;
};

class StudentManagement {
private:
    vector<Student> students;

public:
    void add_student(const Student& student);
    bool delete_student(const string& student_id);
    void list_students() const;
    Student* find_student(const string& student_id);
};

#endif