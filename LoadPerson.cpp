// Дано:
#if 0
// Функция 1
// Дана функция LoadPerson.
// Оцените, насколько удобно ей пользоваться, тестировать, поддерживать, и выберите наиболее
// подходящий способ рефакторинга. Реализуйте выбранный способ и сохраните решение в файл LoadPerson.cpp.
vector<Person> LoadPersons(string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
                           DBLogLevel db_log_level, int min_age, int max_age, string_view name_filter) {
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
#endif

// Реинжиниринг функционала (перечень значений задан произвольно, для примера):
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>

using namespace std;
using namespace literals;

struct Person {
    std::string name;
    int age;
};

enum DBLogLevel {
    ONE,
    TWO
};

struct DBQuery {
    DBQuery(const string& str) {

    }
};

struct DBHandler {
    bool IsOK() const {
        return true;
    }
    string_view Quote(string_view name_filter) {
        return name_filter;
    }
    template<typename S, typename I>
    map<S, I> LoadRows(DBQuery query) {
        map<S, I> m;
        m["s1"] = 1;
        m["s2"] = 2;
        m["s3"] = 3;
        return m;
    }
};

struct DBConnector {
    DBConnector(bool db_allow_exceptions, DBLogLevel db_log_level) {
    }
    DBHandler ConnectTmp(string_view db_name, int db_connection_timeout) {
        return DBHandler();
    }
    DBHandler Connect(string_view db_name, int db_connection_timeout) {
        return DBHandler();
    }
};

// Группируем:
struct DBSettings {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    string file_filter = "tmp."s;
    bool isTmp() const {
        return db_name.starts_with(file_filter);
    }
};

struct DBQueryParameters {
    int min_age;
    int max_age;
    string_view name_filter;
};

std::optional<DBHandler> BuildDB(const DBSettings& settings) {
    DBConnector connector(settings.db_allow_exceptions, settings.db_log_level);
    DBHandler db;
    if (settings.isTmp()) {
        db = connector.ConnectTmp(settings.db_name,
                                  settings.db_connection_timeout);
    } else {
        db = connector.Connect(settings.db_name,
                               settings.db_connection_timeout);
    }
    if (!settings.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }
    return db;
}

vector<Person> LoadPersons(const DBSettings& settings, const DBQueryParameters &parameters) {
    // create DB:
    auto db = BuildDB(settings);
    if(db == nullopt) {
        return {};
    }
    // create query:
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << parameters.min_age << " and "s << parameters.max_age << " "s
              << "and Name like '%"s << db->Quote(parameters.name_filter) << "%'"s;
    // create vector:
    vector<Person> persons;
    for (auto [name, age] : db->LoadRows<string, int>(DBQuery(query_str.str()))) {
        persons.push_back({move(name), age});
    }
    return persons;
}
