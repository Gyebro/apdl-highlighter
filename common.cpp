//
// Created by Gyebro on 2017. 12. 01..
//

#include <c++/algorithm>
#include <c++/bitset>
#include "common.h"

string file_to_string(string filename) {
    std::ifstream in(filename);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

bool anagram_pair_test(string s1, string s2) {
    // Check length
    if (s1.size() == s2.size()) {
        // Sort characters and compare s1 and s2
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        return (s1 == s2);
    } else {
        // Unequal length, s1 and s2 are not anagrams
        return false;
    }
}

string trim_spaces(string value) {
    // Remove leading spaces
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
    // Remove trailing ones
    value.erase(std::find_if(value.rbegin(), value.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), value.end());
    return value;
}

Clock::Clock() {
    start();
}

void Clock::start() {
    t_start = chrono::high_resolution_clock::now();
}

void Clock::stop() {
    t_stop = chrono::high_resolution_clock::now();
}

double Clock::read_sec() {
    return read_millisec()/1000.0;
}

long long int Clock::read_millisec() {
    auto dt = t_stop - t_start;
    return chrono::duration_cast<std::chrono::milliseconds>(dt).count();
}

long long int Clock::read_microsec() {
    auto dt = t_stop - t_start;
    return chrono::duration_cast<std::chrono::microseconds>(dt).count();
}

long long int Clock::read_nanosec() {
    auto dt = t_stop - t_start;
    return chrono::duration_cast<std::chrono::nanoseconds>(dt).count();
}

void Clock::tell_sec() {
    cout << "Computation took: " << read_sec() << " [s]\n";
}

void Clock::tell_millisec() {
    cout << "Computation took: " << read_millisec() << " [ms]\n";
}

void Clock::tell_microsec() {
    cout << "Computation took: " << read_microsec() << " [us]\n";
}

void Clock::tell_nanosec() {
    cout << "Computation took: " << read_nanosec() << " [us]\n";
}

string bytes_to_hex_string(vector<uint8_t> bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (const uint8_t &b : bytes) {
        ss << std::setw(2) << (int)b;
    }
    return ss.str();
}

WinClock::WinClock() {
    QueryPerformanceFrequency(&li);
    PCFreq = double(li.QuadPart)/1000.0;
    start();
}

void WinClock::start() {
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

void WinClock::stop() {
    QueryPerformanceCounter(&li);
    CounterStop = li.QuadPart;
}

double WinClock::read_millisec() {
    return double(CounterStop-CounterStart)/PCFreq;
}

double WinClock::read_microsec() {
    return read_millisec()/1000.0;
}

string hex_string_to_binary(const string s) {
    stringstream ss;
    for (size_t i=0; i<s.size(); i+=2) {
        string hex = s.substr(i,2);
        size_t value = stoul(hex,0,16);
        ss << bitset<8>(value);
    }
    return ss.str();
}


