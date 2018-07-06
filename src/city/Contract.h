#pragma once

class Person;

struct Contract
{
    enum class Job{WORKER, SELLER, MANAGER, DOCTOR, TEACHER, POLICEMAN};

    const Person* person;
    Job job;
    float salary;
};
