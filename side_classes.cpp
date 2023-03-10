//
// Created by david on 18/12/22.
//
#include "side_classes.h"
#include <iomanip>
#include <sstream>

bool isTimeOK(int hours,int minutes){
    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
        return false;
    }
    return true;
}
Time::Time(int hours0, int minutes0) {
    setTime(hours0, minutes0);
}

Time::Time(std::string& time){
    int h, m;
    h = stoi(time.substr(0, time.find(':')));
    m = stoi(time.substr(time.find(':') + 1, time.size() -1));
    if(isTimeOK(h,m)){
        hours = h;
        minutes = m;
    }
}

bool Time::setTime(int hours0, int minutes0){
    if (!isTimeOK(hours0, minutes0)) {
        hours = 0;
        minutes = 0;
        return false;
    } else {
        hours = hours0;
        minutes = minutes0;
        return true;
    }
}
std::string Time::getTime() const {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":" <<std::setw(2)<< minutes;
    return ss.str();
}

Time Time::operator+(const Time &other) {
    Time newTime;
    if(minutes + other.minutes >= 60) {
        newTime.hours += 1;
    }
    newTime.minutes = (minutes + other.minutes)%60;
    newTime.hours += (hours + other.hours)%24;
    return newTime;
}

Time Time::operator-(const Time &other) {
    Time newTime;
    if(minutes - other.minutes < 0) {
        newTime.hours -= 1;
    }
    newTime.minutes = (minutes - other.minutes)%60;
    newTime.hours = (hours - other.hours)%24;
    if(newTime.minutes < 0){
        newTime.hours -= 1;
        newTime.minutes = 60 + newTime.minutes;
    }
    if(newTime.hours < 0){
        newTime.hours = 24 + newTime.hours;
    }
    return newTime;
}
Time Time::operator+=(const Time &other) {
    if(minutes + other.minutes >= 60) {
        hours += 1;
    }
    minutes = (minutes + other.minutes)%60;
    hours = (hours + other.hours)%24;
}

Time Time::operator-=(const Time &other) {
    if(minutes - other.minutes < 0) {
        hours -= 1;
    }
    minutes = (minutes - other.minutes)%60;
    hours = (hours - other.hours)%24;
    if(minutes < 0){
        minutes = 60 + minutes;
    }
    if(hours < 0){
        hours = 24 + hours;
    }
}

bool Time::operator<(const Time &other) {
    if(hours < other.hours){
        return true;
    }if(hours == other.hours && minutes < other.minutes){
        return true;
    }
    return false;
}

bool Time::operator>(const Time &other) {
    if(hours > other.hours){
        return true;
    }if(hours == other.hours && minutes > other.minutes){
        return true;
    }
    return false;
}

bool Time::operator==(const Time &other) {
    return hours == other.hours && minutes == other.minutes;
}

bool Time::operator<=(const Time &other) {
    if(hours < other.hours){
        return true;
    }if(hours == other.hours && (minutes < other.minutes || minutes == other.minutes)){
        return true;
    }
    return false;
}