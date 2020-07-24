#include "journal.h"
using namespace std;

std::string mdel = ", ";
bool ui = false;

int main(int argc, char** argv) {
    Journal jr; 
    if (argc > 1) {
        fstream ifs(argv[1], fstream::in);
        jr.unpack(ifs);
        ifs.close();
    }
    ui = true;
    help_message();
    while (ui) {
        interact(jr);
    } 
    if (argc > 1) {
        fstream ofs(argv[1], fstream::out);
        jr.pack(ofs);
        ofs.close();
    }
}

void help_message() {
    cout << "enter command number, then <ENTER>, then appropriate arguments" << endl;
    cout << "commands:" << endl;
    cout << "1. show journal" << endl;
    cout << "2. add student" << endl;
    cout << "3. add group" << endl;
    cout << "4. add exam" << endl;
    cout << "5. exit" << endl;
}

void interact(Journal& jr) {
    char c;
    cin >> c;    
    bool ok = true;
    switch(c) {
        case '1': {
            jr.pack(cout);
            break;
        }
        case '2': {
            cout << "enter group name:" << endl; 
            string group_name;
            cin >> group_name;
            Student st;
            cout << "enter student information:" << endl;
            for (auto& e : st.info) {
                cout << e;
                cin >> e;
            }
            jr.addStudent(group_name, st);
            break;
        }
        case '3': {
            cout << "enter group name:" << endl; 
            string group_name;
            cin >> group_name;
            jr.addGroup(group_name);
            break;
        }
        case '4': {
            cout << "enter group name:" << endl; 
            string group_name;
            cin >> group_name;
            cout << "enter exam name:" << endl; 
            string exam_name;
            cin >> exam_name;
            jr.makeExam(group_name, exam_name);
            break;
        }
        case '5': {
            ui = false;
            break;
        }
        default: {
            cerr << "non-existing command" << endl;
            ok = false;
        }
    }
    if (ok) cout << "success" << endl;
}

Journal::Journal() {

}

void Journal::pack(std::ostream& os) {
    if (!ui) os << data.size() << endl;
    for (auto& e : data) {
        if (ui) os << "group name: ";
        os << e.first << endl;
        if (!ui) os << e.second.size() << endl;
        else os << "students: " << endl;
        for (auto& v : e.second) {
            os << v; 
        }
    }
}

void Journal::unpack(std::istream& is) {
    size_t n;
    is >> n;
    for (size_t i{}; i < n; ++i) {
        string group_name;
        size_t st_count;
        is >> group_name >> st_count;
        addGroup(group_name);
        for (size_t j{}; j < st_count; ++j) {
            Student st;
            is >> st;
            addStudent(group_name, st);
        }
    }
}

void Journal::addGroup(std::string group_name) {
    data[group_name] = vector<Student>{}; 
}

void Journal::addStudent(std::string group_name,Student st) {
    if (data.find(group_name) == data.end()) {
        cerr << "No such group" << endl;
        return;
    }
    data[group_name].push_back(st);
}

void Journal::makeExam(std::string group_name, std::string exam_name) {
    if (data.find(group_name) == data.end()) {
        cerr << "No such group" << endl;
        return;
    }
    /* if (data[group_name].size() != results.size()) {
        cerr << "Not all students" << endl;
        return;
    }*/
    auto& v = data[group_name];
    for (size_t i{}; i < v.size(); ++i) {
        cout << "result for: ";
        for (auto& e : v[i].info) cout << e;
        cout << endl << "enter result: ";
        string result;
        cin >> result;
        v[i].exams.push_back(exam_name + ": " + result);
    }
}

Student::Student(){ 
    if (ui) {
        info.resize(2);
        info[0] = "name:";
        info[1] = "second name:";
    }
}

Student::Student(vector<string> info) : info{info} {

}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    if (!ui) os << s.delimiter;
    else {
        os << "information: ";
    }
    bool first = true;
    for (auto& e : s.info) {
        if (ui) {
            if (first) {
                os << e;
                first = false;
            }
            else os << mdel << e;
        }
        else os << e << mdel;
    } 
    if (!ui) os << s.delimiter;
    else os << ". ";
    if (ui) {
        os << "exams: ";
    }
    first = true;
    for (auto& e : s.exams) {
        if (ui) {
            if (first) {
                os << e;
                first = false;
            }
            else os << mdel << e;
        }
        else os << e << mdel;
    }
    os << s.delimiter;
    return os;
}

std::istream& operator>>(std::istream& is, Student& s) {
    cout << "getting student.." << endl;
    char c;
    is.get(c);
    while (c == s.delimiter) {
        is.get(c);
        if (!is) {
            throw runtime_error("corrupted file");
        }
    }
    while (c != s.delimiter) {
        if (!is) {
            throw runtime_error("corrupted file");
        }
        string buf;
        bool ok = true;
        do  {
            buf.push_back(c);
            if (buf.size() >= mdel.size()) {
                ok = false;
                size_t diff = buf.size() - mdel.size();
                for (size_t i{}; i < mdel.size(); ++i) {
                    if (buf[i+diff] != mdel[i]) {
                        ok = true;
                        break;
                    }
                } 
            }
            is.get(c);
        } while (ok);
        s.info.push_back(string(buf.begin(), buf.end() - mdel.size()));
    }
    while (c == s.delimiter) {
        is.get(c);
        if (!is) {
            throw runtime_error("corrupted file");
        }
    }
    while (c != s.delimiter) {
        string buf;
        bool ok = true;
        do  {
            if (!is) {
                throw runtime_error("corrupted file");
            }
            buf.push_back(c);
            if (buf.size() >= mdel.size()) {
                ok = false;
                size_t diff = buf.size() - mdel.size();
                for (size_t i{}; i < mdel.size(); ++i) {
                    if (buf[i+diff] != mdel[i]) {
                        ok = true;
                        break;
                    }
                } 
            }
            is.get(c);
        } while (ok);
        s.exams.push_back(string(buf.begin(), buf.end() - mdel.size()));
        if (!is) break;
    }
    return is;
}
