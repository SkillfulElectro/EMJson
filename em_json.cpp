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

void EMJson::reset_parser_state() {
    obj_stack.clear();
    field_stack.clear();
    tokens.clear();
    index_tk = 0;
    ctx = get_default_context(EMJ_MAIN_OBJ);
}

EMJsonParsedVal EMJson::parse(const std::string& json , size_t index_file) {

    size_t index_in_file = index_file;

    DFMatcherRes res;

    DFActionReturnVal comp_res;

    do {
        res = lexer.get_token(json, index_in_file);

        if (res.status == NO_MATCHED_TOKEN) {
            return {
                EMJ_MORE_INPUT_OR_UNKNOWN_CHAR , 
                index_in_file ,
                &obj_stack,
                &parsed_len
            };
        }

        DFActionToken tok;
        tok.value = res.value;
        tok.type = (DFActionType) res.token_identifier;

        tokens.push_back(tok);
    } while (res.status != END_OF_FILE);



    for (;index_tk < tokens.size();) {


        comp_res = this->run_dfa_on(tokens , EMJ_MAIN_OBJ , &index_tk , &ctx);



        if (comp_res.status == PANIC_WHILE_PROCESSING) {
            return {
                EMJ_JSON_SYNTAX_ERROR , 
                index_in_file ,
                &obj_stack,
                &parsed_len
            };
        } else if (comp_res.status == ALL_REDUCTIONS_ARE_COMPLETED) {
            tokens.erase(tokens.begin() , tokens.begin() + index_tk);
            parsed_len += 1;
            index_tk = 0;

            ctx = get_default_context(EMJ_MAIN_OBJ);
        }
    }



    return {
        EMJ_SUCCESS , 
        index_in_file ,
        &obj_stack,
        &parsed_len
    };
}