#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// ======================
// ABSTRACT BASE CLASS
// ======================
class EmissionSource {
public:
    virtual double calculateEmission() = 0;
    virtual ~EmissionSource() {}
};

// ======================
// TRANSPORT
// ======================
class Transport : public EmissionSource {
private:
    double km;

public:
    Transport(double k) : km(k) {}

    double calculateEmission() override {
        return km * 0.21;
    }
};

// ======================
// ELECTRICITY
// ======================
class Electricity : public EmissionSource {
private:
    double kwh;

public:
    Electricity(double k) : kwh(k) {}

    double calculateEmission() override {
        return kwh * 0.50;
    }
};

// ======================
// FOOD
// ======================
class Food : public EmissionSource {
private:
    double kg;

public:
    Food(double k) : kg(k) {}

    double calculateEmission() override {
        return kg * 27.0;
    }
};

// ======================
// MONTHLY RECORD
// ======================
class MonthlyRecord {
private:
    string month;
    double total;

public:
    MonthlyRecord(string m, double t) {
        month = m;
        total = t;
    }

    string getMonth() {
        return month;
    }

    double getTotal() {
        return total;
    }

    void display() {
        cout << month << " -> " << total << " kg CO2\n";
    }
};

// ======================
// USER CLASS
// ======================
class User {
private:
    string name;
    vector<MonthlyRecord> records;

public:
    User(string n = "") {
        name = n;
    }

    string getName() {
        return name;
    }

    void addRecord(MonthlyRecord r) {
        records.push_back(r);
    }

    vector<MonthlyRecord>& getRecords() {
        return records;
    }

    void viewHistory() {
        if (records.empty()) {
            cout << "No records found.\n";
            return;
        }

        for (auto &r : records)
            r.display();
    }

    void highestMonth() {
        if (records.empty()) return;

        MonthlyRecord maxR = records[0];

        for (auto &r : records)
            if (r.getTotal() > maxR.getTotal())
                maxR = r;

        cout << "Highest Month: ";
        maxR.display();
    }

    void lowestMonth() {
        if (records.empty()) return;

        MonthlyRecord minR = records[0];

        for (auto &r : records)
            if (r.getTotal() < minR.getTotal())
                minR = r;

        cout << "Lowest Month: ";
        minR.display();
    }
};

// ======================
// SYSTEM (MULTI USER + FILE)
// ======================
class System {
private:
    vector<User> users;
    int currentUser;

public:
    System() {
        currentUser = -1;
    }

    // REGISTER USER
    void registerUser() {
        string name;
        cout << "Enter username: ";
        cin >> name;

        users.push_back(User(name));
        cout << "User registered successfully!\n";
    }

    // LOGIN USER
    void loginUser() {
        string name;
        cout << "Enter username: ";
        cin >> name;

        for (int i = 0; i < users.size(); i++) {
            if (users[i].getName() == name) {
                currentUser = i;
                cout << "Login successful!\n";
                return;
            }
        }

        cout << "User not found!\n";
    }

    bool isLoggedIn() {
        return currentUser != -1;
    }

    User& getUser() {
        return users[currentUser];
    }

    // ADD MONTHLY RECORD
    void addMonthlyRecord() {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }

        string month;
        double km, kwh, foodKg;

        cout << "Enter month: ";
        cin >> month;

        cout << "Transport km: ";
        cin >> km;

        cout << "Electricity kWh: ";
        cin >> kwh;

        cout << "Food kg: ";
        cin >> foodKg;

        Transport t(km);
        Electricity e(kwh);
        Food f(foodKg);

        double total =
            t.calculateEmission() +
            e.calculateEmission() +
            f.calculateEmission();

        getUser().addRecord(MonthlyRecord(month, total));

        cout << "Monthly record added!\n";
    }

    // SAVE FILE
    void saveToFile() {
        ofstream file("carbon_data.txt");

        for (auto &u : users) {
            for (auto &r : u.getRecords()) {
                file << u.getName() << " "
                     << r.getMonth() << " "
                     << r.getTotal() << "\n";
            }
        }

        file.close();
        cout << "Data saved to file!\n";
    }

    // LOAD FILE
    void loadFile() {
        ifstream file("carbon_data.txt");

        string name, month;
        double total;

        cout << "\n--- FILE DATA ---\n";

        while (file >> name >> month >> total) {
            cout << name << " | " << month << " | " << total << " kg CO2\n";
        }

        file.close();
    }
};

// ======================
// MAIN MENU
// ======================
int main() {

    System sys;
    int choice;

    do {
        cout << "\n=============================\n";
        cout << " CARBON FOOTPRINT SYSTEM\n";
        cout << "=============================\n";

        cout << "1. Register User\n";
        cout << "2. Login User\n";
        cout << "3. Add Monthly Record\n";
        cout << "4. View History\n";
        cout << "5. Highest Emission Month\n";
        cout << "6. Lowest Emission Month\n";
        cout << "7. Save Data\n";
        cout << "8. Load File Data\n";
        cout << "9. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            sys.registerUser();
            break;

        case 2:
            sys.loginUser();
            break;

        case 3:
            sys.addMonthlyRecord();
            break;

        case 4:
            if (sys.isLoggedIn())
                sys.getUser().viewHistory();
            else
                cout << "Login first!\n";
            break;

        case 5:
            if (sys.isLoggedIn())
                sys.getUser().highestMonth();
            else
                cout << "Login first!\n";
            break;

        case 6:
            if (sys.isLoggedIn())
                sys.getUser().lowestMonth();
            else
                cout << "Login first!\n";
            break;

        case 7:
            sys.saveToFile();
            break;

        case 8:
            sys.loadFile();
            break;

        case 9:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 9);

    return 0;
}