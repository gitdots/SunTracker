#include "Database.hpp"

#include <sqlite3.h>
#include <iostream>
using namespace std;

Database::Database() {
    int r = sqlite3_open("test.db", &db);
    if(r != SQLITE_OK) {
        cout << "Failed to open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
    }
    cout << "Database created succesfully" << endl;
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::connect() {

}