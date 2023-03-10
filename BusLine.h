//
// Created by david on 18/12/22.
//

#include <iostream>
#include <vector>
#include "BusStop.h"

#pragma once

enum Type{BUS, TRAM, METRO};


class BusLine{
    std::vector<std::pair<BusStop*, int>> stops;
    int interval_workdays = -1;
    int interval_weekends = -1;
    int line_num = -1;
    bool status = false;
    Type type;
    int direction = 0; //which stop is a starting point (-1 last one to first one, 1 => first to last in stops list, 0 => both ways)

public:
    BusLine():line_num(-1), direction(-2){};
    BusLine(int number, enum Type type0, int direct=1);
    BusLine(int number,  enum Type type0, int workdays_interval, int weekends_interval,int direct = 1);
    bool changeDirection(int new_direct);
    int getDirection(){return direction;};
    bool isLineInOrder() const {return status;};
    std::string getStopsString() const;
    std::vector<std::pair<BusStop*,int>> &getStopVector() { return stops;};
    bool changeStatus(); //cannot change if some of the variables is negative or not initialized except for stops
    bool setIntervalWorkdays(int work_int);
    bool setIntervalWeekends(int wknd_int);
    std::string getTimetable() const;
    void timetableToFile(const std::string& file) const;
    bool changeLineNum(int new_line_num) ;
    int getLineNum() const {return line_num;};
    Type getLineType() const {return type;};
    int getIntervalWorkdays() const {return interval_workdays;};
    int getIntervalWeekends() const{return interval_weekends;};
    BusStop* getLastStop() const {return stops.back().first;};

    BusStop* getFirstStop() const {return stops.front().first;};
    bool addStop(int position, BusStop &new_stop, int mins_from_prev, int mins_to_next) ; //inserts a new stop at the postion in vector
    bool removeStop(BusStop &stop_rem, int mins_prev_to_next = -1); //if mins_prev_to_next not sepcified then just uses sum of the neighbouring elements around stop_rem
    std::vector<std::pair<BusStop, Time>> getEarliestFromStop(BusStop &start, BusStop &dest, Time &time, bool weekend=false);//return vector of stops and their scheduled departure
    std::string getEarliestFromStopString(BusStop &start, BusStop &dest, Time &time, bool weekend=false);

};
