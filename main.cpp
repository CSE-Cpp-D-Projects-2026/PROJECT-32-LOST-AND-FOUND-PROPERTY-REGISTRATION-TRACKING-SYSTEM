#include "common.hpp"

void runMenu(const string& title, const vector<string>& options, void (*handler)(int)) {
    while (true) {
        printLine(title);
        for (size_t i = 0; i < options.size(); i++) cout << i + 1 << ". " << options[i] << "\n";
        cout << "0. Back\n";
        int choice = readInt("Enter choice: ", 0, (int)options.size());
        if (choice == 0) return;
        try {
            handler(choice);
        } catch (const NotFoundException& e) {
            cout << "Not found: " << e.what() << "\n";
        } catch (const ValidationException& e) {
            cout << "Invalid: " << e.what() << "\n";
        } catch (const FileException& e) {
            cout << "File error: " << e.what() << "\n";
        } catch (const AppException& e) {
            cout << "Error: " << e.what() << "\n";
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << "\n";
        }
    }
}

void handleMain(int choice) {
    if (choice == 1) {
        Date d = readDate("Enter a date (YYYY-MM-DD or YYYY-MM-DD HH:MM): ");
        int days = today() - d;
        if (days < 0) cout << "That date is in the future.\n";
        else cout << d.toString() << " was " << days << " day(s) ago.\n";
    }
}

int main() {
    cout << "LOST AND FOUND PROPERTY REGISTRATION AND TRACKING SYSTEM\n";
    try {
        runMenu("MAIN MENU", {"Check a date"}, handleMain);
        cout << "Goodbye.\n";
    } catch (const exception& e) {
        cout << "Program stopped: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
