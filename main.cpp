#include <iostream>
#include "em_json.h" 

int main() {
    // parser instance
    EMJson parser;

    std::string j_file = R"({"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,{"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"})";

    auto res = parser.parse(j_file);

    auto& obj = *res.get_object();

    std::cout << (*obj["version"].get_double()) << "\n";

    std::cout << EMJson::to_json(res) << std::endl;

    return 0;
}
