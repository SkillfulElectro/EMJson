#include "em_json.h"

std::string EMJson::to_json(const EMJsonData& data){
    if (data.type != EMJ_TYPE_OBJECT) {
         return "";
    }

    std::string json_str = "{";
    bool first_pair = true;

    try {
        const auto& obj = std::get<EMJsonObject>(data.value);

        for (const auto& pair : obj) {
            if (!first_pair) {
                json_str += ",";
            }
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

            json_str += serialize_value(pair.second);

            first_pair = false;
        }
    } catch (const std::bad_variant_access& e) {
         if (!first_pair) json_str += ",";
         json_str += "\"<ERROR:TopLevelVariantAccess>\":\"true\"";
    }


    json_str += '}';
    return json_str;
}

std::string EMJson::serialize_value(const EMJsonData& data) {
    std::stringstream ss;

    try {
        switch (data.type) {
            case EMJ_TYPE_NUMBER:
                ss << std::get<double>(data.value);
                return ss.str();

            case EMJ_TYPE_STRING:
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


            case EMJ_TYPE_BOOLEAN:
                return std::get<bool>(data.value) ? "true" : "false";

            case EMJ_TYPE_NULLVAL:
                return "null";

            case EMJ_TYPE_ARRAY: {
                std::string arr_str = "[";
                const auto& vec = std::get<EMJsonArray>(data.value);
                bool first_element = true;
                for (const auto& element : vec) {
                    if (!first_element) {
                        arr_str += ",";
                    }
                    arr_str += serialize_value(element);
                    first_element = false;
                }
                arr_str += "]";
                return arr_str;
            }

            case EMJ_TYPE_OBJECT:
                return to_json(data);

            default:
                return "\"<ERROR:UnknownDataType>\"";
        }
    } catch (const std::bad_variant_access& e) {
        return "\"<ERROR:VariantAccess>\"";
    }
}

EMJsonData EMJson::parse(const std::string& json) {
    obj_stack.clear();
    field_stack.clear();

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