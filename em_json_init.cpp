#include "em_json.h"

void EMJson::init_lexer() {
    TokenDFA str;
    str.start_state = "0";
    str.ignore = false;
    str.token_identifier = EMJ_STRING;
    str.dfa["0"]['\"'] = "1";
    str.add_ASCII_range("1", char(0), char(127), "1");
    str.dfa["1"]['\"'] = "2";
    str.dfa["1"]['\\'] = "3";
    str.add_ASCII_range("3", char(0), char(127), "1");
    str.add_final_state("2");
    lexer.insert_token(str);
    
    TokenDFA num;
    num.start_state = "0";
    num.token_identifier = EMJ_NUMBER;
    num.ignore = false;
    num.add_ASCII_range("0", '0', '9', "1");
    num.add_ASCII_range("1", '0', '9', "1");;
    num.dfa["1"]['.'] = "2";
    num.add_ASCII_range("2", '0', '9', "3");
    num.add_ASCII_range("3", '0', '9', "3");
    num.add_final_state("1");
    num.add_final_state("3");
    lexer.insert_token(num);


    lexer.create_word_token("null" , (int)EMJ_NULL); // Changed NULL to null
    lexer.create_word_token("[" , (int)EMJ_OP_BRAKET);
    lexer.create_word_token("]" , (int)EMJ_CLOSE_BRAKET);
    lexer.create_word_token("{" , (int)EMJ_OP_BRACE);
    lexer.create_word_token("}" , (int)EMJ_CLOSE_BRACE);
    lexer.create_word_token("true" , (int)EMJ_TRUE);
    lexer.create_word_token("false" , (int)EMJ_FALSE);
    lexer.create_word_token(":", (int)EMJ_DOUBLE_DOT);
    lexer.create_word_token(",", (int)EMJ_CAMMA);

    lexer.create_word_token(" " , (int)EMJ_IGNORES , true);
    lexer.create_word_token("\n" , (int)EMJ_IGNORES , true);
    lexer.create_word_token("\t" ,(int)EMJ_IGNORES , true);
}

void EMJson::init_dfaction() {
    this->machine = &main_obj;
    DFA object_handler;
    DFA array_handler;
    DFA string_handler;
    DFA number_handler;
    DFA boolean_null_handler;

    DFA type_check;
    DFA key_handler;

    this->add_special_dfa(EMJ_OBJECT_HANDLER , object_handler);
    this->add_special_dfa(EMJ_BOOLEAN_NULL_HANDLER , boolean_null_handler);
    this->add_special_dfa(EMJ_TYPE_CHECK , type_check);
    this->add_special_dfa(EMJ_ARRAY_HANDLER , array_handler);
    this->add_special_dfa(EMJ_NUMBER_HANDLER , number_handler);
    this->add_special_dfa(EMJ_STRING_HANDLER , string_handler);
    this->add_special_dfa(EMJ_KEY_HANDLER , key_handler);
}

EMJson::EMJson() {
    init_lexer();
    init_dfaction();
}