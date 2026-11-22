#include <iostream>
#include <string>
#include <mysql.h>

using namespace std;

string escapeStr(MYSQL* conn, const string& input) {
    string escaped(input.size() * 2 + 1, '\0');
    unsigned long len = mysql_real_escape_string(
        conn, &escaped[0], input.c_str(), static_cast<unsigned long>(input.size())
    );
    escaped.resize(len);
    return escaped;
}

int main() {
    MYSQL* connection = mysql_init(nullptr);
    if (!connection) {
        cout << "Error: Could not initialize MySQL" << endl;
        return 1;
    }

    if (mysql_real_connect(connection, "localhost", "root", "YOUR_PASSWORD", "SharkDB", 3306, nullptr, 0)) {
        cout << "We welcome you to the WORLD OF SHARKS! \nHope you'll have a nice time here and will learn something new from here!!!" << endl;
    } else {
        cout << "Error: " << mysql_error(connection) << endl;
        mysql_close(connection);
        return 1;
    }

    // way to the main menu
    while (true) {
        cout << "Shark Database Menu" << endl;
        cout << "1. View all sharks" << endl;
        cout << "2. Add a new shark" << endl;
        cout << "3. Search for a shark by name" << endl;
        cout << "4. Exit" << endl;

        cout << "Enter your choice: ";
        string choice;
        if (!getline(cin, choice)) {
            break;
        }
        if (!choice.empty() && choice.back() == '\r') {
            choice.pop_back();
        }

        if (choice == "1") {
            // Displays all sharks
            if (mysql_query(connection, "SELECT name, description, habitat, diet, status, fact FROM sharks;") == 0) {
                MYSQL_RES* res = mysql_store_result(connection);
                if (res) {
                    MYSQL_ROW shark;
                    while ((shark = mysql_fetch_row(res))) {
                        cout << "\nName: " << (shark[0] ? shark[0] : "")
                             << "\nDescription: " << (shark[1] ? shark[1] : "")
                             << "\nHabitat: " << (shark[2] ? shark[2] : "")
                             << "\nDiet: " << (shark[3] ? shark[3] : "")
                             << "\nStatus: " << (shark[4] ? shark[4] : "")
                             << "\nFact: " << (shark[5] ? shark[5] : "") << endl;
                    }
                    mysql_free_result(res);
                }
            } else {
                cout << "Error: " << mysql_error(connection) << endl;
            }
        } else if (choice == "2") {
            // Adds a new shark to db
            string name, description, habitat, diet, status, fact;
            cout << "Enter Shark's name: ";
            getline(cin, name);
            cout << "Enter its description: ";
            getline(cin, description);
            cout << "Enter its habitat: ";
            getline(cin, habitat);
            cout << "Enter its diet: ";
            getline(cin, diet);
            cout << "Enter the conservation status: ";
            getline(cin, status);
            cout << "Enter a fun fact: ";
            getline(cin, fact);

            string query =
                "INSERT INTO sharks (name, description, habitat, diet, status, fact) VALUES ('" +
                escapeStr(connection, name) + "', '" +
                escapeStr(connection, description) + "', '" +
                escapeStr(connection, habitat) + "', '" +
                escapeStr(connection, diet) + "', '" +
                escapeStr(connection, status) + "', '" +
                escapeStr(connection, fact) + "')";

            if (mysql_query(connection, query.c_str()) == 0) {
                cout << "Shark '" << name << "' added successfully." << endl;
            } else {
                cout << "Error: " << mysql_error(connection) << endl;
            }
        } else if (choice == "3") {
            // Search for shark by name
            string sharkname;
            cout << "Enter the name of the shark to search: ";
            getline(cin, sharkname);

            string query =
                "SELECT name, description, habitat, diet, status, fact FROM sharks WHERE name = '" +
                escapeStr(connection, sharkname) + "'";

            if (mysql_query(connection, query.c_str()) == 0) {
                MYSQL_RES* res = mysql_store_result(connection);
                if (res) {
                    MYSQL_ROW shark = mysql_fetch_row(res);
                    if (shark) {
                        cout << "\nName: " << (shark[0] ? shark[0] : "")
                             << "\nDescription: " << (shark[1] ? shark[1] : "")
                             << "\nHabitat: " << (shark[2] ? shark[2] : "")
                             << "\nDiet: " << (shark[3] ? shark[3] : "")
                             << "\nStatus: " << (shark[4] ? shark[4] : "")
                             << "\nFact: " << (shark[5] ? shark[5] : "") << endl;
                    } else {
                        cout << "No shark named '" << sharkname << "' found in the database." << endl;
                    }
                    mysql_free_result(res);
                }
            } else {
                cout << "Error: " << mysql_error(connection) << endl;
            }
        } else if (choice == "4") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice" << endl;
        }
    }

    if (connection) {
        mysql_close(connection);
        cout << "Connection to SharkDB has been closed." << endl;
        cout << "THANK YOU FOR SHOWING INTEREST IN LEARNING ABOUT SHARKS \nvisit again!!!" << endl;
    }

    return 0;
}
