#pragma once
#ifndef DFMATCHER
#define DFMATCHER

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>


enum DFMatcherResCode {
	SUCCESS,
	END_OF_FILE,
	NO_MATCHED_TOKEN,
};


struct TokenDFA {
	std::unordered_map<std::string, std::unordered_map<char, std::string>> dfa;
	std::string start_state;
	std::unordered_set<std::string> final_states;
	bool ignore = false;
    int token_identifier;


	void add_ASCII_range(std::string state, char start, char end, std::string next_state) {
		TokenDFA& token = *this;
		char tmp_start = start;

		for (; start <= end && start >= tmp_start; ++start) {
			token.dfa[state][start] = next_state;
		}
	}

	void add_final_state(const std::string& state_name) {
		final_states.insert(state_name);
	}
};

struct DFMatcherRes {
	DFMatcherResCode status;
    int token_identifier;
	std::string_view value;
};

/// <summary>
/// the last token added = higher priority
/// </summary>
class DFMatcher {
	std::vector<TokenDFA> tokens;

public:
	/// <summary>
	/// with this function you insert TokenDFA to the engine
	/// </summary>
	/// <param name="token"></param>
	/// <returns>index of token in the engine</returns>
	size_t insert_token(const TokenDFA& token) {
		size_t res = tokens.size();
		this->tokens.push_back(token);

		return res;
	}

	/// <summary>
	/// this function is used to free the token engine for adding new tokens
	/// from scratch
	/// </summary>
	void free_tokens() {
		tokens.clear();
	}

	/// <summary>
	/// by using this function instead of providing dfa rep of the token , you provide the 
	/// word and it will gen the dfa itself
	/// </summary>
	/// <param name="word"></param>
	/// <param name="ignore"> tells the engine do not report this token </param>
	/// <param name="token_name"></param>
	/// <returns>index of token in the engine</returns>
	size_t create_word_token(const std::string& word,   int token_identi, bool ignore = false) {
		size_t res = tokens.size();

		TokenDFA token;

		std::string base_state_name = "state_";
		token.start_state = "state_0";
		token.final_states = std::unordered_set<std::string>{ word };

		for (size_t i{ 0 }; i < word.length(); ++i) {
			if (i == word.length() - 1) {
				token.dfa[base_state_name + std::to_string(i)][word[i]] = word;

				continue;
			}

			token.dfa[base_state_name + std::to_string(i)][word[i]] = base_state_name + std::to_string(i + 1);
		}

		token.ignore = ignore;
		token.token_identifier = token_identi;
		tokens.push_back(token);


		return res;
	}

	/// <summary>
	/// you insert the token dfa as a string and the system will gen TokenDFA of it
	/// </summary>
	/// <param name="token_dfa"></param>
	/// <param name="finals"></param>
	/// <param name="ignore"></param>
	/// <returns> index of the token in the engine </returns>
	size_t insert_token_as_str(const std::string& token_dfa
		, std::unordered_set<std::string> finals
		,   int token_ident
		, bool ignore = false) {

		std::string state_name;
		char ch;
		std::string next_state;

		std::istringstream extracter{ token_dfa };

		extracter >> state_name >> ch >> next_state;


		TokenDFA dfa;
		dfa.start_state = state_name;
		dfa.final_states = finals;

		dfa.dfa[state_name][ch] = next_state;

		while (!extracter.eof()) {
			extracter >> state_name >> ch >> next_state;

			if (!extracter.fail()) {
				dfa.dfa[state_name][ch] = next_state;
			}
			else {
				return -1;
			}
		}

		dfa.ignore = ignore;
		dfa.token_identifier = token_ident;

		size_t res = tokens.size();

		tokens.push_back(dfa);

		return res;
	}

	/// <summary>
	/// returns the token final state it seen
	/// </summary>
	/// <returns>token name if "" means no matching token</returns>
	inline DFMatcherRes get_token(
		const std::string& buffer
		, size_t& buffer_index) {

		DFMatcherRes result;

		while (true) {

			size_t max_len = 0;
		    int max_token_ident =  int(0);
			size_t token_ind = 0;

			for (size_t token_index{ 0 }; token_index < tokens.size(); ++token_index) {
				std::string state = tokens[token_index].start_state;
				TokenDFA& token = tokens[token_index];


				size_t len = 0;

				for (size_t buf_index{ buffer_index }; buf_index < buffer.length(); ++buf_index) {
					if (token.dfa[state].find(buffer[buf_index]) == token.dfa[state].end()) {
						break;
					}

					state = token.dfa[state][buffer[buf_index]];
					++len;
				}

				if (token.final_states.find(state) != token.final_states.end()) {
					if (len >= max_len) {
						max_len = len;
						max_token_ident = token.token_identifier;
						token_ind = token_index;
					}
				}
			}

			if (max_len == 0 && buffer_index < buffer.length()) {
				return DFMatcherRes{NO_MATCHED_TOKEN , -1 , 0};
            }

			std::string_view max_view(buffer.data() + buffer_index , max_len);
			buffer_index += max_len;

			if (tokens[token_ind].ignore && buffer_index < buffer.length()) {
				continue;
			}


			if (buffer_index >= buffer.length()) {
				result.status = END_OF_FILE;
			}
			else {
				result.status = SUCCESS;
			}

			result.token_identifier = max_token_ident;
			result.value = max_view;

			return result;
		}
	}
};


#endif // !DFMATCHER