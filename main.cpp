// main.cpp
#include <iostream>
#include "em_json.h" // This will also include em_jsondata.h

int main() {
    // Create a sample JSON structure using EMJsonData
    EMJson parser;
    std::string j_file = R"({"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,{"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"})";

    auto res = parser.parse(j_file);


    std::cout << EMJson::to_json(res) << std::endl;
    return 0; // Indicate successful execution
}
