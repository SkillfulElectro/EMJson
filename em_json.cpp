#include "em_json.h"

std::string EMJson::to_json(const EMJsonData& data){
    // Check if the top-level data is an object
    if (data.type != OBJECT) {
         // Return empty string if top-level isn't an object, as per original design
         return "";
    }

    std::string json_str = "{";
    bool first_pair = true;

    try { // Add try-catch here for safety, though less likely to be needed now
        const auto& obj = std::get<EMJsonObject>(data.value);

        for (const auto& pair : obj) {
            if (!first_pair) {
                json_str += ",";
            }
            // Basic escaping for the key string
            std::string escaped_key;
             for (char c : pair.first) {
                if (c == '"') {
                    escaped_key += "\\\"";
                } else if (c == '\\') {
                    escaped_key += "\\\\";
                } else {
                    escaped_key += c;
                }
            }
            json_str += "\"" + escaped_key + "\":";

            // Serialize the value using the helper function
            json_str += serialize_value(pair.second);

            first_pair = false;
        }
    } catch (const std::bad_variant_access& e) {
         // Handle cases where std::get fails (type mismatch)
         // This shouldn't happen if the outer check (data.type == OBJECT) passes
         // But adding for robustness. We append an error message inside the object.
         if (!first_pair) json_str += ",";
         json_str += "\"<ERROR:TopLevelVariantAccess>\":\"true\"";
    }


    json_str += '}';
    return json_str;
}

std::string EMJson::serialize_value(const EMJsonData& data) {
    std::stringstream ss;

    try { // Add try-catch here to handle potential bad_variant_access internally
        switch (data.type) {
            case NUMBER:
                ss << std::get<double>(data.value);
                return ss.str();

            case STRING:
                // Basic escaping for quotes needed for valid JSON output
                {
                    std::string escaped_str;
                    std::string original_str = std::get<std::string>(data.value);
                    for (char c : original_str) {
                        if (c == '"') {
                            escaped_str += "\\\"";
                        } else if (c == '\\') {
                            escaped_str += "\\\\";
                        } else {
                            escaped_str += c;
                        }
                    }
                    return "\"" + escaped_str + "\"";
                }


            case BOOLEAN:
                return std::get<bool>(data.value) ? "true" : "false";

            case NULLVAL:
                return "null";

            case ARRAY: {
                std::string arr_str = "[";
                const auto& vec = std::get<EMJsonArray>(data.value);
                bool first_element = true;
                for (const auto& element : vec) {
                    if (!first_element) {
                        arr_str += ",";
                    }
                    arr_str += serialize_value(element); // Recursive call
                    first_element = false;
                }
                arr_str += "]";
                return arr_str;
            }

            case OBJECT:
                // Recursive call to the main function for nested objects
                return to_json(data);

            default:
                // Return an error indicator string instead of throwing
                return "\"<ERROR:UnknownDataType>\"";
        }
    } catch (const std::bad_variant_access& e) {
        // Handle cases where std::get fails (type mismatch)
        return "\"<ERROR:VariantAccess>\"";
    }
}

EMJsonData EMJson::parse(const std::string& json) {


    obj_stack.push_back(EMJsonObject{});

    size_t index = 0;

    DFMatcherRes res;

    std::vector<DFActionToken> tokens;


    do {
        res = lexer.get_token(json, index);


        DFActionToken tok;
        tok.value = res.value;
        tok.type = (DFActionType) res.token_identifier;



        tokens.push_back(tok);
    } while (res.status != END_OF_FILE);



    auto comp_res = this->run_dfa_on(tokens, EMJ_MAIN_OBJ);



    if (comp_res.status == FAILED_TO_DO_ALL_REDUCTIONS) {
        std::cout << "COMPILATION FAILED !\n";
    }

    return obj_stack.back();
}