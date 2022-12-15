//
// Created by david on 14/12/22.
//
#include <iostream>
#include <vector>

#pragma once
const unsigned int min_interval_mins  = 5;
const unsigned int max_interval_mins = 240;

const std::string WrongBusStopName = "Wrong bus stop name";
const std::string WrongBusStopNumber = "Wrong bus stop number";
const std::string WrongLineNum = "Incorrect line number";
const std::string WrongBusLineNumber = "Wrong bus line number";
const std::string WrongDirection = "Unknown direction - use values -1,0,1 only!";
const std::string OutsideWeekendInterval = "Interval for weekends outside range <" + std::to_string(min_interval_mins)+ "," +
        std::to_string(max_interval_mins) + ">!";
const std::string OutsideWorkdayInterval = "Interval for workdays outside range <" + std::to_string(min_interval_mins)+ "," +
                                          std::to_string(max_interval_mins) + ">!";

class Exception : public std::exception {
    std::string mess;
public:
    explicit Exception(std::string message) : mess(std::move(message)) {}
    std::string message(){
        return mess;
    }
};


class BusStop{
    int number = -1;
    std::string name;
    int *lines = new int;
    int number_of_lines = 0;
public:
    BusStop(const std::string& stop_name, int num);
    bool operator == (BusStop const &b1){
        if(b1.getName() == name && b1.getStopNumber() == number){
            return true;
        }
        return false;
    };
    bool addLine(int line_num);
    bool removeLine(int line_num);
    bool changeName(std::string new_name);
    bool changeNumLine(int new_num);
    int numberOfLines() const {return number_of_lines;};
    std::string getName() const { return name; };
    int getStopNumber() const {return number; };
    std::string getBSlines() const;
};

class BusLine{
    int direction = 0; //which stop is a starting point (-1 last one to first one, 1 => first to last in stops list, 0 => both ways)
    std::vector<BusStop> stops;
    int interval_workdays = -1;
    int interval_weekends = -1;
    int line_num = -1;
    bool status = false;

public:
    BusLine(int number, int direct=1);
    BusLine(int number,  int workdays_interval, int weekends_interval,int direct = 1);
    bool changeDirection(int new_direct);
    bool isLineInOrder() const {return status;};
    std::string getStopsString() const;
    std::vector<BusStop> getStopVector() const { return stops;};
    bool changeStatus(); //cannot change if some of the variables is negative or not initialized except for stops
    bool setIntervalWorkdays(int work_int);
    bool setIntervalWeekends(int wknd_int);
    std::string getTimetable() const;
    void timetableToFile(std::string file) const;
    bool changeLineNum(int new_line_num) ;
    int getLineNum() const {return line_num;};
    int getIntervalWorkdays() const {return interval_workdays;};
    int getIntervalWeekends() const{return interval_weekends;};
    bool addStop(BusStop &new_stop) ;
    bool removeStop(BusStop &stop_rem) ;
};