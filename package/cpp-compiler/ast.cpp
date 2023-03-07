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
        std::cout << "{\"type\":\"ProgramItem\"}";
    }
    virtual std::string toString() {
        return std::string("{\"type\":\"ProgramItem\"}");
    }
};
class Statement: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Statement";
    }
    virtual std::string toString() {
        return std::string("Statement");
    }
};
class Declaration: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Declaration";
    }
    virtual std::string toString() {
        return std::string("Declaration");
    }
};
class Expression: public ProgramItem {
    public:
    virtual void print(){
        std::cout << "Expression";
    }
    virtual std::string toString() {
        return std::string("Expression");
    }
};
class Program {
    public:
    std::vector<std::unique_ptr<ProgramItem>> body;
    void print() {
        std::cout << "{\"type\":\"Program\",\"body\":[";
        for(int i = 0 ; i < body.size() ; ++i) {
            body[i]->print();
            if(i != body.size()-1)
                std::cout << ",";
        }
        std::cout << "]}";
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
        std::cout << "{\"type\":\"NumberLiteral\",\"value\":\""<<value<<"\"}";   
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"NumberLiteral\",\"value\":\"%f\"}";  
        size_t length = std::snprintf(nullptr,0, format_str, value);
        char* char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str, value);
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
        std::cout << "{\"type\":\"BoolLiteral\",\"value\":\""<<value<<"\"}"; 
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"BoolLiteral\",\"value\":\"%d\"}";  
        size_t length = std::snprintf(nullptr, 0, format_str, value);
        char* char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str, value);
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
        std::cout << "{\"type\":\"Identifer\",\"name\":\""<<name<<"\"}";   
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"Identifer\",\"name\":\"%s\"}";  
        size_t length = std::snprintf(nullptr, 0, format_str, name.c_str());
        char *char_buffer = new char[length +1];
        std::snprintf(char_buffer, length+1, format_str, name.c_str());
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
        std::cout <<"{\"type\":\"UnaryExpression\",\"argument\":"<<argument->toString()<<",\"operator\":\""<<oper<<"\"}";  
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"UnaryExpression\",\"argument\":%s,\"operator\":\"%s\"}";
        std::string argument_string = argument->toString();
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
        std::cout << "{\"type\":\"BinaryExpression\",\"left\":"<<left->toString()<<",\"right\":"<<right->toString()<<",\"operator\":\""<<oper<<"\"}";
    }
    std::string toString() {
        const char format_str[]=  "{\"type\":\"BinaryExpression\",\"left\":%s,\"right\":%s,\"operator\":\"%s\"}";
        std::string left_string = left->toString();
        std::string right_string = right->toString();
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
        std::cout << "{\"type\":\"ConditionalExpression\",\"conditional\":"<<cond->toString()<<",\"consequnce\":"<<conseq->toString()<<",\"alter\":"<<alter->toString()<<"}";
    }
    std::string toString() {
        std::string cond_string = cond->toString();
        std::string conseq_string = conseq->toString();
        std::string alter_string = alter->toString();
        char format_str[] = "{\"type\":\"ConditionalExpression\",\"conditional\":%s,\"consequnce\":%s,\"alter\":%s}";
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
        std::cout << "{\"type\":\"AssignmentExpression\",\"left\":"<<left->toString()<<",\"right\":"<<right->toString()<<"}";
    }
    std::string toString() {
        std::string left_string = left->toString();
        std::string right_string = right->toString();
        char format_str[] ="{\"type\":\"AssigmentExpression\",\"left\":%s,\"right\":%s}";
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
    CallExpression(std::string callee, std::vector<std::unique_ptr<Expression>> params):
        callee(callee), params(std::move(params)) {};
    void print() {
        std::cout << "{\"type\":\"CallExpression\",\"callee\":\""<<callee<<"\",\"params\":[";
        for(int i = 0 ; i < params.size();++i) {
            std::cout << params[i]->toString();
            if(params.size() -1 != i) {
                std::cout<<",";
            }
        }
        std::cout<<"]}";
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"CallExpression\",\"callee\":\"%s\",\"params\":[";
        size_t length = std::snprintf(nullptr, 0 , format_str, callee.c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str, callee.c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        for(int i = 0 ; i < params.size();++i) {
            std_str.append(params[i]->toString());
            if(params.size() -1 != i) {
                std_str.append(",");
            }
        }
        std_str.append("]}");
        return std_str;
    }
};
/**
 * Statement 
*/
class BlockStatement: public Statement {
    public:
    std::vector<std::unique_ptr<ProgramItem>> body;
    BlockStatement(std::vector<std::unique_ptr<ProgramItem>> body): body(std::move(body)) {}
    void print() {
        std::cout << "{\"type\":\"BlockStatement\",\"body\":[";
        for(int i = 0 ; i < body.size() ; ++i) {
            std::cout << body[i]->toString();
            if(i!= body.size()-1) 
                std::cout <<",";
        } 
        std::cout << "]}";
    }
    std::string toString() {
        std::string std_str("{\"type\":\"BlockStatement\",\"body\":[");
        for(int i = 0 ; i < body.size() ; ++i) {
            std_str.append(body[i]->toString());
            if(i!= body.size()-1) 
                std_str.append(",");
        }
        std_str.append("]}");
        return std_str;
    }
};
class WhileStatement: public Statement {
    public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<BlockStatement> body;
    WhileStatement(std::unique_ptr<Expression> cond,std::unique_ptr<BlockStatement> body):
        cond(std::move(cond)), body(std::move(body)){}
    void print() {
        std::cout << "{\"type\":\"WhileStatement\",\"condition\":"<<cond->toString()<<",\"body\":"<<body->toString()<<"}";
    }
    std::string toString() {
        const char format_str[] = "{\"type\":\"WhileStatement\",\"condition\":%s,\"body\":%s}";
        size_t length = std::snprintf(nullptr, 0, format_str, cond->toString().c_str(), body->toString().c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str,cond->toString().c_str(), body->toString().c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class IfStatement: public Statement {};
class ReturnStatement: public Statement {};
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
        std::cout<<"{\"type\":\"VariableDeclaration\",\"name\":\""<<name<<"\",\"variableType\":\""<<type_to_string(type)<<"\",";
        if(init.get() == nullptr) {
            std::cout << "\"init\":\"\"";
        }else {
            std::cout << "\"init\":" <<init->toString();
        }    
        std::cout<<"}";
    }
    std::string toString() {
        char format_str[] = "{\"type\":\"VariableDeclaration\",\"name\":\"%s\",\"variableType\":\"%s\",\"init\":%s}";
        size_t length = std::snprintf(
            nullptr, 0, format_str, 
            name.c_str(), type_to_string(type).c_str(), init.get() == nullptr ? "": init->toString().c_str()
        );
        char *char_buffer = new char[length+1];
        std::snprintf(
            char_buffer, length+1, format_str,
            name.c_str(), type_to_string(type).c_str(), init.get() == nullptr ? "": init->toString().c_str()
        );
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class FunctionParam {
    public:
    std::string name;
    Type return_type;
    FunctionParam(std::string name, Type return_type): name(name), return_type(return_type) {}
    void print() {
        std::cout<<"{\"type\":\"FunctionParam\", \"name\":\""<<name<<"\",\"paramType\":\""<<type_to_string(return_type)<<"\"}";
    }
    std::string toString() {
        char format_str[] = "{\"type\":\"FunctionParam\", \"name\":\"%s\",\"paramType\":\"%s\"}";
        size_t length = std::snprintf(nullptr, 0 , format_str, name.c_str(), type_to_string(return_type).c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1, format_str, name.c_str(), type_to_string(return_type).c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        return std_str;
    }
};
class FunctionDeclaration: public Declaration {
    public:
    std::string name;
    std::vector<std::unique_ptr<FunctionParam>> params;
    std::unique_ptr<BlockStatement> body;
    Type return_type;
    FunctionDeclaration(
        std::string name, Type type,
        std::vector<std::unique_ptr<FunctionParam>> params,
        std::unique_ptr<BlockStatement> body
    ): name(name), return_type(type), params(std::move(params)), body(std::move(body)) {}
    void print() {
        std::cout<<"{\"type\":\"FunctionDelcaration\",\"name\":\""<<name<<"\",\"returnType\":\""<< type_to_string(return_type) <<"\",\"params\":[";
        for(int i = 0 ; i< params.size() ; ++i ){
            params[i]->print();
            if(i != params.size()-1)
                std::cout << ",";
        }
        std::cout<< "],\"body\":"<< body->toString() << "}";
    }
    std::string toString() {
        const char format_str[]= "{\"type\":\"FunctionDelcaration\",\"name\":\"%s\",\"returnType\":\"%s\",\"params\":[";
        size_t length = std::snprintf(nullptr, 0, format_str, name.c_str(), type_to_string(return_type).c_str());
        char *char_buffer = new char[length+1];
        std::snprintf(char_buffer, length+1,format_str, name.c_str(), type_to_string(return_type).c_str());
        std::string std_str(char_buffer);
        delete[] char_buffer;
        for(int i = 0 ; i < params.size() ; ++i) {
            std_str.append(params[i]->toString());
            if(i!= params.size()-1) 
                std_str.append(",");
        }
        std_str.append("],\"body\":");
        std_str.append(body->toString());
        std_str.append("}");
        return std_str;
    }
};
} // End Namesacpe AST
#endif