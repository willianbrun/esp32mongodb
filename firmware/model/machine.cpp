#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

class Machine
{
public:
    long long int id;
    String name;
    String status;
    double latitude;
    double longitude;
    String last_updated;
    String created;
    Machine(long long int id,
            String name,
            String status,
            double latitude,
            double longitude,
            String last_updated,
            String created)
    {
        this->id = id;
        this->name = name;
        this->status = status;
        this->latitude = latitude;
        this->longitude = longitude;
        this->last_updated = last_updated;
        this->created = created;
    }
};