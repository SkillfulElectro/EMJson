#ifndef EM_JSONDATA
#define EM_JSONDATA

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#define DFACTION_TYPES
#define DFACTION_STATES


enum DFActionState {
    EMJ_TYPE_CHECK = 0,
    EMJ_BOOLEAN_NULL_HANDLER = 1,
    EMJ_OBJECT_HANDLER = 2,
    EMJ_ARRAY_HANDLER = 3,
    EMJ_STRING_HANDLER = 4,
    EMJ_NUMBER_HANDLER = 5,
    EMJ_KEY_HANDLER = 6,
    EMJ_MAIN_OBJ = 7,
};

enum DFActionType {
    EMJ_OP_BRACE ,
    EMJ_CLOSE_BRACE,
    EMJ_STRING,
    EMJ_NUMBER,
    EMJ_OP_BRAKET,
    EMJ_CLOSE_BRAKET,
    EMJ_NULL,
    EMJ_CAMMA,
    EMJ_DOUBLE_DOT,
    EMJ_TRUE,
    EMJ_FALSE,

    EMJ_IGNORES,
};

using DFActionVal = std::string_view;


enum EMJsonDataTypes {
    EMJ_TYPE_NUMBER,
    EMJ_TYPE_STRING,
    EMJ_TYPE_ARRAY,
    EMJ_TYPE_OBJECT,
    EMJ_TYPE_NULLVAL,
    EMJ_TYPE_BOOLEAN,
};

struct EMJsonData;

using EMJsonObject = std::unordered_map<std::string , EMJsonData>;

using EMJsonArray = std::vector<EMJsonData>;

using EMJsonDataValue = std::variant<double , std::string ,
                                     EMJsonArray , bool , EMJsonObject , std::nullptr_t>; 

struct EMJsonData {
    EMJsonDataTypes type;
    EMJsonDataValue value;

    EMJsonData() : type(EMJ_TYPE_NULLVAL), value(nullptr) {}
    EMJsonData(double v) : type(EMJ_TYPE_NUMBER), value(v) {}
    EMJsonData(const std::string& v) : type(EMJ_TYPE_STRING), value(v) {}
    EMJsonData(const char* v) : type(EMJ_TYPE_STRING), value(std::string(v)) {} 
    EMJsonData(const EMJsonArray& v) : type(EMJ_TYPE_ARRAY), value(v) {}
    EMJsonData(bool v) : type(EMJ_TYPE_BOOLEAN), value(v) {}
    EMJsonData(const EMJsonObject& v) : type(EMJ_TYPE_OBJECT), value(v) {}
    EMJsonData(std::nullptr_t) : type(EMJ_TYPE_NULLVAL), value(nullptr) {}

    /// @return null on fail 
    double* get_double();
    const double* get_double() const;

    /// @return null on fail 
    std::string* get_string();
    const std::string* get_string() const;

    /// @return null on fail 
    EMJsonArray* get_array();
    const EMJsonArray* get_array() const;

    /// @return null on fail 
    bool* get_bool();
    const bool* get_bool() const;

    /// @return null on fail 
    EMJsonObject* get_object();
    const EMJsonObject* get_object() const;

    bool is_null() const;
};




#endif
