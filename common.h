//
// Created by Gyebro on 2017. 12. 01..
//

#ifndef AOC17_COMMON_H
#define AOC17_COMMON_H

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <c++/fstream>
#include <c++/sstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "windows.h"

using namespace std;

typedef pair<size_t, size_t> size_p;

string file_to_string(string filename);

template<typename T>
void split(const std::string &s, char delim, T result) {
    stringstream ss(s); string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

vector<string> split(const string &s, char delim);

/**
 * Check whether string s1 and s2 are anagrams (e.g. abcd and cadb are anagrams)
 * @param s1
 * @param s2
 * @return
 */
bool anagram_pair_test(string s1, string s2);

template<typename T>
size_t max_idx(const vector<T> v) {
    auto result = std::max_element(v.begin(), v.end());
    auto index = std::distance(v.begin(), result);
    return index;
}

template<typename T>
size_t min_idx(const vector<T>& v) {
    auto result = std::min_element(v.begin(), v.end());
    auto index = std::distance(v.begin(), result);
    return index;
}

template <class T>
bool has_elem(const vector<T>& v, const T e) {
    return find(v.begin(), v.end(), e) != v.end();
}

template<class T>
long long int find(const vector<T>& v, const T e) {
    return distance(v.begin(), find(v.begin(), v.end(), e));
}

string trim_spaces(const string s);

template <class Clock>
void
display_precision()
{
    typedef std::chrono::duration<double, std::nano> NS;
    NS ns = typename Clock::duration(1);
    std::cout << ns.count() << " ns\n";
}

class Clock {
private:
    chrono::high_resolution_clock::time_point t_start, t_stop;
public:
    Clock();
    void start();
    void stop();
    double read_sec();
    long long int read_millisec();
    long long int read_microsec();
    long long int read_nanosec();
    void tell_sec();
    void tell_millisec();
    void tell_microsec();
    void tell_nanosec();
};

class WinClock {
private:
    double PCFreq;
    LARGE_INTEGER li;
    __int64 CounterStart, CounterStop;
public:
    WinClock();
    void start();
    void stop();
    double read_millisec();
    double read_microsec();
};

string bytes_to_hex_string(vector<uint8_t> bytes);

string hex_string_to_binary(const string s);

#endif