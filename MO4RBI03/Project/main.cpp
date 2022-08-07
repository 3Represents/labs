/**
 * MU4RBI02 C++ avancé - Projet
 * 
 * @file main.cpp
 * @author Jiheng WEI
 */

#include "Navigator.hpp"
#include "Grade.hpp"

using namespace travel;

int main(int argc, char** argv) {
    if (argc == 1) {
        /* Test using Grade */
        Navigator vianavigo("./data/s.csv", "./data/c.csv");
        Grade grd(false);
        grd.dijkstra(vianavigo, false);
    } else if (argc == 3) {
        /* Specify stations */
        Navigator vianavigo("./data/s.csv", "./data/c.csv");
        uint64_t _start, _end;

        try {
            // Using IDs
            _start = std::stoull(argv[1]);
            _end = std::stoull(argv[2]);
            vianavigo.compute_and_display_travel(_start, _end);
        } catch (std::invalid_argument) {
            // Using names
            vianavigo.compute_and_display_travel(argv[1], argv[2]);
        }
    } else if (argc == 5) {
        /* Specify files and stations */
        Navigator vianavigo(argv[1], argv[2]);
        uint64_t _start, _end;

        try {
            // Using IDs
            _start = std::stoull(argv[3]);
            _end = std::stoull(argv[4]);
            vianavigo.compute_and_display_travel(_start, _end);
        } catch (std::invalid_argument) {
            // Using names
            vianavigo.compute_and_display_travel(argv[3], argv[4]);
        }
    } else {
        std::cerr << "Wrong number of arguments" << std::endl;
    }

    return 0;
}