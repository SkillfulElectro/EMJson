//em_jsondata.h
#ifndef EM_JSONDATA
#define EM_JSONDATA

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream> // for std::ostream

enum EMJsonDataTypes {
    NUMBER,
    STRING,
    ARRAY,
    OBJECT,
    NULLVAL,
    BOOLEAN,
};

struct EMJsonData;

using EMJsonObject = std::unordered_map<std::string , EMJsonData>;

using EMJsonArray = std::vector<EMJsonData>;

using EMJsonDataValue = std::variant<double , std::string ,
                                     EMJsonArray , bool , EMJsonObject , std::nullptr_t>; 

struct EMJsonData {
    EMJsonDataTypes type;
    EMJsonDataValue value;

    EMJsonData() : type(NULLVAL), value(nullptr) {}
    EMJsonData(double v) : type(NUMBER), value(v) {}
    EMJsonData(const std::string& v) : type(STRING), value(v) {}
    EMJsonData(const char* v) : type(STRING), value(std::string(v)) {} 
    EMJsonData(const EMJsonArray& v) : type(ARRAY), value(v) {}
    EMJsonData(bool v) : type(BOOLEAN), value(v) {}
    EMJsonData(const EMJsonObject& v) : type(OBJECT), value(v) {}
    EMJsonData(std::nullptr_t) : type(NULLVAL), value(nullptr) {}
};

// Overload operator<< for printing EMJsonData
std::ostream& operator<<(std::ostream& os, const EMJsonData& data);

#endif
