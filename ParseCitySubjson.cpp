#if 0
// Функция 3
// Оцените, насколько удобно пользоваться, тестировать и поддерживать функцию ParseCitySubjson,
// выберите наиболее подходящий способ рефакторинга и переделайте ParseCitySubjson, не меняя функционал.
// При необходимости измените способ вызова этой функции из ParseCountryJson. Сохраните решение в файл ParseCitySubjson.cpp.

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const string& country_name,
                      const string& country_iso_code, const string& country_phone_code, const string& country_time_zone,
                      const vector<Language>& languages) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], country.name, country.iso_code, country.phone_code,
                         country.time_zone, country.languages);
    }
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

using Language = string;

struct City {
    string name;
    string iso_code;
    string phone_code;
    string_view country_name;
    string_view country_iso_code;
    string_view country_time_zone;
    // может быть висячая ссылка, но по другому пока не знаю как.
    vector<Language>& languages;
};

struct Country {
    string name;
    string iso_code;
    string phone_code;
    string time_zone;
    vector<Language> languages;
};

void ParseCitySubjson(vector<City>& cities, const Json::Value& json, const Country& coutry) {
    auto& list_cities = json.AsList();
    cities.reserve(list.size());
    for (const auto& city_json : list_cities) {
        const auto& city_obj = city_json.AsObject();
        cities.emplace_back(move(City{
                                  city_obj["name"s].AsString(),
                                  city_obj["iso_code"s].AsString(),
                                  coutry.phone_code + city_obj["phone_code"s].AsString(),
                                  coutry.name,
                                  coutry.iso_code,
                                  coutry.time_zone,
                                  coutry.languages});
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    const auto& list_country = json.AsList();
    countries.reserve(list_country.size());
    for (const auto& country_json : list_country) {
        const auto& country_obj = country_json.AsObject();
        Country country = {
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString()
        };
        const auto& list_languages = country_obj["languages"s].AsList();
        country.languages.reserve(list_languages.size());
        for (const auto& lang_obj : list_languages) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        countries.emplace_back(country);
        ParseCitySubjson(cities, country_obj["cities"s], countries.back());
    }
}
