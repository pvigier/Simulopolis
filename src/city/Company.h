 #pragma once

 #include <vector>
 #include <string>

class Person;

class Company
{
public:
    Company(std::string name, std::string suffix, int creationYear, Person* owner);

    const std::string& getName() const;
    std::string getFullName() const;
    Person* getOwner() const;
    void setOwner(Person* owner);
    const std::vector<Person*>& getEmployees() const;
    void addEmployee(Person* employee);

private:
    std::string mName;
    std::string mSuffix;
    int mCreationYear;
    Person* mOwner;
    std::vector<Person*> mEmployees;
};
