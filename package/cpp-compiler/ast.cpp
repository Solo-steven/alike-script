#include <iostream>
#include <vector>
#include <memory>

#ifndef CPP_COMPILER_AST
#define CPP_COMPILER_AST

namespace AST { // Start Namespace AST
enum Type {
    Number,
    Bool,
    String,
    Null,
};
// helper function
std::string type_to_string(Type t) {
    switch(t) {
    case Type::Number:
        return std::string("Number");
    case Type::Bool:
        return std::string("Bool");
    case Type::String:
        return std::string("String");
    default:
        return std::string("");
    }
}
class ProgramItem {
    public:
    virtual void print(){
        std::cout << "ProgramItem" << std::endl;
    }
    virtual std::string toString() {
        return std::string("ProgramItem");
    }
};
class Statement: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Statement" << std::endl;
    }
    virtual std::string toString() {
        return std::string("Statement");
    }
};
class Declaration: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Declaration" << std::endl;
    }
    virtual std::string toString() {
        return std::string("Declaration");
    }
};
class Expression: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Expression" << std::endl;
    }
    virtual std::string toString() {
        return std::string("Expression");
    }
};
class Program {
    public:
    std::vector<std::unique_ptr<ProgramItem>> body;
    void print() {
        std::cout << "Program(body[";
        for(int i = 0 ; i < body.size() ; ++i) {
            body[i]->print();
            if(i != body.size()-1)
                std::cout << ",";
        }
        std::cout << "])";
    }
    
};
/** ===================================================================
 *  Expression
 * ====================================================================
*/
class NumberLiteral: public Expression {
    public:
    float value;
    NumberLiteral(){}
    NumberLiteral(float value) {
        this->value = value;
    }
    void print(){
        std::cout << "NumberLiteral(" << value << ")";
    }
    std::string toString() {
        size_t length = std::snprintf(nullptr,0, "NumberLiteral(%f)", value);
        char* char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, "NumberLiteral(%f)", value);
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class BoolLiteral: public Expression {
    public:
    bool value;
    BoolLiteral(){}
    BoolLiteral(bool value): value(value) {}
    void print(){
        std::cout << "BoolLiteral(" << value << ")";
    }
    std::string toString() {
        size_t length = std::snprintf(nullptr, 0, "BoolLiteral(%d)", value);
        char* char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, "BoolLiteral(%d)", value);
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class StringLiteral: public Expression {
    public:
    std::string value;
    StringLiteral(){}
    StringLiteral(std::string str): value(str) {}
    
};
class NullLiteral: public Expression {};
class Identifier: public Expression {
    public:
    std::string name;
    Type type;
    Identifier() {}
    Identifier(std::string name): name(name){}
    void print() {
        std::cout << "Identifer(" << name  << ")";    
    }
    std::string toString() {
        size_t length = std::snprintf(nullptr, 0, "Identifier(%s)", name.c_str());
        char *char_buffer = new char[length +1];
        std::snprintf(char_buffer, length+1, "Identifier(%s)", name.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class UnaryExpression: public Expression {
    public:
    std::string oper;
    std::unique_ptr<Expression> argument;
    UnaryExpression() {}
    UnaryExpression(std::unique_ptr<Expression> argument, std::string op): argument(std::move(argument)), oper(op) {}
    void print() {
        std::cout << "UnaryExpression("<<"argument:"<< argument->toString()<<",operator:"<<oper<<")";
    }
    std::string toString() {
        std::string argument_string = argument->toString();
        char format_str[] = "UnaryExpression(argument:%s,operator:%s)";
        size_t length = std::snprintf(nullptr, 0 , format_str, argument_string.c_str(), oper.c_str());
        char* char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str, argument_string.c_str(), oper.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }

};
class BinaryExpression: public Expression {
    public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string oper;
    BinaryExpression() {}
    BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, std::string op): 
        left(std::move(left)), right(std::move(right)), oper(op) {}
    void print() {
        std::cout << "BinaryExpression(left:"<<left->toString() << ",right:" << right->toString()<< ",operator:"<< oper <<")";
    }
    std::string toString() {
        std::string left_string = left->toString();
        std::string right_string = right->toString();
        char format_str[] = "BinaryExpression(left:%s,right:%s,operator:%s)";
        size_t length = std::snprintf(nullptr, 0, format_str, left_string.c_str(), right_string.c_str(), oper.c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1,  format_str, left_string.c_str(), right_string.c_str(), oper.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class ConditionalExpression: public Expression {
    public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Expression> alter;
    std::unique_ptr<Expression> conseq;
    ConditionalExpression() {}
    ConditionalExpression(std::unique_ptr<Expression> cond, std::unique_ptr<Expression> conseq, std::unique_ptr<Expression> alter):
        cond(std::move(cond)), conseq(std::move(conseq)), alter(std::move(alter)){}
    void print() {
        std::cout << "ConditionalExpression(condition:"<< cond->toString() << ",consequnce:" << conseq->toString() << ",alter:" << alter->toString()<<")";
    }
    std::string toString() {
        std::string cond_string = cond->toString();
        std::string conseq_string = conseq->toString();
        std::string alter_string = alter->toString();
        char format_str[] = "ConditionalExpression(conditional:%s,consequnce:%s,alter:%s";
        size_t length = std::snprintf(nullptr, 0, format_str, cond_string.c_str(), conseq_string.c_str(), alter_string.c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1,  format_str, cond_string.c_str(), conseq_string.c_str(), alter_string.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
        
};
class AssigmentExpression: public Expression {
    public:
    // currently only one assigment operator is legal.
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    AssigmentExpression() {}
    AssigmentExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right ) {
        this->left = std::move(left);
        this->right = std::move(right);
    }
    void print() {
        std::cout << "AssigmentExpression(left:"<<left->toString() << ",right:" << right->toString() <<")";
    }
    std::string toString() {
        std::string left_string = left->toString();
        std::string right_string = right->toString();
        char format_str[] = "AssigmentExpression(left:%s,right:%s,operator:%s)";
        size_t length = std::snprintf(nullptr, 0, format_str, left_string.c_str(), right_string.c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1,  format_str, left_string.c_str(), right_string.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class SequnceExpression: public Expression {
    public:
    std::vector<std::unique_ptr<Expression>> exprs;
    SequnceExpression() {}
    SequnceExpression(std::vector<std::unique_ptr<Expression>> exprs): exprs(std::move(exprs)) {}
    // TODO: Must
};
class CallExpression: public Expression {
    public:
        std::string callee;
        std::vector<std::unique_ptr<Expression>> params;
};
/**
 *  Delaration
*/
class VariableDelaration: public Declaration {
    public:
    std::string name;
    Type type;
    std::unique_ptr<Expression> init;
    VariableDelaration(std::string name, Type type, std::unique_ptr<Expression> init):
        name(name), type(type), init(std::move(init)) {}
    void print() {
        std::cout << "VariableDeclaration(name:"<<name<<",type:" << type_to_string(type);
        if(init.get() == nullptr) {
            std::cout << ",init:()";
        }else {
            std::cout << ".init(" << init->toString() << ")";
        }
        std::cout << ")";
    }
};
class FunctionDeclaration: public Declaration {};
class FunctionParam {};
/**
 * Statement 
*/
class WhileStatement: public Statement {};
class BlockStatement: public Statement {};
class IfStatement: public Statement {};
class ReturnStatement: public Statement {};
} // End Namesacpe AST


#endif