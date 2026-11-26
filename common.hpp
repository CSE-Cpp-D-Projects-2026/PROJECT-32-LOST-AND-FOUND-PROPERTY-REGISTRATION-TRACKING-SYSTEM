#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <ctime>

using namespace std;

const int STATUS_COUNT = 5;
const int CATEGORY_COUNT = 6;
const int ID_LEN = 16;
const int TEXT_LEN = 80;
const int UNCLAIMED_DAYS = 30;
const int RECENT_DAYS = 7;
const int FREE_DAYS = 7;
const int MATCH_THRESHOLD = 50;
const double FEE_PER_DAY = 5.0;
const double MAX_REWARD = 100000.0;

enum Status { OPEN, POTENTIAL_MATCH, UNDER_VERIFICATION, RETURNED, UNCLAIMED };
enum ReportType { LOST, FOUND };

const string STATUS_NAMES[STATUS_COUNT] = {
    "Open", "Potential Match", "Under Verification", "Returned", "Unclaimed"
};

const string CATEGORY_NAMES[CATEGORY_COUNT] = {
    "Electronics", "Documents", "Clothing", "Accessories", "Bags", "Other"
};

const string LOCATION_TYPES[4] = {
    "Building", "Hall", "Transport Point", "Service Desk"
};

const bool ALLOWED[STATUS_COUNT][STATUS_COUNT] = {
    {false, true, false, false, true},
    {true, false, true, false, false},
    {true, false, false, true, false},
    {false, false, false, false, false},
    {true, false, false, false, false}
};

class AppException : public runtime_error {
public:
    AppException(const string& msg) : runtime_error(msg) {}
};

class ValidationException : public AppException {
public:
    ValidationException(const string& msg) : AppException(msg) {}
};

class NotFoundException : public AppException {
public:
    NotFoundException(const string& msg) : AppException(msg) {}
};

class FileException : public AppException {
public:
    FileException(const string& msg) : AppException(msg) {}
};

inline string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline string toLower(string s) {
    for (char& c : s) c = tolower((unsigned char)c);
    return s;
}

inline bool containsText(const string& text, const string& word) {
    return toLower(text).find(toLower(word)) != string::npos;
}

inline vector<string> splitBy(const string& s, char sep) {
    vector<string> parts;
    string cur;
    stringstream ss(s);
    while (getline(ss, cur, sep)) parts.push_back(cur);
    if (!s.empty() && s.back() == sep) parts.push_back("");
    return parts;
}

inline vector<string> splitWords(const string& s) {
    vector<string> words;
    stringstream ss(toLower(s));
    string w;
    while (ss >> w) {
        w.erase(remove_if(w.begin(), w.end(), [](char c) { return !isalnum((unsigned char)c); }), w.end());
        if (w.size() > 2) words.push_back(w);
    }
    return words;
}

inline void checkLength(const string& text, const string& label) {
    if ((int)text.size() >= TEXT_LEN)
        throw ValidationException(label + " must be shorter than " + to_string(TEXT_LEN) + " characters");
}

inline bool isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

inline int daysInMonth(int y, int m) {
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeap(y)) return 29;
    return days[m - 1];
}

struct Date {
    int year, month, day, hour, minute;

    Date() : year(2000), month(1), day(1), hour(0), minute(0) {}
    Date(int y, int m, int d, int h = 0, int mi = 0)
        : year(y), month(m), day(d), hour(h), minute(mi) {}

    bool isValid() const {
        if (year < 1990 || year > 2100 || month < 1 || month > 12) return false;
        if (day < 1 || day > daysInMonth(year, month)) return false;
        return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
    }

    int toDays() const {
        int y = year, m = month;
        if (m <= 2) {
            y--;
            m += 12;
        }
        return 365 * y + y / 4 - y / 100 + y / 400 + (153 * (m - 3) + 2) / 5 + day;
    }

    long long key() const {
        return (long long)toDays() * 10000 + hour * 100 + minute;
    }

    string toString() const {
        char buf[32];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d", year, month, day, hour, minute);
        return string(buf);
    }

    bool operator<(const Date& o) const { return key() < o.key(); }
    bool operator==(const Date& o) const { return key() == o.key(); }
    int operator-(const Date& o) const { return toDays() - o.toDays(); }
};

inline Date parseDate(const string& text) {
    int y = 0, m = 0, d = 0, h = 0, mi = 0;
    int n = sscanf(text.c_str(), "%d-%d-%d %d:%d", &y, &m, &d, &h, &mi);
    if (n != 3 && n != 5)
        throw ValidationException("Date must look like YYYY-MM-DD or YYYY-MM-DD HH:MM");
    Date dt(y, m, d, h, mi);
    if (!dt.isValid())
        throw ValidationException("That is not a real calendar date or time: " + text);
    return dt;
}

inline Date today() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return Date(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min);
}

template <typename T>
T maxOf(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
string toText(T value) {
    ostringstream out;
    out << value;
    return out.str();
}

template <>
inline string toText<bool>(bool value) {
    return value ? "Yes" : "No";
}

template <typename T>
class Table {
    vector<T> rows;

public:
    void add(const T& row) { rows.push_back(row); }
    bool empty() const { return rows.empty(); }
    int size() const { return (int)rows.size(); }
    const T& at(int i) const { return rows[i]; }
};

template <typename T>
void printTable(const Table<T>& table, const string& emptyMessage) {
    if (table.empty()) {
        cout << emptyMessage << "\n";
        return;
    }
    for (int i = 0; i < table.size(); i++) cout << table.at(i) << "\n";
    cout << "Total: " << table.size() << "\n";
}

inline void printLine(const string& text) {
    cout << "\n=== " << text << " ===\n";
}

inline void printLine(char c, int count) {
    cout << string(count, c) << "\n";
}

inline string readLine(const string& prompt, bool allowEmpty = false) {
    while (true) {
        cout << prompt;
        string s;
        if (!getline(cin, s)) throw AppException("Input stream closed");
        s = trim(s);
        replace(s.begin(), s.end(), '|', '/');
        if (!s.empty() || allowEmpty) return s;
        cout << "Error: this field is required.\n";
    }
}

inline int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            if (pos != s.size()) throw invalid_argument("extra characters");
            if (v < lo || v > hi) {
                cout << "Error: value must be between " << lo << " and " << hi << ".\n";
                continue;
            }
            return v;
        } catch (const invalid_argument&) {
            cout << "Error: please enter a whole number.\n";
        } catch (const out_of_range&) {
            cout << "Error: that number is too large.\n";
        }
    }
}

inline double readDouble(const string& prompt, double lo, double hi) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos != s.size()) throw invalid_argument("extra characters");
            if (!(v >= lo && v <= hi)) {
                cout << "Error: value must be between " << lo << " and " << hi << ".\n";
                continue;
            }
            return v;
        } catch (const invalid_argument&) {
            cout << "Error: please enter a number.\n";
        } catch (const out_of_range&) {
            cout << "Error: that number is too large.\n";
        }
    }
}

inline Date readDate(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            return parseDate(s);
        } catch (const ValidationException& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

inline bool askYes(const string& prompt) {
    while (true) {
        string s = toLower(readLine(prompt));
        if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        cout << "Error: please answer y or n.\n";
    }
}

#endif
