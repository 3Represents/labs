/**
 * MU4RBI02 C++ avancé - Projet
 * 
 * @file Navigator.hpp
 * @author Jiheng WEI
 */

#pragma once
#include "Generic_mapper.hpp"

using namespace travel;

class Navigator : public Generic_mapper {
protected:
    virtual void read_stations(const std::string& _filename) override;
    virtual void read_connections(const std::string& _filename) override;

    std::string& trim(std::string& str);
    uint64_t hamming(const std::string& str1, const std::string& str2);
    std::pair<uint64_t, uint64_t> compute_id(const std::string& _start, const std::string& _end);
public:
    Navigator(const std::string& sfile, const std::string& cfile);

    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;

    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string&, const std::string&) override;
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string&, const std::string&) override;

    Navigator& operator=(const Navigator&) = delete;
};