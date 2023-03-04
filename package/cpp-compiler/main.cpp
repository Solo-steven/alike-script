#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include "parser.cpp"
int main() {
    auto tokenizer = std::make_unique<Tokenizer>("a = a+b*c/d ");
    while(1) {
        auto token = tokenizer->next_token();
        if(token != TokenKinds::EOFToken) {
            std::cout << tokenizer->get_value()<<std::endl;
        }else {
            break;
        }
    }
    auto parser = std::make_unique<Parser>("a = (a+b)*c/d ");
    auto item = parser->parse();
    item->print();
    return 0;
}