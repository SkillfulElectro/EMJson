// em_json.h
#ifndef EM_JSON
#define EM_JSON

#include "em_jsondata.h"
#include <string>
#include <iostream> // Ensure iostream is included for logging

#include "em_jsontypes.h"

#include "sys/DFAction.h"
#include "sys/DFMatcher.h"

class EMJson : public DFAction {
    DFMatcher lexer;
    void init_lexer();

    DFA main_obj;
    void init_dfaction();

    std::vector<EMJsonData> obj_stack;
    std::vector<std::string> field_stack;

protected:

	DFActionFlow action_function(
		size_t& index_in_tokens
		, const std::vector<DFActionToken>& tokens
		, DFActionState& current_state
		, bool& go_next_index
	) override;

    // actions
private:
    DFActionFlow emj_type_check(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_boolean_null_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_number_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_string_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_object_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_key_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_main_obj(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

    DFActionFlow emj_array_handler(
        size_t& index_in_tokens
        , const std::vector<DFActionToken>& tokens
        , bool& go_next_index);

private:
    static std::string serialize_value(const EMJsonData& data);

public:
    EMJson();

    EMJsonData parse(const std::string& json);

    static std::string to_json(const EMJsonData& data);
};

#endif // EM_JSON