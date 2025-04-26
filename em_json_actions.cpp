#include "em_json.h"

DFActionFlow EMJson::action_function(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , DFActionState& current_state
    , bool& go_next_index
) {


    switch (current_state)
    {
    case EMJ_KEY_HANDLER:
        return emj_key_handler(index_in_tokens , tokens , go_next_index);
    case EMJ_TYPE_CHECK:
        return emj_type_check(index_in_tokens , tokens , go_next_index);
    case EMJ_BOOLEAN_NULL_HANDLER:
        return emj_boolean_null_handler(index_in_tokens , tokens , go_next_index);
    case EMJ_NUMBER_HANDLER:
        return emj_number_handler(index_in_tokens , tokens , go_next_index);
    case EMJ_STRING_HANDLER:
        return emj_string_handler(index_in_tokens , tokens , go_next_index);
    case EMJ_OBJECT_HANDLER:
        return emj_object_handler(index_in_tokens , tokens , go_next_index);
    case EMJ_MAIN_OBJ:
        return emj_main_obj(index_in_tokens , tokens , go_next_index);
    case EMJ_ARRAY_HANDLER:
        return emj_array_handler(index_in_tokens , tokens , go_next_index);
        
    default:
        break;
    }

    return {DFACTION_SAFE , DFActionState(0)};
}


DFActionFlow EMJson::emj_type_check(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index) {

    auto& token = tokens[index_in_tokens];
    go_next_index = false;

    switch (token.type)
    {
    case EMJ_FALSE:
    case EMJ_TRUE:
    case EMJ_NULL:
        return {DFACTION_GO_TO_SP_DFA , EMJ_BOOLEAN_NULL_HANDLER};

    case EMJ_NUMBER:
        return {DFACTION_GO_TO_SP_DFA , EMJ_NUMBER_HANDLER};
    
    case EMJ_STRING:
        return {DFACTION_GO_TO_SP_DFA , EMJ_STRING_HANDLER};
    
    case EMJ_OP_BRACE:
        return {DFACTION_GO_TO_SP_DFA , EMJ_OBJECT_HANDLER};
    
    case EMJ_OP_BRAKET:
        return {DFACTION_GO_TO_SP_DFA , EMJ_ARRAY_HANDLER};
    
    case EMJ_DOUBLE_DOT:
    case EMJ_CAMMA:
    case EMJ_CLOSE_BRACE:
    case EMJ_CLOSE_BRAKET:
        return {DFACTION_BACK_TO_PREV , DFActionState(0)};
    }
    
    std::cout << token.value << "\n";
    return {DFACTION_PANIC, DFActionState(0)}; // Added default return for unhandled cases
}

DFActionFlow EMJson::emj_boolean_null_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index){
    
    auto& token = tokens[index_in_tokens];

    auto field_name = field_stack.back();
    field_stack.pop_back();

    auto& obj_wrapper = obj_stack.back();

    if (field_name != "") {

        auto& obj = std::get<EMJsonObject>(obj_wrapper.value);
    
        obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_BOOLEAN;
    
        switch (token.type)
        {
        case EMJ_TRUE:
            obj[field_name] = true;
            break;
        case EMJ_FALSE:
            obj[field_name] = false;
            break;
        case EMJ_NULL:
            obj[field_name] = nullptr;
            obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_NULLVAL;
            break;
        }

    } else {
        auto& obj = std::get<EMJsonArray>(obj_wrapper.value);

    
        switch (token.type)
        {
        case EMJ_TRUE:
        obj.push_back(true);
            break;
        case EMJ_FALSE:
        obj.push_back(false);
            break;
        case EMJ_NULL:
            obj.push_back(nullptr);
            break;
        }
    }

    return {DFACTION_BACK_TO_PREV , DFActionState(0)};
}

DFActionFlow EMJson::emj_number_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index) {
    
    auto& token = tokens[index_in_tokens];

    auto field_name = field_stack.back();
    field_stack.pop_back();

    auto& obj_wrapper = obj_stack.back();


    if (field_name != "") {
        auto& obj = std::get<EMJsonObject>(obj_wrapper.value);

        obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_NUMBER;
    
        obj[field_name].value = atof(std::string(token.value).c_str());
    } else {
        auto& obj = std::get<EMJsonArray>(obj_wrapper.value);


        obj.push_back(atof(std::string(token.value).c_str()));
    }

    return {DFACTION_BACK_TO_PREV , DFActionState(0)};
}

DFActionFlow EMJson::emj_string_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index){
    
    auto& token = tokens[index_in_tokens];

    auto field_name = field_stack.back();
    field_stack.pop_back();

    auto& obj_wrapper = obj_stack.back();
    if (field_name != "") {
        auto& obj = std::get<EMJsonObject>(obj_wrapper.value);

        obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_STRING;

        obj[field_name].value = std::string(token.value).substr(1, token.value.length() - 2); 
    } else {
        auto& obj = std::get<EMJsonArray>(obj_wrapper.value);


        obj.push_back(std::string(token.value).substr(1, token.value.length() - 2));
    }

    return {DFACTION_BACK_TO_PREV , DFActionState(0)};
}

DFActionFlow EMJson::emj_object_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index) {
    
    auto& token = tokens[index_in_tokens];
    

    switch (token.type)
    {
    case EMJ_OP_BRACE:
        obj_stack.push_back(EMJsonData{EMJsonObject()});
        
        return {DFACTION_GO_TO_SP_DFA , EMJ_KEY_HANDLER};
    case EMJ_CAMMA:
        return {DFACTION_GO_TO_SP_DFA , EMJ_KEY_HANDLER};
    case EMJ_DOUBLE_DOT:
        return {DFACTION_GO_TO_SP_DFA , EMJ_TYPE_CHECK};
    case EMJ_CLOSE_BRACE:{
        auto field_name = field_stack.back();
        field_stack.pop_back();



        auto nest_obj_wrapper = obj_stack.back();
        auto& nest_obj = std::get<EMJsonObject>(nest_obj_wrapper.value);
        obj_stack.pop_back();

        
        auto& obj_wrapper = obj_stack.back();


        if (field_name != "") {
            auto& obj = std::get<EMJsonObject>(obj_wrapper.value);
            obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_OBJECT;
        
            obj[field_name] = nest_obj;
        } else {
            auto& obj = std::get<EMJsonArray>(obj_wrapper.value);


            obj.push_back(nest_obj);
        }
        return {DFACTION_BACK_TO_PREV , DFActionState(0)};
    }
    }

    return {DFACTION_PANIC, DFActionState(0)}; // Added default return for unhandled cases
}

DFActionFlow EMJson::emj_main_obj(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index) {


    auto& token = tokens[index_in_tokens];

    

    switch (token.type)
    {
    case EMJ_OP_BRACE:
        return {DFACTION_GO_TO_SP_DFA , EMJ_KEY_HANDLER};
    case EMJ_CAMMA:
        return {DFACTION_GO_TO_SP_DFA , EMJ_KEY_HANDLER};
    case EMJ_DOUBLE_DOT:
        return {DFACTION_GO_TO_SP_DFA , EMJ_TYPE_CHECK};
    case EMJ_CLOSE_BRACE:{
        return {DFACTION_COMPILE_DONE , DFActionState(0)};
    }
    }

    std::cout << "goz\n";
    return {DFACTION_PANIC, DFActionState(0)}; // Added default return for unhandled cases
}

DFActionFlow EMJson::emj_key_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index){
    
    auto& token = tokens[index_in_tokens];

    switch(token.type) {
    case EMJ_STRING: {
        auto field_name = std::string(std::string(token.value).substr(1, token.value.length() - 2));
        field_stack.push_back(field_name);

        return {DFACTION_BACK_TO_PREV , DFActionState(0)};
    }
    default:
        return {DFACTION_PANIC , DFActionState(0)};
    }
}


DFActionFlow EMJson::emj_array_handler(
    size_t& index_in_tokens
    , const std::vector<DFActionToken>& tokens
    , bool& go_next_index) {

    auto& token = tokens[index_in_tokens];

    switch (token.type)
    {
    case EMJ_OP_BRAKET:
        field_stack.push_back("");
        obj_stack.push_back(EMJsonArray{});
        
        return {DFACTION_GO_TO_SP_DFA , EMJ_TYPE_CHECK};
    case EMJ_CAMMA:
        field_stack.push_back("");
            
        return {DFACTION_GO_TO_SP_DFA , EMJ_TYPE_CHECK};
    case EMJ_CLOSE_BRAKET: {
        auto field_name = field_stack.back();
        field_stack.pop_back();



        auto nest_obj_wrapper = obj_stack.back();
        auto& nest_obj = std::get<EMJsonArray>(nest_obj_wrapper.value);
        obj_stack.pop_back();

        
        auto& obj_wrapper = obj_stack.back();
        auto& obj = std::get<EMJsonObject>(obj_wrapper.value);

    
        obj[field_name].type = EMJsonDataTypes::EMJ_TYPE_ARRAY;
    
        obj[field_name] = nest_obj;
        
        return {DFACTION_BACK_TO_PREV , DFActionState(0)};
    }
    }

    return {DFACTION_PANIC , DFActionState(0)};
}