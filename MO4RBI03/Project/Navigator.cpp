/**
 * MU4RBI02 C++ avancé - Projet
 * 
 * @file Navigator.cpp
 * @author Jiheng WEI
 */

#include "Navigator.hpp"
#include <fstream>
#include <sstream>
#include <limits>
#include <set>
#include <cmath>
#include <algorithm>
#include <codecvt>

using namespace travel;

#define INF std::numeric_limits<uint64_t>::max()
#define DNE INF

Navigator::Navigator(const std::string& sfile, const std::string& cfile) {
    read_stations(sfile);
    read_connections(cfile);
}


/**
 * Remove spaces before/after a string.
 * 
 * @param str string to process
 * @return Trimmed string.
 */
std::string& Navigator::trim(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}


void Navigator::read_stations(const std::string& _filename) {
    std::ifstream fin(_filename);
    if (fin.fail()) {
        std::cerr << "File not found" << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(fin, line); // Omit the header
    
    while (std::getline(fin, line)) {
        std::istringstream sin(line);
        std::vector<std::string> cells;
        std::string cell;

        while (std::getline(sin, cell, ',')) {
            this->trim(cell);
            cells.push_back(cell);
        }

        Station value = {cells[0], cells[2], cells[3], cells[4]};
        this->stations_hashmap[std::stoull(cells[1])] = value;
    }

    fin.close();
}


void Navigator::read_connections(const std::string& _filename) {
    std::ifstream fin(_filename);
    if (fin.fail()) {
        std::cerr << "File not found" << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(fin, line); // Omit the header

    while (std::getline(fin, line)) {
        std::istringstream sin(line);
        std::vector<uint64_t> cells;
        std::string cell;

        while (std::getline(sin, cell, ',')) {
            this->trim(cell);
            cells.push_back(std::stoull(cell));
        }

        this->connections_hashmap[cells[0]][cells[1]] = cells[2];
    }

    fin.close();
}


std::vector<std::pair<uint64_t,uint64_t> > Navigator::compute_travel(uint64_t _start, uint64_t _end) {
    std::vector<std::pair<uint64_t,uint64_t> > best_path;

    if (
        this->stations_hashmap.find(_start) == this->stations_hashmap.end() ||
        this->stations_hashmap.find(_end) == this->stations_hashmap.end()
    ) {
        // Station not found
        return best_path;
    }

    /* Dijkstra's algorithm */
    std::unordered_map<uint64_t,uint64_t> cost, prev;
    std::set<uint64_t> Q;

    for (const auto& elem : this->connections_hashmap) {
        const uint64_t& vert(elem.first);
        cost[vert] = INF;
        Q.insert(vert);
    }

    cost[_start] = 0;
    for (const auto& elem : this->connections_hashmap[_start]) {
        const uint64_t& neighbour(elem.first), cost_pair(elem.second);
        cost[neighbour] = cost_pair;
    }

    bool init(true);
    while (!Q.empty()) {
        uint64_t u;
        if (init) {
            u = _start;
            init = false;
        } else {
            uint64_t cost_min(INF);
            for(const uint64_t& i : Q) {
                if (cost[i] < cost_min) {
                    cost_min = cost[i];
                    u = i;
                }
            }
        }

        Q.erase(u);
        if (u == _end) {
            break;
        }

        for (auto const& elem : this->connections_hashmap[u]) {
            const uint64_t& v(elem.first);
            if (Q.find(v) != Q.end()) {
                uint64_t alt(cost[u] + this->connections_hashmap[u][v]);
                if (alt <= cost[v]) {
                    cost[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    /* Reverse iteration */
    uint64_t child(_end), parent;
    best_path.push_back(std::make_pair(_end, cost[_end]));
    while (child != _start) {
        parent = prev[child];
        best_path.insert(best_path.begin(), std::make_pair(parent, cost[parent]));
        child = parent;
    }
    
    return best_path;
}


std::vector<std::pair<uint64_t,uint64_t> > Navigator::compute_and_display_travel(uint64_t _start, uint64_t _end) {
    std::vector<std::pair<uint64_t,uint64_t> > best_path(this->compute_travel(_start, _end));

    std::cout << std::endl;

    if (best_path.empty()) {
        std::cerr << "Station(s) not found." << std::endl << std::endl;
        return best_path;
    }

    const Station& departure(this->stations_hashmap[_start]), arrival(this->stations_hashmap[_end]);

    if (departure.name == arrival.name && departure.line_id == arrival.line_id) {
        const Station& arrival(this->stations_hashmap[_end]);
        std::cout << arrival.name << " (" << arrival.address << ")" << std::endl;
        std::cout << "You have already arrived." << std::endl << std::endl;
        return best_path;
    }
    
    std::cout << "Departure: " << departure.name << " (" << departure.address << ")" << std::endl;

    if (departure.name == arrival.name) {
        // Same station, different lines
        std::cout << "    -> Walk to the platform of M" << arrival.line_id << std::endl;
    } else {
        double cost_prev(0);
        bool line_begin(true);

        for (size_t i(0); i < best_path.size()-1; ++i) {
            const Station& sta_curr(this->stations_hashmap[best_path[i].first]), sta_next(this->stations_hashmap[best_path[i+1].first]);
            
            // Get on a new line
            if (line_begin) {
                if (best_path[i+1].second == best_path[i].second || sta_curr.name == sta_next.name) {
                    // Start with the wrong direction
                    continue;
                } else {
                    // Information of current line
                    std::cout << "    -> M" << sta_curr.line_id << " " << sta_curr.line_name;
                    line_begin = false;
                }
            }

            // Transfer
            if ((sta_curr.name == sta_next.name && sta_curr.line_id != sta_next.line_id) || i == best_path.size()-2) {
                // Time spent on the previous line
                double cost_line(round((double(best_path[i+1].second) - cost_prev) / 60));
                std::cout << " (" << cost_line << " min)" << std::endl;

                // Transfer station (arrival excluded)
                if (
                    i != best_path.size()-2 &&
                    !(i == best_path.size()-3 && sta_curr.name == sta_next.name)
                ) {
                    std::cout << "  " << sta_curr.name << std::endl;
                }

                cost_prev = best_path[i+1].second; // Time from _start to the transfer station
                line_begin = true; // Get on a new line
            }
        }
    }

    std::cout << "Arrival  : " << arrival.name << " (" << arrival.address << ")" << std::endl << std::endl;
    double cost_tot(round(double(best_path.back().second) / 60));
    std::cout << "Total: " << cost_tot << " min" << std::endl << std::endl;

    return best_path;
}


/**
 * Calculate the Hamming distance between two strings.
 * 
 * @param str1 string to compare
 * @param str2 string to compare
 * @return Hamming distance between str1 and str2.
 */
uint64_t Navigator::hamming(const std::string& str1, const std::string& str2) {
    // std::string -> std::wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;
    std::wstring wstr1(converter.from_bytes(str1)), wstr2(converter.from_bytes(str2));

    if (wstr1.size() == wstr2.size()) {
        transform(wstr1.begin(), wstr1.end(), wstr1.begin(), ::towlower);
        transform(wstr2.begin(), wstr2.end(), wstr2.begin(), ::towlower);

        // Hamming distance
        uint64_t dist(0);
        for (size_t i(0); i < wstr1.size(); ++i) {
            dist += (wstr1[i] != wstr2[i]);
        }

        return dist;
    } else {
        // Sizes of the strings not equal
        return DNE;
    }
}


/**
 * Find the IDs of the stations.
 * 
 * @param _start name of the departure station
 * @param _end   name of the arrival station
 * @return Pair {start_id, end_id}.
 */
std::pair<uint64_t, uint64_t> Navigator::compute_id(const std::string& _start, const std::string& _end) {
    uint64_t start_id(DNE), end_id(DNE), dist_start(DNE), dist_end(DNE), res;

    for (const auto& elem : this->stations_hashmap) {
        const uint64_t& id(elem.first);
        const Station& station(elem.second);
        
        // Search for the departure station
        res = this->hamming(station.name, _start);
        if (res < dist_start) {
            dist_start = res;
            start_id = id;
        }
        
        // Search for arrival station
        res = this->hamming(station.name, _end);
        if (res < dist_end) {
            dist_end = res;
            end_id = id;
        }
    };

    // Distance > tolerance
    if (dist_start > 3)
        start_id = DNE;
    if (dist_end > 3)
        end_id = DNE;

    return std::make_pair(start_id, end_id);
}


std::vector<std::pair<uint64_t,uint64_t> > Navigator::compute_travel(const std::string& _start, const std::string& _end) {
    std::pair<uint64_t, uint64_t> station_ids(this->compute_id(_start, _end));
    std::vector<std::pair<uint64_t,uint64_t> > best_path(this->compute_travel(station_ids.first, station_ids.second));
    
    return best_path;
}


std::vector<std::pair<uint64_t,uint64_t> > Navigator::compute_and_display_travel(const std::string& _start, const std::string& _end) {
    std::pair<uint64_t, uint64_t> station_ids(this->compute_id(_start, _end));
    std::vector<std::pair<uint64_t,uint64_t> > best_path(this->compute_and_display_travel(station_ids.first, station_ids.second));
    
    return best_path;
}