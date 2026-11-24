#include <iostream>
#include <string>
#include <mysql.h>

using namespace std;

// Base class (Inheritance)
class MarineAnimal {
protected:
    string name;
    string habitat;
    string diet;

public:
    MarineAnimal() {}

    MarineAnimal(string n, string h, string d) {
        name = n;
        habitat = h;
        diet = d;
    }

    string getName() { return name; }
    string getHabitat() { return habitat; }
    string getDiet() { return diet; }

    // Virtual function for polymorphism
    virtual void display() {
        cout << "\nName: " << name
             << "\nHabitat: " << habitat
             << "\nDiet: " << diet << endl;
    }

    virtual ~MarineAnimal() {}
};

// Derived class inheriting from MarineAnimal
class Shark : public MarineAnimal {
private:
    string description;
    string status;
    string fact;

public:
    Shark() {}

    Shark(string n, string desc, string h, string d, string st, string f)
        : MarineAnimal(n, h, d) {
        description = desc;
        status = st;
        fact = f;
    }

    string getDescription() { return description; }
    string getStatus() { return status; }
    string getFact() { return fact; }

    void inputDetails() {
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
    }

    // Overriding display() from MarineAnimal
    void display() override {
        cout << "\nName: " << name
             << "\nDescription: " << description
             << "\nHabitat: " << habitat
             << "\nDiet: " << diet
             << "\nStatus: " << status
             << "\nFact: " << fact << endl;
    }
};

// Handles all MySQL database operations (Encapsulation & Abstraction)
class SharkDatabase {
private:
    MYSQL* connection;
    bool connected;

    string escapeStr(string input) {
        string escaped(input.size() * 2 + 1, '\0');
        unsigned long len = mysql_real_escape_string(
            connection, &escaped[0], input.c_str(), (unsigned long)input.size()
        );
        escaped.resize(len);
        return escaped;
    }

public:
    // Constructor connects to MySQL
    SharkDatabase(string host, string user, string pass, string dbName) {
        connected = false;
        connection = mysql_init(nullptr);
        if (!connection) {
            cout << "Error: Could not initialize MySQL" << endl;
            return;
        }

        if (mysql_real_connect(connection, host.c_str(), user.c_str(), pass.c_str(), dbName.c_str(), 3306, nullptr, 0)) {
            connected = true;
            cout << "We welcome you to the WORLD OF SHARKS! \nHope you'll have a nice time here and will learn something new from here!!!" << endl;
        } else {
            cout << "Error: " << mysql_error(connection) << endl;
            mysql_close(connection);
            connection = nullptr;
        }
    }

    // Destructor automatically closes the connection
    ~SharkDatabase() {
        if (connection) {
            mysql_close(connection);
            cout << "Connection to SharkDB has been closed." << endl;
            cout << "THANK YOU FOR SHOWING INTEREST IN LEARNING ABOUT SHARKS \nvisit again!!!" << endl;
        }
    }

    bool isConnected() {
        return connected;
    }

    void viewAllSharks() {
        if (mysql_query(connection, "SELECT name, description, habitat, diet, status, fact FROM sharks;") == 0) {
            MYSQL_RES* res = mysql_store_result(connection);
            if (res) {
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) {
                    Shark s(
                        row[0] ? row[0] : "",
                        row[1] ? row[1] : "",
                        row[2] ? row[2] : "",
                        row[3] ? row[3] : "",
                        row[4] ? row[4] : "",
                        row[5] ? row[5] : ""
                    );
                    s.display();
                }
                mysql_free_result(res);
            }
        } else {
            cout << "Error: " << mysql_error(connection) << endl;
        }
    }

    void addShark(Shark s) {
        string query =
            "INSERT INTO sharks (name, description, habitat, diet, status, fact) VALUES ('" +
            escapeStr(s.getName()) + "', '" +
            escapeStr(s.getDescription()) + "', '" +
            escapeStr(s.getHabitat()) + "', '" +
            escapeStr(s.getDiet()) + "', '" +
            escapeStr(s.getStatus()) + "', '" +
            escapeStr(s.getFact()) + "')";

        if (mysql_query(connection, query.c_str()) == 0) {
            cout << "Shark '" << s.getName() << "' added successfully." << endl;
        } else {
            cout << "Error: " << mysql_error(connection) << endl;
        }
    }

    void searchShark(string sharkname) {
        string query =
            "SELECT name, description, habitat, diet, status, fact FROM sharks WHERE name = '" +
            escapeStr(sharkname) + "'";

        if (mysql_query(connection, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(connection);
            if (res) {
                MYSQL_ROW row = mysql_fetch_row(res);
                if (row) {
                    Shark s(
                        row[0] ? row[0] : "",
                        row[1] ? row[1] : "",
                        row[2] ? row[2] : "",
                        row[3] ? row[3] : "",
                        row[4] ? row[4] : "",
                        row[5] ? row[5] : ""
                    );
                    s.display();
                } else {
                    cout << "No shark named '" << sharkname << "' found in the database." << endl;
                }
                mysql_free_result(res);
            }
        } else {
            cout << "Error: " << mysql_error(connection) << endl;
        }
    }
};

int main() {
    SharkDatabase db("localhost", "root", "YOUR_PASSWORD", "SharkDB");
    if (!db.isConnected()) {
        return 1;
    }

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
            db.viewAllSharks();
        } else if (choice == "2") {
            Shark newShark;
            newShark.inputDetails();
            db.addShark(newShark);
        } else if (choice == "3") {
            string sharkname;
            cout << "Enter the name of the shark to search: ";
            getline(cin, sharkname);
            db.searchShark(sharkname);
        } else if (choice == "4") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}
