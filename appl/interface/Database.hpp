#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <memory>
class Database {

    public:
        Database();
        ~Database();
        void connect();

    private:
        sqlite3 *db;
};

#endif