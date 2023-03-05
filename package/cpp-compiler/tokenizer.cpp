#include <iostream>
#include <memory>
#include <set>
#include "token.cpp"

#ifndef CPP_COMPILER_TOKENIZER
#define CPP_COMPILER_TOKENIZER

struct Position {
    unsigned int row;
    unsigned int col;
    unsigned int index;
};

void initPosition(Position& p) {
    p.col = 0;
    p.row = 0;
    p.index = 0;
}

class Tokenizer {
    private:
        TokenKinds currentToken;
        std::string currentValue;
        std::string code;
        Position position;
        Position startPosition;
        Position endPosition;
    private:
        char get_char() {
            if(position.index >= code.length()) {
                return EOF;
            }
            return code[position.index];
        }
        bool is_eof() {
            return get_char() == EOF;
        }
        bool is_char(char ch) {
            char current = get_char();
            return current == ch;
        }
        bool is_char_set(std::set<char> char_set) {
            char current = get_char();
            return char_set.find(current) != char_set.end();
        }
        std::string eat_char(unsigned int num = 1) {
            std::string value ;
            for(int i = 0 ; i < num; ++i) {
                char currentChar = get_char();
                if(currentChar == EOF)
                    return value;
                value += currentChar;
                if(currentChar == '\n') {
                    position.col = 0 ;
                    position.row ++;
                }else {
                    position.col ++;
                }
                position.index ++;
            }
            return value;
        }
        void skip() {
            while(
                ! is_eof() &&
                (get_char() == ' ' || get_char() == '\t' || is_char('\n'))
            ) {
                eat_char(1);
            }
        }
    public:
        bool start_with(std::string start_str) {
            return code.substr( position.index).rfind(start_str, 0) == 0 ;
        }
        Tokenizer(std::string codeStr) {
            code = codeStr;
            currentToken = TokenKinds::StartState;
            currentValue = "";
            initPosition(position);
            initPosition(startPosition);
            initPosition(endPosition);
        }
        Position get_start_poistion() {
            return startPosition;
        }
        Position get_end_position() {
            return endPosition;
        }
        TokenKinds get_token() {
            if(currentToken == TokenKinds::StartState) {
                driver();
            }
            return currentToken;
        }
        TokenKinds next_token() {
            driver();
            return currentToken;
        }
        std::string get_value() {
            if(currentValue == "") {
                driver();
            }
            return currentValue;
        }

        void driver() {
            skip();
            char current_char = get_char();
            //std::cout<< current_char << '\n';
            switch (current_char) {
                /**
                 * 
                 */
                case EOF: {
                    currentToken = TokenKinds::EOFToken;
                    currentValue = EOF;
                    return;   
                }
                /**
                 * Puncatuator
                 */
                case ':': {
                    eat_char();
                    currentToken = TokenKinds::Colon;
                    currentValue = ":"; 
                    break;
                }
                case ',': {
                    eat_char();
                    currentToken = TokenKinds::Comma;
                    currentValue = ",";
                    break;
                }
                case '#': {
                    eat_char();
                    currentToken = TokenKinds::HashTag;
                    currentValue = "#";
                    break;
                }
                case '.': {
                    eat_char();
                    currentToken = TokenKinds::Dot;
                    currentValue = ".";
                    break;
                }
                case '{': {
                    eat_char();
                    currentToken = TokenKinds::BracesLeft;
                    currentValue = "{";
                    break;
                }
                case '}': {
                    eat_char();
                    currentToken = TokenKinds::BracesRight;
                    currentValue = "}";
                    break;
                }
                case '[': {
                    eat_char();
                    currentToken = TokenKinds::BracketLeft;
                    currentValue = "{";
                    break;
                }
                case ']': {
                    eat_char();
                    currentToken = TokenKinds::BracketRight;
                    currentValue = "{";
                    break;
                }
                case '(': {
                    eat_char();
                    currentToken = TokenKinds::ParenthesesLeft;
                    currentValue = "{";
                    break;
                }
                case ')': {
                    eat_char();
                    currentToken = TokenKinds::ParenthesesRight;
                    currentValue = ")";
                    break;
                }
                case '?': {
                    eat_char();
                    currentToken = TokenKinds::Qustion;
                    currentValue = "?";
                    break;
                }
                /**
                 * Operator
                */
                case '+': {
                    read_plus();
                    break;
                } 
                case '-': {
                    read_minus();
                    break;
                }
                case '*': {
                    read_multiply();
                    break;
                }
                case '/': {
                    read_slash();
                    break;
                }
                case '%': {
                    read_percent();
                    break;
                }
                case '=': {
                    read_eq();
                    break;
                }
                case '!': {
                    read_not();
                    break;
                }
                case '>': {
                    read_gt();
                    break;
                }
                case '<': {
                    read_lt();
                    break;
                }
                case '|': {
                    read_bitwise_or();
                    break;
                }
                case '&': {
                    read_bitwise_and();
                    break;
                }
                case '~': {
                    throw "[Error]: Not Supoort ~ Operator";
                    break;
                }
                case '^': {
                    throw "[Error]: Not Supoort ~ Operator";
                    break;
                }
                /**
                 *  Literal, Keyword or Identifier
                 */
                case '0':
                case '1': 
                    break;
                default:
                    read_identifier_keyword();
                    break;

            }
        }
    private:
        void read_plus() {
            if(start_with("+=")) {
                throw "[Error]: Not Support += Operator.";
            }
            if(start_with("++")) {
                throw "[Error]: Not Support ++ Operator";
            }
            if(start_with("+")) {
                eat_char(1);
                currentToken = TokenKinds::Plus;
                currentValue = "+";
                return;
            }
            throw "[Error]: Unexpect char";
        }
        void read_minus() {
            if(start_with("-=")) {
                throw "[Error]: Not Support -= Operator.";
            }
            if(start_with("--")) {
                throw "[Error]: Not Support -- Operator";
            }
            if(start_with("-")) {
                eat_char(1);
                currentToken = TokenKinds::Minus;
                currentValue = "-";
                return;
            }
            throw "[Error]: Unexpect char";
        }
        void read_multiply() {
            if(start_with("*=")) {
                throw "[Error]: Not Support *= Operator.";
            }
            if(start_with("**")) {
                throw "[Error]: Not Support ** Operator";
            }
            if(start_with("*")) {
                eat_char(1);
                currentToken = TokenKinds::Multiply;
                currentValue = "*";
                return;
            }
            throw "[Error]: Unexpect char";   
        }
        void read_slash() {
            if(start_with("/=")) {
                throw "[Error]: Not Support /= Operator.";
            }
            if(start_with("//")) {
                // read comment
            }
            if(start_with("/")) {
                eat_char(1);
                currentToken = TokenKinds::Divide;
                currentValue = "/";
                return;
            }
            throw "[Error]: Unexpect char";
        }
        void read_percent() {
            if(start_with("%=")) {
                throw "[Error]: Not Support %= Operators";
            }
            if(start_with("%")) {
                eat_char(1);
                currentToken = TokenKinds::Mod;
                currentValue = "%";
                return;
            }
        }
        void read_eq() {
            if(start_with("==")) {
                eat_char(2);
                currentToken = TokenKinds::Eq;
                currentValue = "==";
                return;
            }
            eat_char(1);
            currentToken = TokenKinds::Assign;
            currentValue = "=";
            return;
        }
        void read_not() {
            if(start_with("!=")) {
                eat_char(2);
                currentToken = TokenKinds::NotEq;
                currentValue = "!=";
                return;
            }
            eat_char(1);
            currentToken = TokenKinds::LogicalNOT;
            currentValue = "!";
            return;
        }
        void read_gt(){
            if(start_with(">=")) {
                eat_char(2);
                currentToken = TokenKinds::Gteq;
                currentValue = ">=";
                return;
            }
            eat_char(1);
            currentToken = TokenKinds::Gt;
            currentValue = ">";
            return;
        }
        void read_lt(){
            if(start_with("<=")) {
                eat_char(2);
                currentToken = TokenKinds::Lteq;
                currentValue = "<=";
                return;
            }
            eat_char(1);
            currentToken = TokenKinds::Lt;
            currentValue = "<";
            return;
        }
        void read_bitwise_or() {
            if(start_with("||")) {
                eat_char(2);
                currentToken = TokenKinds::LogicalOR;
                currentValue = "||";
            }
            if(start_with("|=")) {
                throw "[Error]: Not Support |= Operator";
            }
            throw "[Error]: Not Support | Operator";
        }
        void read_bitwise_and() {
            if(start_with("&&")) {
                eat_char(2);
                currentToken = TokenKinds::LogicalAND;
                currentValue = "||";
            }
            if(start_with("&=")) {
                throw "[Error]: Not Support &= Operator";
            }
            throw "[Error]: Not Support & Operator";
        }
        void read_identifier_keyword() {
            // read utils space or changeline
            std::set<char> char_set{
                ';',';',',','{','}', '[', ']', '(', ')',
                '#', ',', '?',
                '+', '-', '*', '/', '%',
                '=', '>', '<', '|', '&', '!'
            };
            std::string word;
            while(
                ! is_eof() &&
                !(is_char(' ')  || is_char('\t') || is_char('\n')) &&
                !( is_char_set(char_set) )
            ) {
                word += eat_char(1);
            }
            std::cout << "Word:" << word << std::endl;
            // Keywords
            if(word == "while") {
                currentToken = TokenKinds::WhileKeyword;
                currentValue = "while";
                return;
            }
            if(word == "for") {
                currentToken = TokenKinds::ForKeyword;
                currentValue = "for";
                return;
            }
            if(word == "if") {
                currentToken = TokenKinds::IfKeyword;
                currentValue = "if";
                return;
            }
            if(word == "eles") {
                currentToken = TokenKinds::ElesKeyword;
                currentValue = "else";
                return;
            }
            if(word == "return") {
                currentToken = TokenKinds::ReturnKeyword;
                currentValue = "return";
                return;
            }
            if(word == "var") {
                currentToken = TokenKinds::VarKeyword;
                currentValue = "var";
                return;
            }
            if(word == "function") {
                currentToken = TokenKinds::FunctionKeyword;
                currentValue = "function";
                return;
            }
            if(word == "number") {
                currentToken = TokenKinds::NumberKeyword;
                currentValue = "number";
                return;
            }
            if(word == "string") {
                currentToken = TokenKinds::StringKeyword;
                currentValue = "string";
                return;
            }
            if(word == "bool") {
                currentToken = TokenKinds::BoolKeyword;
                currentValue = "bool";
                return;
            }
            if(word == "true") {
                currentToken = TokenKinds::BoolLiteral;
                currentValue = "true";
                return;
            }
            if(word == "false") {
                currentToken = TokenKinds::BoolLiteral;
                currentValue = "false";
                return;
            }
            // Identifier
            currentToken = TokenKinds::Identifier;
            currentValue = word;
            return;
        }
};

#endif