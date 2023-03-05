#include <iostream>
#include <memory>
#include <vector>
#include "ast.cpp"
#include "tokenizer.cpp"

class Parser {
    private:
        std::unique_ptr<Tokenizer> tokenizer;
    public:
        Parser(std::string code) {
            tokenizer = std::make_unique<Tokenizer>(code);
        }
        std::unique_ptr<AST::Program> parse() {
            return parse_program();
        }
        std::unique_ptr<AST::Program> parse_program() {
            auto program = std::make_unique<AST::Program>();
            while(true) {
                auto token = tokenizer->get_token();
                if(token == TokenKinds::EOFToken) {
                    break;
                }
                auto program_item = parse_program_item();
                program->body.push_back(std::move(program_item));
            }
            return program;
        }
        std::unique_ptr<AST::ProgramItem> parse_program_item() {
            auto token = tokenizer->get_token();
            switch(token) {
                // case TokenKinds::WhileKeyword: {
                // }
                // case TokenKinds::ForKeyword: {
                // }
                // case TokenKinds::IfKeyword: {
                // }
                // case TokenKinds::ReturnKeyword: {
                // } 
                case TokenKinds::BracesLeft: {
                    return parse_block_statement();
                }
                case TokenKinds::VarKeyword: {
                    return parse_variable_declaration();
                }
                case TokenKinds::FunctionKeyword: {
                    return parse_function_declaration();
                } 
                default : {
                    return parse_expression();
                }
            }
        }
/** =============================================================
 *  Declaration
 * ==============================================================
*/
        std::unique_ptr<AST::VariableDelaration> parse_variable_declaration() {
            if(tokenizer->get_token() != TokenKinds::VarKeyword) {
                throw "";
            }
            tokenizer->next_token();
            if(tokenizer->get_token() != TokenKinds::Identifier) {
                throw "";
            }
            std::string name = tokenizer->get_value();
            tokenizer->next_token();
            AST::Type type;
            switch(tokenizer->get_token()) {
            case TokenKinds::NumberKeyword:
                type = AST::Type::Number;
                break;
            case TokenKinds::StringKeyword:
                type = AST::Type::String;
                break;
            case TokenKinds::BoolKeyword:
                type = AST::Type::Bool;
            default:
                throw "";
            }
            tokenizer->next_token();
            if(tokenizer->get_token() == TokenKinds::Assign) {
                tokenizer->next_token();
                auto expr = parse_expression();
                return std::make_unique<AST::VariableDelaration>(name, type, std::move(expr));
            }
            return std::make_unique<AST::VariableDelaration>(name, type, nullptr);
        }
        std::unique_ptr<AST::FunctionDeclaration> parse_function_declaration() {
            if(tokenizer->get_token() != TokenKinds::FunctionKeyword) {
                throw "[Error]: function Delcaration must start with function keyword";
            }
            tokenizer->next_token();
            if(tokenizer->get_token() != TokenKinds::Identifier) {
                throw "[Error]: function delcaration must has a name.";
            }
            std::string name = std::string(tokenizer->get_value());
            tokenizer->next_token();
            if(tokenizer->get_token() != TokenKinds::ParenthesesLeft) {
                throw "[Error]: function declaration must has param list";
            }
            tokenizer->next_token();
            std::vector<std::unique_ptr<AST::FunctionParam>> params;
            while(
                tokenizer->get_token() != TokenKinds::ParenthesesRight &&
                tokenizer->get_token() != TokenKinds::EOFToken 
            ) {
                params.push_back(parse_function_param());
                if(tokenizer->get_token() == TokenKinds::Comma) {
                    tokenizer->next_token();
                }
            }
            if(tokenizer->get_token() == TokenKinds::EOFToken) {
                throw "[Error]: function params expected end with ParenthesesRight, but got EOF";
            }
            tokenizer->next_token();
            if(tokenizer->get_token() != TokenKinds::Colon) {
                throw "[Error]: function expected a return type";
            }
            tokenizer->next_token();
            AST::Type type;
            switch(tokenizer->get_token()) {
            case TokenKinds::NumberKeyword:
                type = AST::Type::Number;
                break;
            case TokenKinds::StringKeyword:
                type = AST::Type::String;
                break;
            case TokenKinds::BoolKeyword:
                type = AST::Type::Bool;
                break;
            default:
                throw "[Error] function declaration must has a type";
            }
            tokenizer->next_token();
            auto body = parse_block_statement();
            return std::make_unique<AST::FunctionDeclaration>(name, type, std::move(params), std::move(body));
        }
        std::unique_ptr<AST::FunctionParam> parse_function_param() {
            if(tokenizer->get_token() != TokenKinds::Identifier) {
                throw "[Error]: function param must start with a identifier";
            }
            std::string name = std::string(tokenizer->get_value());
            tokenizer->next_token();
            if(tokenizer->get_token() != TokenKinds::Colon) {
                throw "[Error] function param must has a type";
            }
            tokenizer->next_token();
            AST::Type type;
            switch(tokenizer->get_token()) {
            case TokenKinds::NumberKeyword:
                type = AST::Type::Number;
                break;
            case TokenKinds::StringKeyword:
                type = AST::Type::String;
                break;
            case TokenKinds::BoolKeyword:
                type = AST::Type::Bool;
                break;
            default:
                throw "[Error] function param must has a type";
            }
            tokenizer->next_token();
            return std::make_unique<AST::FunctionParam>(name, type);
        }
/** =========================================================
 *  Statement
 * ==========================================================
*/
    std::unique_ptr<AST::BlockStatement> parse_block_statement() {
        if(tokenizer->get_token() != TokenKinds::BracesLeft) {
            throw "[Error]: block Statement Must Start With BracesLeft";
        }
        tokenizer->next_token();
        std::vector<std::unique_ptr<AST::ProgramItem>> body;
        while(
            tokenizer->get_token() != TokenKinds::BracesRight && 
            tokenizer->get_token() != TokenKinds::EOFToken
        ) {
            auto program_item = parse_program_item();
            body.push_back(std::move(program_item));
        }
        if(tokenizer->get_token() == TokenKinds::EOFToken) {
            throw "[Error]: block Statement Must End With BracesRight, But got EOF";
        }
        tokenizer->next_token();
        return std::make_unique<AST::BlockStatement>(std::move(body));
    }
/** =========================================================
 *  Expression
 * ==========================================================
*/
        std::unique_ptr<AST::Expression> parse_expression() {
            // TODO
            auto expr =  parse_assigment_expression();
            auto exprs = std::vector<std::unique_ptr<AST::Expression>>();
            exprs.push_back(std::move(expr));
            while(tokenizer->get_token() == TokenKinds::Comma) {
                tokenizer->next_token();
                exprs.push_back(std::move(parse_assigment_expression()));
            }
            if(exprs.size() == 1) {
                return std::move(exprs[0]);
            }
            return std::make_unique<AST::SequnceExpression>(std::move(exprs));
        } 
        std::unique_ptr<AST::Expression> parse_assigment_expression() {
            auto left = parse_conditional_expression();
            if(tokenizer->get_token() != TokenKinds::Assign) {
                return left;
            }
            tokenizer->next_token();
            auto right = parse_conditional_expression();
            auto assigment_expr = std::make_unique<AST::AssigmentExpression>(std::move(left), std::move(right));
            return assigment_expr;
        }
        std::unique_ptr<AST::Expression> parse_conditional_expression() {
            auto condition = parse_binary_expression();
            if(tokenizer->get_token() != TokenKinds::Qustion) {
                return condition;
            }
            tokenizer->next_token();
            auto conseq = parse_binary_expression();
            if(tokenizer->get_token() != TokenKinds::Colon) {
                throw "[Error]: Condition Expression Should Has Alternative and Consequence";
            }
            tokenizer->next_token();
            auto alter = parse_binary_expression();
            auto condition_expr = std::make_unique<AST::ConditionalExpression>(std::move(condition), std::move(conseq), std::move(alter));
            return condition_expr;
        }
        std::unique_ptr<AST::Expression> parse_binary_expression() {
            auto atom = parse_unary_expression();
            auto next_token = tokenizer->get_token();
            if(is_binary_op(next_token)) {
                auto binary = parse_binary_ops(std::move(atom));
                auto binary_ptr = static_cast<AST::BinaryExpression*>(binary.get());
                return binary;
            }
            return atom;
        }
        // implement of Operator-precedence parser
        // ref: https://en.wikipedia.org/wiki/Operator-precedence_parser
        std::unique_ptr<AST::Expression> parse_binary_ops(std::unique_ptr<AST::Expression> left, int last_precedence = -1) {
            while(true) {
                auto current_op = tokenizer->get_token();
                if(!is_binary_op(current_op) || get_binary_op_precedence(current_op) < last_precedence) {
                    break;
                }
                tokenizer->next_token();
                auto right = parse_unary_expression();
                auto next_op = tokenizer->get_token();
                if(
                    is_binary_op(next_op) &&
                    get_binary_op_precedence(next_op) > get_binary_op_precedence(current_op)
                ) {
                    right = parse_binary_ops(std::move(right), get_binary_op_precedence(next_op));
                }
                left = std::make_unique<AST::BinaryExpression>(std::move(left), std::move(right), binary_op_to_string(current_op));
            }
            return left;
        }
        std::unique_ptr<AST::Expression> parse_unary_expression() {
            auto current_token = tokenizer->get_token();
            switch (current_token)
            {
            case TokenKinds::LogicalNOT:
            case TokenKinds::Minus:
            case TokenKinds::Plus: {
                tokenizer->next_token();
                auto argument = parse_unary_expression();
                return std::make_unique<AST::UnaryExpression>(std::move(argument), unary_op_to_string(current_token));
            }
            default:
                return parse_primary_expression();
            }
        }
        std::unique_ptr<AST::Expression> parse_primary_expression() {
            auto current_token = tokenizer->get_token();
            switch (current_token)
            {
            case TokenKinds::Identifier: {
                auto name = tokenizer->get_value();
                tokenizer->next_token();
                return std::make_unique<AST::Identifier>(name);
            }
            case TokenKinds::BoolLiteral: {
                auto value = tokenizer->get_value();
                tokenizer->next_token();
                if(value == "true") {
                    return std::make_unique<AST::BoolLiteral>(true);
                }
                if(value == "false") {
                    return std::make_unique<AST::BoolLiteral>(false);
                }
                throw "[Error]";
            }
            case TokenKinds::StringLiteral: {
                auto value = tokenizer->get_value();
                return std::make_unique<AST::StringLiteral>(value);
            }
            case TokenKinds::NumberLiteral: {
                auto value = tokenizer->get_value();
                // TODO, parse string of number into float number
            }
            case TokenKinds::ParenthesesLeft: {
                tokenizer->next_token();
                auto expr = parse_expression();
                if(tokenizer->get_token() != TokenKinds::ParenthesesRight) {
                    throw "";
                }
                tokenizer->next_token();
                return expr;
            }
            default:
               throw "[Error]: Unexecpted Primary Expression";
            }  
        }

};