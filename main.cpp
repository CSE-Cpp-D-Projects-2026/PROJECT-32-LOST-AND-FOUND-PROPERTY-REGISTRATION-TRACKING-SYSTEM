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
class Usr {
public:
    int id;
    string nm, ph, ref;

    void get() {
        cout << "User ID: ";
        cin >> id;
        cin.ignore();
        cout << "Name: ";
        getline(cin, nm);
        cout << "Phone: ";
        getline(cin, ph);
        cout << "Report Ref: ";
        getline(cin, ref);
    }

    void show() {
        cout << id << " | " << nm << " | " << ph << " | Ref:" << ref << "\n";
    }
};

Usr usrs[100];
int uc = 0;

int findUsr(int id) {
    for (int i = 0; i < uc; i++) {
        if (usrs[i].id == id) return i;
    }
    return -1;
}

void addUsr() {
    if (uc >= 100) {
        cout << "User limit reached.\n";
        return;
    }
    Usr x;
    x.get();
    if (x.id <= 0 || findUsr(x.id) != -1) {
        cout << "Invalid or duplicate ID.\n";
        return;
    }
    if (x.nm == "" || x.ph == "" || x.ref == "") {
        cout << "Empty field not allowed.\n";
        return;
    }
    usrs[uc] = x;
    uc++;
    cout << "User added.\n";
}

void viewUsrs() {
    if (uc == 0) {
        cout << "No users.\n";
        return;
    }
    for (int i = 0; i < uc; i++) {
        usrs[i].show();
    }
}

int main() {
    int ch;
    do {
        cout << "\n--- LOST & FOUND SYSTEM ---\n";
        cout << "1. Add Location\n";
        cout << "2. View Locations\n";
        cout << "3. Add User\n";
        cout << "4. View User\n";
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
            case 3: 
                addUsr(); 
                break;    
            case 4: 
                viewUsrs(); 
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
