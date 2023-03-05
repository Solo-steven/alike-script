#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include "parser.cpp"
int main() {
    auto tokenizer = std::make_unique<Tokenizer>("\
    function add (a: number, b: number): number {\
        a + b   \
    }\
    ");
    while(1) {
        auto token = tokenizer->next_token();
        if(token != TokenKinds::EOFToken) {
            std::cout << tokenizer->get_value()<<std::endl;
        }else {
            break;
        }
    }
    auto parser = std::make_unique<Parser>("\
    function add (a: number, b: number): number {\
        a + b   \
    }\
    ");
    auto item = parser->parse();
    item->print();
    return 0;
}