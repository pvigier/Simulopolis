 #pragma once

 #include <vector>
 #include <string>

class Person;

class Company
{
public:
    Company(std::string name);

    const std::string& getName() const;
    Person* getManager() const;
    void setManager(Person* manager);
    const std::vector<Person*>& getEmployees() const;
    void addEmployee(Person* employee);

private:
    std::string mName;
    Person* mManager;
    std::vector<Person*> mEmployees;
};
