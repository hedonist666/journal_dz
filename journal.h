#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <map>
#include <exception>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */

struct Student {
    static const char delimiter = '\n';
    std::vector<std::string> info; //name, second_name;
    std::vector<std::string> exams;
    friend std::ostream& operator<<(std::ostream&, const Student& s);
    friend std::istream& operator>>(std::istream&, Student& s);
    Student();
    Student(std::vector<std::string>);
};


struct Journal {
    std::map<std::string, std::vector<Student>> data;
    Journal();
    void pack(std::ostream&);
    void unpack(std::istream&); 
    void addGroup(std::string);
    void addStudent(std::string,Student);
    //void makeExam(std::string, std::string, std::vector<std::string>);
    void makeExam(std::string, std::string);
};

void help_message();
void interact(Journal&);
