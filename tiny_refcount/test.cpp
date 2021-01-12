#include "refcount.h"
#include <string>


class Person : public refcount {
public:
    Person() {}
    Person(std::string name, int age) : _name(name), _age(age) {}
    ~Person() {}
    std::string _name;
    int _age;
};

class Student : public Person {
public:
    Student() {}
    ~Student() {}
    Student(std::string name, int age, std::string school) : 
        Person(name, age), _school(school) {}
    std::string _school;
};

int main() {
    Person park = Person("park", 12);
    Student jack_stu = Student("jack", 14, "pku");
    smartptr<Student> student_ptr = smartptr<Student>(new Student("jack", 14, "pku"));
    smartptr<Person> person_ptr = smartptr<Person>(new Person("park", 12));
    Person* row_person_ptr = person_ptr;
    //smartptr<Person> person_ptr2 = student_ptr;
    //smartptr<Student> student_ptr2 = person_ptr;
    return 0;
}