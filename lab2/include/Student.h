#pragma once
#include <string>

class Student {
public:
    std::string name;
    int age;
    int group;

    Student() = default;

    Student(std::string name, int age, int group)
            : name(name), age(age), group(group) {}
};