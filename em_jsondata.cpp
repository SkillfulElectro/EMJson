#include "em_jsondata.h"
#include <iostream>

static void printIndent(std::ostream& os, int indent) {
    for (int i = 0; i < indent; ++i) {
        os << "  ";
    }
}

static std::ostream& printJsonData(std::ostream& os, const EMJsonData& data, int indent = 0) {
    switch (data.type) {
        case NUMBER:
            os << std::get<double>(data.value);
            break;
        case STRING:
            os << '"' << std::get<std::string>(data.value) << '"';
            break;
        case BOOLEAN:
            os << (std::get<bool>(data.value) ? "true" : "false");
            break;
        case NULLVAL:
            os << "null";
            break;
        case ARRAY: {
            const EMJsonArray& arr = std::get<EMJsonArray>(data.value);
            os << "[\n";
            for (size_t i = 0; i < arr.size(); ++i) {
                printIndent(os, indent + 1);
                printJsonData(os, arr[i], indent + 1);
                if (i != arr.size() - 1) {
                    os << ",";
                }
                os << "\n";
            }
            printIndent(os, indent);
            os << "]";
            break;
        }
        case OBJECT: {
            const EMJsonObject& obj = std::get<EMJsonObject>(data.value);
            os << "{\n";
            size_t count = 0;
            for (const auto& [key, val] : obj) {
                printIndent(os, indent + 1);
                os << '"' << key << "\": ";
                printJsonData(os, val, indent + 1);
                if (count != obj.size() - 1) {
                    os << ",";
                }
                os << "\n";
                ++count;
            }
            printIndent(os, indent);
            os << "}";
            break;
        }
        default:
            os << "unknown";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const EMJsonData& data) {
    return printJsonData(os, data, 0);
}
