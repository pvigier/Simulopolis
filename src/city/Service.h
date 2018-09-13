#pragma once

#include <set>
#include "city/Building.h"
#include "city/Work.h"

class Service : public Building
{
public:
    Service(const std::string& name, Type type, unsigned int nbStairs, std::size_t nbEmployees,
        Work::Type employeeType);
    virtual ~Service();

    virtual std::unique_ptr<Tile> clone() const override;
    virtual void update() override;
    virtual void tearDown() override;
    virtual void setOwner(Company* owner) override;

    std::vector<Work>& getEmployees();
    const std::vector<Work>& getEmployees() const;

protected:
    std::vector<Work> mEmployees;
    std::set<Id> mWorksInMarket;

private:
    // Serialization
    friend class boost::serialization::access;

    Service() = default;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Building>(*this);
        ar & mEmployees & mWorksInMarket;
    }
};
