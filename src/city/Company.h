 #pragma once

 #include <vector>
 #include <string>

class Person;

class Company
{
public:
    Company(std::string name, int creationYear, Person* owner);

    const std::string& getName() const;
    Person* getOwner() const;
    void setOwner(Person* owner);
    const std::vector<Person*>& getEmployees() const;
    void addEmployee(Person* employee);

private:
    std::string mName;
    int mCreationYear;
    Person* mOwner;
    std::vector<Person*> mEmployees;
};
