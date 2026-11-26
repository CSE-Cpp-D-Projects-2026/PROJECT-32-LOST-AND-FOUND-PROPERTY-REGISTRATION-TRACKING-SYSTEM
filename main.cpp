#include <iostream>
#include <string>
using namespace std;

class Loc {
public:
    int id;
    string nm, tp;

    void get() {
        cout << "Loc ID: ";
        cin >> id;
        cin.ignore();
        cout << "Name: ";
        getline(cin, nm);
        cout << "Type (Building/Hall/Desk): ";
        getline(cin, tp);
    }

    void show() {
        cout << id << " | " << nm << " | " << tp << "\n";
    }
};

Loc locs[50];
int lc = 0;

int findLoc(int id) {
    for (int i = 0; i < lc; i++) {
        if (locs[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addLoc() {
    if (lc >= 50) {
        cout << "Location limit reached.\n";
        return;
    }

    Loc x;
    x.get();

    if (x.id <= 0 || findLoc(x.id) != -1) {
        cout << "Invalid or duplicate ID.\n";
        return;
    }

    if (x.nm == "" || x.tp == "") {
        cout << "Empty field not allowed.\n";
        return;
    }

    locs[lc] = x;
    lc++;
    cout << "Location added.\n";
}

void viewLocs() {
    if (lc == 0) {
        cout << "No locations.\n";
        return;
    }
    for (int i = 0; i < lc; i++) {
        locs[i].show();
    }
}

int main() {
    int ch;
    do {
        cout << "\n--- LOST & FOUND SYSTEM ---\n";
        cout << "1. Add Location\n";
        cout << "2. View Locations\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> ch;

        switch (ch) {
            case 1:
                addLoc();
                break;
            case 2:
                viewLocs();
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Wrong option.\n";
        }
    } while (ch != 0);

    return 0;
}
