#include <string>

#include "format.h"

using std::string;

const int DAY_SECONDS = 24*60*60;
const int SECONDS_OF_A_HOUR = 60*60;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    std::string timeString = "";
    long remaining_seconds = seconds;

    int hours = remaining_seconds / SECONDS_OF_A_HOUR;
    if (hours < 10)
        timeString += "0" + std::to_string(hours) + ":";
    else
        timeString += std::to_string(hours) + ":";

    remaining_seconds = remaining_seconds % SECONDS_OF_A_HOUR;
    int minutes = remaining_seconds / 60;
    if (minutes < 10)
        timeString += "0" + std::to_string(minutes) + ":";
    else
        timeString += std::to_string(minutes) + ":";

    remaining_seconds = remaining_seconds % 60;
    if (remaining_seconds < 10)
        timeString += "0" + std::to_string(remaining_seconds);
    else
        timeString += std::to_string(remaining_seconds);
    return timeString;   
}