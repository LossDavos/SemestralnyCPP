#include <sstream>
#include <bits/stdc++.h>
#include "Network.h"
using namespace std;

std::vector<std::tuple<int, int, int>> PTNetwork::whereNext(BusStop &from) {
    /*
     * returns a vector of tuples of adjacent stops(number), corresponding distance(mins) and connecting line(number) if stop in network (id and name)
     */
    if(busstops.find(from.getStopNumber()) == busstops.end() || busstops.find(from.getStopNumber())->second.getName() != from.getName()){
        throw Exception("Given stop " + from.getName() + " not in network");
    }
    BusLine *curr;
    std::vector<std::tuple<int, int, int>> result;
    for(int i:from.getCrossingLines()){
        curr = &buslines[i];
        if(curr->getStopVector().size() <= 1 || !buslines[i].isLineInOrder()){
            continue;
        }
        std::vector<std::pair<BusStop *, int>> stops = curr->getStopVector();
        for(auto it = stops.begin(); it != stops.end(); ++it){
            if(it->first == &from){
                if(curr->getDirection() == 0){
                    if(it->first != curr->getFirstStop()){ //not in the beggining
                        auto prev = it -1;
                        result.emplace_back((prev)->first->getStopNumber(), it->second,curr->getLineNum());
                    }
                }
                if (it->first != curr->getLastStop()) {
                    auto next = it + 1;
                    result.emplace_back((next)->first->getStopNumber(), next->second,curr->getLineNum());
                }
                break;
            }

        }
    }
    return result;
}

std::map<int, std::vector<std::tuple<int, int, int>>> PTNetwork::createAdjMap(){
    /*
     * returns a map of adjacencies- key is stop number and value is tuple of adjacent bus stop(number), distance (mins) and connecting busline(number)
     */
    std::map<int, std::vector<std::tuple<int, int,int>>> result;
    for(auto &i:busstops){
        result[i.first] = whereNext(i.second);
    }
    return result;
}


std::vector<pair<int,int>> PTNetwork::findShortestPath(BusStop &start, BusStop &dest){
    /*
     * finds shortest path using Dijkstra's algorithm
     */
    std::map<int,std::vector<std::tuple<int,int,int>>> adjmap = createAdjMap();
    int inf = 99999;
    std::map<int, int> distances;
    for(auto &i:busstops){
        distances[i.first] = inf;
    }
    distances[start.getStopNumber()] = 0;
    priority_queue<tuple<int, int>> pq;
    pq.push({0, start.getStopNumber()});
    std::map<int,std::pair<int,int>> previous;
    while(!pq.empty()){
        tuple<int,int> dist_node_line = pq.top();
        pq.pop();
        if(get<0>(dist_node_line) > distances[get<1>(dist_node_line)] ){
            continue;
        }
        for(tuple<int,int,int> node_dist_line: adjmap[get<1>(dist_node_line)]){
            int distance = get<0>(dist_node_line) + std::get<1>(node_dist_line);
            if(distance < distances[std::get<0>(node_dist_line)]){
                distances[std::get<0>(node_dist_line)] = distance;
                previous[std::get<0>(node_dist_line)] = {get<1>(dist_node_line), std::get<2>(node_dist_line)};
                pq.push({distance, std::get<0>(node_dist_line)});
            }
        }
    }
    std::vector<std::pair<int,int>> path;
    if(previous.empty() || previous.find(dest.getStopNumber()) == previous.end() ){
        return path;
    }
    path.emplace_back(dest.getStopNumber(),0);
    while((path.end()-1)->first != start.getStopNumber()){
        path.push_back(previous[(path.end()-1)->first]);
    }
    reverse(path.begin(), path.end());
    return path;
}


//std::vector<pair<int,int>> PTNetwork::findShortestPath(BusStop &start, BusStop &dest, Time when){
//    /*
//     * finds shortest path using Dijkstra's algorithm
//     */
//    std::map<int,std::vector<std::tuple<int,int,int>>> adjmap = createAdjMap();
//    int inf = 600;
//    std::map<int, int> distances;
//    for(auto &i:busstops){
//        distances[i.first] = inf;
//    }
//    distances[start.getStopNumber()] = 0;
//    priority_queue<tuple<int, int,int>> pq;
//    pq.push({0, start.getStopNumber(),0});
//    std::map<int,std::pair<int,int>> previous;
//    while(!pq.empty()){
//        tuple<int,int,int> dist_node_line = pq.top();
//        pq.pop();
//        if(get<0>(dist_node_line) > distances[get<1>(dist_node_line)]){
//            continue;
//        }
//        for(tuple<int,int,int> node_dist_line: adjmap[get<1>(dist_node_line)]){
//            Time time_dist = (buslines[std::get<2>(node_dist_line)].getEarliestFromStop(busstops[get<1>(dist_node_line)], busstops[get<0>(node_dist_line)], when).end()-1)->second;
////            when = time_dist;
//            int distance = get<0>(dist_node_line) + time_dist.getTimePair().first*60 + time_dist.getTimePair().second;
//            if(distance < distances[std::get<0>(node_dist_line)]){
//                distances[std::get<0>(node_dist_line)] = distance;
//                previous[std::get<0>(node_dist_line)] = {get<1>(dist_node_line), std::get<2>(node_dist_line)};
//                pq.push({distance, std::get<0>(node_dist_line), get<2>(node_dist_line)});
//            }
//        }
//    }
//    std::vector<std::pair<int,int>> path;
//    if(previous.empty()){
//        return path;
//    }
//    path.emplace_back(dest.getStopNumber(),0);
//    while((path.end()-1)->first != start.getStopNumber()){
//        path.push_back(previous[(path.end()-1)->first]);
//    }
//    reverse(path.begin(), path.end());
//    return path;
//}

std::string getStringType(Type type){
    switch (type) {
        case BUS:
            return "BUS";
        case TRAM:
            return "TRAM";
        case METRO:
            return "METRO";
        default:
            return "";
    }
}

std::string PTNetwork::getRoute(BusStop &start, BusStop &end, Time when, bool weekend) {
    std::string result;
    vector<pair<int,int>> path = findShortestPath(start, end);
    if(path.empty()){
        return "No path from " + start.getName() + " to " + end.getName();
    }
    int curline = path.begin()->second;
    int curstart = path.begin()->first;
    int curend;
    result += to_string(curline) + "(" + getStringType(buslines[curline].getLineType()) + ")" +  ": ";
    Time curtime = when;
    Time endTime, startTime;
    startTime = buslines[curline].getEarliestFromStop(busstops[curstart], busstops[(path.begin()+1)->first], curtime, weekend).begin()->second;
    for(auto &stop:path){
        curend = stop.first;
        if(stop.second != curline){
            result += buslines[curline].getEarliestFromStopString(busstops[curstart], busstops[curend], curtime,weekend);
            if(stop.second != 0) {
                result += " /-prestup- " + to_string(stop.second) + "(" + getStringType(buslines[stop.second].getLineType()) + ")" + " -/ ";
            }
            curtime = (buslines[curline].getEarliestFromStop(busstops[curstart], busstops[curend], curtime, weekend).end()-1)->second;
            curline = stop.second;
            curstart = stop.first;
        }
    }
    return "(time length: " + (curtime - startTime).getTime() + ") " + result;
}