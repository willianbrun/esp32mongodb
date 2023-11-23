using namespace std;

class Log
{
public:
    float temperature;
    bool turned_alarm_on;

    Log(
        float temperature, bool turned_alarm_on)
    {
        this->temperature = temperature;
        this->turned_alarm_on = turned_alarm_on;
    }
};