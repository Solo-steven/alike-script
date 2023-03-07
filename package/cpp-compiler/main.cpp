#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include "parser.cpp"
int main() {
    auto tokenizer = std::make_unique<Tokenizer>("{ test(c, n) }");
    while(1) {
        auto token = tokenizer->next_token();
        if(token != TokenKinds::EOFToken) {
            std::cout << tokenizer->get_value()<<std::endl;
        }else {
            break;
        }
    }
    try{
        auto parser = std::make_unique<Parser>("{ test(c, n) }");
        auto item = parser->parse();
        item->print();
    } catch(char const* str) {
        std::cout <<str;
    }
    return 0;
}