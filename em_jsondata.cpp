#include "em_jsondata.h"

double* EMJsonData::get_double() {
    return std::get_if<double>(&value);
}

const double* EMJsonData::get_double() const {
    return std::get_if<double>(&value);
}

std::string* EMJsonData::get_string() {
    return std::get_if<std::string>(&value);
}

const std::string* EMJsonData::get_string() const {
    return std::get_if<std::string>(&value);
}

EMJsonArray* EMJsonData::get_array() {
    return std::get_if<EMJsonArray>(&value);
}

const EMJsonArray* EMJsonData::get_array() const {
    return std::get_if<EMJsonArray>(&value);
}

bool* EMJsonData::get_bool() {
    return std::get_if<bool>(&value);
}

const bool* EMJsonData::get_bool() const {
    return std::get_if<bool>(&value);
}

EMJsonObject* EMJsonData::get_object() {
    return std::get_if<EMJsonObject>(&value);
}

const EMJsonObject* EMJsonData::get_object() const {
    return std::get_if<EMJsonObject>(&value);
}

bool EMJsonData::is_null() const {
    return std::holds_alternative<std::nullptr_t>(value);
}
