#include <iostream>
#include "em_json.h" 

int main() {
    // parser instance
    EMJson parser;

    std::string j_file = R"({"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,{"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"}{"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,{"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"})";
    std::string j_file2 = R"({"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,{"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"})";

    std::string j_file_parted_1 = R"({"configuration":{"nested_number":456.789,"nested_string":"I am nested!"},"items":[10,"Array element",true,null,)";   
    std::string j_file_parted_2 = R"({"nested_number":456.789,"nested_string":"I am nested!"}],"description":null,"enabled":true,"version":1,"name":"Example JSON"})";   

    auto res = parser.parse(j_file);
    res = parser.parse(j_file2);
    
    res = parser.parse(j_file_parted_1);
    res = parser.parse(j_file_parted_2);
    
    auto& obj = *res.parsed_objects->back().get_object();

    std::cout << (*obj["version"].get_double()) << "\n";
    std::cout << res.parsed_objects->size() << " , " << *res.parsed_len << "\n";
    std::cout << EMJson::to_json(res.parsed_objects->back()) << std::endl;

    return 0;
}
