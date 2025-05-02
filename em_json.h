#ifndef EM_JSON
#define EM_JSON

#include "em_jsondata.h"
#include <string>

#include "sys/DFAction.h"
#include "sys/DFMatcher.h"

class EMJson : public DFAction {
    DFMatcher lexer;
    void init_lexer();

    DFA main_obj;
    void init_dfaction();

    std::vector<EMJsonData> obj_stack;
    size_t parsed_len;

    std::vector<std::string> field_stack;

    std::vector<DFActionToken> tokens;
    size_t index_tk;

    DFActionContext ctx;

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

    EMJsonParsedVal parse(const std::string& json , size_t index_file = 0);

    void reset_parser_state();

    static std::string to_json(const EMJsonData& data);
};

#endif // EM_JSON