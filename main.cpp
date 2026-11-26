#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

class Loc {
public:
    int id;
    string nm, tp;
    void show() {
        cout << id << " | " << nm << " | " << tp << "\n";
    }
};

Loc locs[50];
int lc = 0;

int findLoc(int id) {
    for (int i = 0; i < lc; i++)
        if (locs[i].id == id) return i;
    return -1;
}

void addLoc() {
    if (lc >= 50) throw "Location limit reached";
    Loc x;
    cout << "Loc ID: ";
    if (!(cin >> x.id)) { cin.clear(); cin.ignore(1000, '\n'); throw "Invalid ID"; }
    cin.ignore(1000, '\n');
    if (x.id <= 0 || findLoc(x.id) != -1) throw "Invalid or duplicate ID";
    cout << "Name: "; getline(cin, x.nm);
    cout << "Type (Building/Hall/Desk): "; getline(cin, x.tp);
    if (x.nm.empty() || x.tp.empty()) throw "Empty field not allowed";
    locs[lc++] = x;
    cout << "Location added.\n";
}

void viewLocs() {
    if (lc == 0) { cout << "No locations.\n"; return; }
    for (int i = 0; i < lc; i++) locs[i].show();
}

int main() {
    int ch;
    do {
        cout << "\n--- LOST & FOUND SYSTEM ---\n";
        cout << "1. Add Location\n2. View Locations\n0. Exit\nChoice: ";
        try {
            if (!(cin >> ch)) { cin.clear(); cin.ignore(1000, '\n'); throw "Invalid choice"; }
            if (ch == 1) addLoc();
            else if (ch == 2) viewLocs();
            else if (ch != 0) cout << "Wrong option.\n";
        } catch (const char* e) {
            cout << "Error: " << e << "\n";
        }
    } while (ch != 0);
    return 0;
}
