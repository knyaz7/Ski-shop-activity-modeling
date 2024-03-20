#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

ofstream file("report.txt");
ofstream results("results.txt");

class Equipment {
protected:
    string brand;
    string model;
    string level;
    int quantity;
    int serialNumber;
    string type;
public:
    Equipment() {
        quantity = 0;
        serialNumber = 0;
    }
    int showQuantity() const {
        return quantity;
    }
    int getSN() const {
        return serialNumber;
    }
    string getLevel() const {
        return level;
    }
    void buy() {
        quantity -= 1;
    }
    void pullback() {
        quantity += 1;
    }
    void showEquipment();
    friend class Sell;
    friend class Rental;
};

class Client {
protected:
    string firstName;
    string lastName;
    string middleName;
    string passport;
};

class SkiShop {
protected:
    string day;
    string month;
    string year;
    string hour;
    string minute;
    double price;
    bool isValidDate(string date) {
        if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
            return false;
        }

        int day = stringToInt(date.substr(0, 2));
        int month = stringToInt(date.substr(3, 2));
        int year = stringToInt(date.substr(6, 4));

        if (day < 1 || day > 31 || month < 1 || month > 12) {
            return false;
        }

        if (year < 1900 || year > 2100) {
            return false;
        }

        return true;
    }
    bool isValidTime(string time) {
        if (time.length() != 5 || time[2] != ':') {
            return false;
        }

        int hour = stringToInt(time.substr(0, 2));
        int minute = stringToInt(time.substr(3, 2));

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            return false;
        }

        return true;
    }
public:
    SkiShop() {
        price = 0;
    }
    double getPrice() const {
        return price;
    }
    static int stringToInt(string x) {
        if (x[0] == '0') {
            x = x[1];
            return stoi(x);
        }
        return stoi(x);
    }
    static string intToString(int x) {
        if (x < 10) {
            return ('0' + to_string(x));
        }
        return to_string(x);
    }
};

class Ski: public Equipment {
public:
    Ski(string b, string m, string l, int q, int sN, string t) {
        this->brand = b;
        this->model = m;
        this->level = l;
        this->quantity = q;
        this->serialNumber = sN;
        this->type = t;
    }
    void showEquipment() {
        cout << "Ski " << brand << " " << model << " " << level << " " << quantity << " " << serialNumber << endl;
    }
    string getMainInfo() const {
        return "Лыжи " + brand + ' ' + model;
    }
};

class Snowboard : public Equipment {
public:
    Snowboard(string b, string m, string l, int q, int sN, string t) {
        this->brand = b;
        this->model = m;
        this->level = l;
        this->quantity = q;
        this->serialNumber = sN;
        this->type = t;
    }
    void showEquipment() {
        cout << "Snowboard " << brand << " " << model << " " << level << " " << quantity << " " << serialNumber << endl;
    }
    string getMainInfo() const {
        return "Сноуборд " + brand + ' ' + model;
    }
};

class Sell : public Client, public SkiShop {
private:
    vector<Equipment> boughtEquip;
public:
    Sell(string ln, string fn, string mn, string p, string date, string time, double price, vector<Equipment> be) {
        if (!isValidDate(date)) {
            cout << "CRITICAL ERROR! Date entered incorrect. Object was not created. Try again dd/mm/yyyy" << endl;
            return;
        }

        if (!isValidTime(time)) {
            cout << "CRITICAL ERROR! Time entered incorrect. Object was not created. Try again hh:mm" << endl;
            return;
        }

        this->lastName = ln;
        this->firstName = fn;
        this->middleName = mn;
        this->passport = p;
        this->day = date.substr(0, 2);
        this->month = date.substr(3, 2);
        this->year = date.substr(6, 4);
        this->hour = time.substr(0, 2);
        this->minute = time.substr(3, 2);
        this->price = price;
        this->boughtEquip = be;
    }
    void printSell() {
        cout << "Продано: ";
        file << "Продано: ";
        for (int i = 0; i < boughtEquip.size(); i++) {
            string type = (boughtEquip[i].type == "ski") ? "Лыжи " : "Сноуборд ";
            cout << type << boughtEquip[i].brand + " " + boughtEquip[i].model;
            file << type << boughtEquip[i].brand + " " + boughtEquip[i].model;
            if (i != boughtEquip.size() - 1) {
                cout << ", ";
                file << ", ";
            }
        }
        cout << endl;
        file << endl;

        cout << "Дата продажи: " << day + '/' + month + '/' + year << " в " << hour + ':' + minute << endl;
        file << "Дата продажи: " << day + '/' + month + '/' + year << " в " << hour + ':' + minute << endl;
        cout << "Цена продажи: " << price << endl;
        file << "Цена продажи: " << price << endl;
        cout << "Покупатель: " << lastName << " " << firstName << " " << middleName << endl;
        file << "Покупатель: " << lastName << " " << firstName << " " << middleName << endl;
        //cout << "Паспорт покупателя: " << passport << endl;
        cout << endl;
        file << endl;
    }
    int getSerial(int id) const {
        return boughtEquip.at(id).serialNumber;
    }
    int getQuantityItems() const {
        return boughtEquip.size();
    }
};

class Rental : public Client, public SkiShop {
private:
    int days;
    bool isReturned;
    vector<Equipment> rentedEquip;
public:
    Rental() {
        days = 0;
        isReturned = true;
    }
    Rental(string ln, string fn, string mn, string p, string date, string time, double price, int d, vector<Equipment> re) {
        if (!isValidDate(date)) {
            days = 0;
            isReturned = true;
            cout << "CRITICAL ERROR! Date entered incorrect. Object was not created. Try again dd/mm/yyyy" << endl;
            return;
        }

        if (!isValidTime(time)) {
            days = 0;
            isReturned = true;
            cout << "CRITICAL ERROR! Time entered incorrect. Object was not created. Try again hh:mm" << endl;
            return;
        }

        this->lastName = ln;
        this->firstName = fn;
        this->middleName = mn;
        this->passport = p;
        this->day = date.substr(0, 2);
        this->month = date.substr(3, 2);
        this->year = date.substr(6, 4);
        this->hour = time.substr(0, 2);
        this->minute = time.substr(3, 2);
        this->price = price;
        this->days = d;
        this->isReturned = false;
        this->rentedEquip = re;
    }
    bool wasReturned() const {
        return isReturned;
    }
    string getDay() const {
        return day;
    }
    string getName() const {
        return lastName + ' ' + firstName + ' ' + middleName;
    }
    string getPassport() const {
        return passport;
    }
    int getRentDays() const {
        return days;
    }
    int getQuantityItems() const {
        return rentedEquip.size();
    }
    int getSerial(int id) const {
        return rentedEquip.at(id).serialNumber;
    }
    void setReturned() {
        isReturned = true;
    }
    void printRental() {
        cout << "Арендовано: ";
        file << "Арендовано: ";
        for (int i = 0; i < rentedEquip.size(); i++) {
            string type = (rentedEquip[i].type == "ski") ? "Лыжи " : "Сноуборд ";
            cout << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            file << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            if (i != rentedEquip.size() - 1) {
                cout << ", ";
                file << ", ";
            }
        }
        cout << endl;
        file << endl;

        cout << "Дата аренды: " << day + '/' + month + '/' + year << " в " << hour + ':' + minute << endl;
        file << "Дата аренды: " << day + '/' + month + '/' + year << " в " << hour + ':' + minute << endl;
        cout << "Цена аренды: " << price << endl;
        file << "Цена аренды: " << price << endl;
        cout << "Арендовано на: " << days << "д." << endl;
        file << "Арендовано на: " << days << "д." << endl;
        cout << "Покупатель: " << lastName << " " << firstName << " " << middleName << endl;
        file << "Покупатель: " << lastName << " " << firstName << " " << middleName << endl;
        cout << "Паспорт покупателя: " << passport << endl;
        file << "Паспорт покупателя: " << passport << endl;
        cout << endl;
        file << endl;
    }
    void printReturn() {
        cout << "Покупатель " << lastName << " " << firstName << " " << middleName << " вернул товар(ы):" << endl;
        file << "Покупатель " << lastName << " " << firstName << " " << middleName << " вернул товар(ы):" << endl;
        for (int i = 0; i < rentedEquip.size(); i++) {
            string type = (rentedEquip[i].type == "ski") ? "Лыжи " : "Сноуборд ";
            cout << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            file << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            if (i != rentedEquip.size() - 1) {
                cout << ", ";
                file << ", ";
            }
        }
        cout << endl << endl;
        file << endl << endl;
    }
    void printAbortedReturn() {
        cout << "Покупатель " << lastName << " " << firstName << " " << middleName << " не вернул в указанный срок товар(ы):" << endl;
        file << "Покупатель " << lastName << " " << firstName << " " << middleName << " не вернул в указанный срок товар(ы):" << endl;
        for (int i = 0; i < rentedEquip.size(); i++) {
            string type = (rentedEquip[i].type == "ski") ? "Лыжи " : "Сноуборд ";
            cout << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            file << type << rentedEquip[i].brand + " " + rentedEquip[i].model;
            if (i != rentedEquip.size() - 1) {
                cout << ", ";
                file << ", ";
            }
        }
        cout << endl;
        file << endl;
        cout << "Его паспорт: " << passport << endl;
        file << "Его паспорт: " << passport << endl;
        cout << endl;
        file << endl;
    }
};

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

bool isEmptyStore(const vector<Ski> &sk, const vector<Snowboard>& sb) {
    for (int i = 0; i < sk.size(); i++) {
        if (sk[i].showQuantity() > 0) {
            return false;
        }
    }
    for (int i = 0; i < sb.size(); i++) {
        if (sb[i].showQuantity() > 0) {
            return false;
        }
    }
    return true;
}

string generatePassport() {
    return "60" + to_string(random(0, 9)) + to_string(random(0, 9)) + '-' + to_string(random(100000, 999999));
}

bool bringBack(int SN, vector<Ski>& sk, vector<Snowboard>& sb) {
    for (int i = 0; i < sk.size(); i++) {
        if (sk[i].getSN() == SN) {
            sk[i].pullback();
            return true;
        }
    }
    for (int i = 0; i < sb.size(); i++) {
        if (sb[i].getSN() == SN) {
            sb[i].pullback();
            return true;
        }
    }
    return false;
}

void Simulation(vector<Ski>& ski, vector<Snowboard>& snowboard, vector<Sell>& sells, vector<Rental>& rents) {
    string LastNames[10] = { "Иванов", "Петров", "Сидоров", "Песчанов", "Егальцов", "Воронцов", "Дубровин", "Омариев", "Бутусов", "Богров" };
    string FirstNames[10] = { "Иван", "Петр", "Владимир", "Тимофей", "Сергей", "Алексей", "Вячеслав", "Александр", "Роман", "Виктор" };
    string MiddleNames[10] = { "Андреевич", "Александрович", "Иванович", "Попович", "Русланович", "Борисович", "Олегович", 
        "Романович", "Львович", "Николаевич" };
    bool stopPrint = false;

    for (int i = 1; i <= 31; i++) { // Дни
        cout << "//=========== День " << i << " ===========//" << endl;
        file << "//=========== День " << i << " ===========//" << endl;
        for (int j = 7; j < 22; j++) { // Часы
            for (int k = 0; k < 60; k++) { // Минуты
                if (random(1, 1000) < 10) { // Вероятность сделки в минуту
                    if (stopPrint) {
                        continue;
                    }

                    if (random(1, 100) > 70) { // Покупка
                        if (isEmptyStore(ski, snowboard)) {
                            cout << "!! На складе закончился весь товар !!" << endl;
                            file << "!! На складе закончился весь товар !!" << endl;
                            cout << endl;
                            file << endl;
                            stopPrint = true;
                        }
                        vector<Equipment> temp;

                        // Генерация случайного числа товаров для покупки
                        int numItemsToBuy = random(1, 4);

                        for (int l = 0; l < numItemsToBuy; l++) {
                            bool isSki = (random(1, 100) > 50);
                            Equipment item;

                            if (isSki) {
                                if (!isEmptyStore(ski, {})) {
                                    int randomSkiIndex = random(0, ski.size() - 1);
                                    item = ski[randomSkiIndex];
                                    if (item.showQuantity()) {
                                        //cout << item.showQuantity() << endl;
                                        temp.push_back(item);
                                        ski[randomSkiIndex].buy();
                                    }
                                }
                            }
                            else {
                                if (!isEmptyStore({}, snowboard)) {
                                    int randomBoardIndex = random(0, snowboard.size() - 1);
                                    item = snowboard[randomBoardIndex];
                                    if (item.showQuantity()) {
                                        //cout << item.showQuantity() << endl;
                                        temp.push_back(item);
                                        snowboard[randomBoardIndex].buy();
                                    }
                                }
                            }
                        }

                        if (!temp.empty()) {
                            double totalPrice = 0;

                            for (const Equipment& e : temp) {
                                totalPrice += (e.getLevel() == "Beginner") ? 30000 : ((e.getLevel() == "Medium") ? 50000 : 100000);
                            }

                            string date = SkiShop::intToString(i) + "/01/2024"; 
                            string time = SkiShop::intToString(j) + ":" + SkiShop::intToString(k);

                            sells.push_back(Sell(LastNames[random(0, 9)], FirstNames[random(0, 9)], MiddleNames[random(0, 9)], generatePassport(), date, time, totalPrice, temp));
                            sells[sells.size() - 1].printSell();
                        }
                    }
                    else { // Аренда
                        if (isEmptyStore(ski, snowboard)) {
                            cout << "!! На складе закончился весь товар !!" << endl;
                            file << "!! На складе закончился весь товар !!" << endl;
                            cout << endl;
                            file << endl;
                            stopPrint = true;
                        }
                        vector<Equipment> temp;

                        // Генерация случайного числа товаров для аренды
                        int numItemsToRent = random(1, 4);

                        for (int l = 0; l < numItemsToRent; l++) {
                            bool isSki = (random(1, 100) > 50);
                            Equipment item;

                            if (isSki) {
                                if (!isEmptyStore(ski, {})) {
                                    int randomSkiIndex = random(0, ski.size() - 1);
                                    item = ski[randomSkiIndex];
                                    if (item.showQuantity()) {
                                        //cout << item.showQuantity() << endl;
                                        temp.push_back(item);
                                        ski[randomSkiIndex].buy();
                                    }
                                }
                            }
                            else {
                                if (!isEmptyStore({}, snowboard)) {
                                    int randomBoardIndex = random(0, snowboard.size() - 1);
                                    item = snowboard[randomBoardIndex];
                                    if (item.showQuantity()) {
                                        //cout << item.showQuantity() << endl;
                                        temp.push_back(item);
                                        snowboard[randomBoardIndex].buy();
                                    }
                                }
                            }
                        }

                        if (!temp.empty()) {
                            double totalPrice = 0;
                            int rentDays = random(1, 7);

                            for (const Equipment& e : temp) {
                                totalPrice += (e.getLevel() == "Beginner") ? 1000 : ((e.getLevel() == "Medium") ? 3000 : 5000);
                            }
                            totalPrice *= rentDays;

                            string date = SkiShop::intToString(i) + "/01/2024";
                            string time = SkiShop::intToString(j) + ":" + SkiShop::intToString(k);

                            rents.push_back(Rental(LastNames[random(0, 9)], FirstNames[random(0, 9)], MiddleNames[random(0, 9)], generatePassport(), date, time, totalPrice, rentDays, temp));
                            rents[rents.size() - 1].printRental();
                        }
                    }
                }
            }
        }
        for (int m = 0; m < rents.size(); m++) { // Возврат арендованного снаряжения
            if (!rents[m].wasReturned()) {
                int rentDay = SkiShop::stringToInt(rents[m].getDay());
                int rentalDays = rents[m].getRentDays();
                int returnDay = rentDay + rentalDays;

                if (i != returnDay) {
                    continue;
                }

                if (random(1, 100) < 97) {
                    rents[m].printReturn();
                    rents[m].setReturned();

                    for (int n = 0; n < rents[m].getQuantityItems(); n++) {
                        if (bringBack(rents[m].getSerial(n), ski, snowboard)) {
                            stopPrint = false;
                        }
                    }
                }
                else {
                    rents[m].printAbortedReturn();
                }
            }
        }
    }
}

void CalculateResults(vector<Ski>& ski, vector<Snowboard>& snowboard, vector<Sell>& sells, vector<Rental>& rents) {
    double sellPrice = 0;
    double rentPrice = 0;
    for (int i = 0; i < sells.size(); i++) {
        sellPrice += sells[i].getPrice();
    }
    for (int i = 0; i < rents.size(); i++) {
        rentPrice += rents[i].getPrice();
    }
    results << "Выручка за январь 2024 года: \n" << fixed << setprecision(3) << "Продажи: " << sellPrice << " руб.\nПрокат: " << rentPrice << " руб." << endl;
    results << "ИТОГО: " << (sellPrice + rentPrice) << " руб." << endl;
    results << "ИТОГО БЕЗ НДС: " << (sellPrice + rentPrice) * 0.8 << " руб." << endl;
    results << endl;

    vector<int> quantityItem;
    vector<string> snItem;
    for (int i = 0; i < ski.size(); i++) {
        quantityItem.push_back(0);
        snItem.push_back(ski[i].getMainInfo());
        for (int j = 0; j < sells.size(); j++) {
            for (int k = 0; k < sells[j].getQuantityItems(); k++) {
                if (ski[i].getSN() == sells[j].getSerial(k)) {
                    quantityItem[i] += 1;
                }
            }
        }
        for (int j = 0; j < rents.size(); j++) {
            for (int k = 0; k < rents[j].getQuantityItems(); k++) {
                if (ski[i].getSN() == rents[j].getSerial(k)) {
                    quantityItem[i] += 1;
                }
            }
        }
    }
    for (int i = 0; i < snowboard.size(); i++) {
        quantityItem.push_back(0);
        snItem.push_back(snowboard[i].getMainInfo());
        for (int j = 0; j < sells.size(); j++) {
            for (int k = 0; k < sells[j].getQuantityItems(); k++) {
                if (snowboard[i].getSN() == sells[j].getSerial(k)) {
                    quantityItem[i + ski.size()] += 1;
                }
            }
        }
        for (int j = 0; j < rents.size(); j++) {
            for (int k = 0; k < rents[j].getQuantityItems(); k++) {
                if (snowboard[i].getSN() == rents[j].getSerial(k)) {
                    quantityItem[i + ski.size()] += 1;
                }
            }
        }
    }
    for (int i = 0; i < quantityItem.size(); i++) {
        for (int j = 0; j < quantityItem.size() - 1 - i; j++) {
            if (quantityItem[j] < quantityItem[j + 1]) {
                int temp = quantityItem[j];
                string t = snItem[j];
                quantityItem[j] = quantityItem[j + 1];
                snItem[j] = snItem[j + 1];
                quantityItem[j + 1] = temp;
                snItem[j + 1] = t;
            }
        }
    }
    results << "Топ-3 самых популярных снарядов по продажам/прокатам: " << endl;
    for (int i = 0; i < 3; i++) {
        results << quantityItem[i] << " покупки/проката: " << snItem[i] << endl;
    }
    results << endl;

    results << "Всего за месяц прошло:" << endl;
    results << sells.size() + rents.size() << " сделок, из них:" << endl;
    results << sells.size() << " продаж (в среднем " << (sells.size() / 31.) << " продаж в день)" << endl;
    results << rents.size() << " прокатов (в среднем " << (rents.size() / 31.) << " продаж в день)" << endl;

    int quantityStoleRents = 0;
    double estimatedStolenPrice = 0;
    vector <string> Thiefs;
    for (int i = 0; i < rents.size(); i++) {
        if (!rents[i].wasReturned()) {
            int rentDay = SkiShop::stringToInt(rents[i].getDay());
            int rentalDays = rents[i].getRentDays();
            int returnDay = rentDay + rentalDays;

            if (returnDay <= 31) {
                quantityStoleRents += 1;
                Thiefs.push_back(rents[i].getName() + ' ' + rents[i].getPassport());
                estimatedStolenPrice += rents[i].getPrice() * 5;
            }
        }
    }
    results << setprecision(2) << endl;
    results << "Из " << rents.size() << " прокатов " << quantityStoleRents << " не было возвращено (" << (quantityStoleRents + 0.) / rents.size() * 100 << "%)." << endl;
    results << "Стоимость украденного имущества: " << estimatedStolenPrice << endl;
    results << "Список клиентов, не вернувших арендованное снаряжение:" << endl;
    for (int i = 0; i < Thiefs.size(); i++) {
        results << Thiefs[i] << endl;
    }
}

void generateCatalog(int quantity) {
    ofstream file("catalog.txt");

    if (!file.is_open()) {
        cout << "Error! File was not open." << endl;
    }

    string brands[5] = { "Atomic", "K2", "Fischer", "HEAD", "Rossignol" };
    string models[10] = { "Sky9", "ProCarve", "GlideSnow", "Olimp", "Race11", "Speed", "Pursuit", "Vantage", "Progressor", "Cloud" };
    
    for (int i = 0; i < quantity; i++) {
        string type = (random(1, 100) > 50) ? "Ski" : "Snowboard";
        string brand = brands[random(0, 4)];
        string model = models[random(0, 9)];
        string level = (random(1, 100) < 33) ? "Beginner" : ((random(1, 100) > 50) ? "Medium" : "Expert");
        int quantity = random(1, 10);
        int serialNumber = random(10000, 99999);

        file << type << " " << brand << " " << model << " " << level << " " << quantity << " " << serialNumber << endl;
    }

    file.close();
}

void insertCatalog(vector<Ski> &ski, vector<Snowboard> &snowboard) {
    ifstream file("catalog.txt");

    if (!file.is_open()) {
        cout << "Error! File was not open." << endl;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string word; 
        iss >> word;
        if (word == "Ski") {
            iss >> word;
            string brand = word;
            iss >> word;
            string model = word;
            iss >> word;
            string level = word;
            iss >> word;
            int quantity = stoi(word);
            iss >> word;
            int serialNumber = stoi(word);
            ski.push_back({ brand, model, level, quantity, serialNumber, "ski" });
        } else if (word == "Snowboard") {
            iss >> word;
            string brand = word;
            iss >> word;
            string model = word;
            iss >> word;
            string level = word;
            iss >> word;
            int quantity = stoi(word);
            iss >> word;
            int serialNumber = stoi(word);
            snowboard.push_back({ brand, model, level, quantity, serialNumber, "snowboard" });
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    //generateCatalog(10);
    vector<Ski> SkiCatalog;
    vector<Snowboard> SnowboardCatalog;
    insertCatalog(SkiCatalog, SnowboardCatalog);
    vector <Sell> sells;
    vector <Rental> rents;
    Simulation(SkiCatalog, SnowboardCatalog, sells, rents);

    file.close();
    CalculateResults(SkiCatalog, SnowboardCatalog, sells, rents);
    results.close();
    return 0;
}

