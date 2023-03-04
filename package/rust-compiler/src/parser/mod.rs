use crate::Tokenizer;
use crate::token::{Token, get_pre_of_binary_op, get_text_of_binary_op, is_binary_op};
use crate::ast::*;
mod test;
pub struct  Parser {
    tokenizer: Tokenizer,
}
impl Parser {
    pub fn new(code: String) -> Parser{
        Parser {
            tokenizer: Tokenizer::new(code)
        }
    }
    pub fn parse_program(&mut self) -> Program {
        let mut body = Vec::<ProgramItem>::new();
        loop {
            match self.tokenizer.get_token() {
                Token::EOF => {
                    return Program { body }
                }
                _ => {
                    body.push(self.parse_program_item());
                }
            }
        }
    }
    fn parse_program_item(&mut self) -> ProgramItem {
        match self.tokenizer.get_token() {
            Token::VarKeyword => {
                ProgramItem::Declaration(self.parse_variable_declaration())
            }
            Token::FunctionKeyword => {
                ProgramItem::Declaration(self.parse_function_declaration())
            }
            Token::BracesLeft => {
                ProgramItem::Statement(self.parse_block_statement())
            }
            Token::WhileKeyword => {
                ProgramItem::Statement(self.parse_while_statement())
            }
            Token::IfKeyword => {
                ProgramItem::Statement(self.parse_if_statement())
            }
            Token::ReturnKeyword => {
                ProgramItem::Statement(self.parse_return_statement())
            }
            _ => {
                ProgramItem::Expression(self.parse_expression())
            }
        }
    }
    fn parse_while_statement(&mut self) -> Statement {
        let test: Expression;
        match self.tokenizer.get_token() {
             Token::WhileKeyword  => {
                self.tokenizer.next_token();
             }
             _ => {
                panic!("[Error]: While Statement Should Start With Keyword");
             }
        }
        match self.tokenizer.get_token() {
            Token::ParenthesesLeft  => {
                self.tokenizer.next_token();
                test = self.parse_expression();
            }
            _ => {
                panic!("[Error]: While Statement's Condition Should be Wrapped By ParentheseLeft");
            }
       }
       match self.tokenizer.get_token() {
            Token::ParenthesesRight => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: While Statement's Condition Should be Wrapper By ParentheseRight")
            }
       }
       return Statement::WhileState(WhileStatement{
            test,
            body: Box::<Statement>::new(self.parse_block_statement())
       })

    }
    fn parse_block_statement(&mut self) -> Statement {
        match self.tokenizer.get_token() {
            Token::BracesLeft => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: BlockStatement Should Start With BraceLeft, {:?}", self.tokenizer.get_token());
            }
        }
        let mut body: Vec<ProgramItem> = Vec::<ProgramItem>::new();
        loop {
            match self.tokenizer.get_token() {
                Token::EOF => {
                    panic!("[Error]: BlockStatement End Without BraceRight");
                }
                Token::BracesRight => {
                    self.tokenizer.next_token();
                    return Statement::BlockState(BlockStatement{
                        body
                    })
                }
                _ => {
                    body.push(self.parse_program_item());
                }
            }
        }
    }
    fn parse_if_statement(&mut self) -> Statement {
        match self.tokenizer.get_token() {
            Token::IfKeyword => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: If Statement Should Start With `if` keyword.");
            }
        }
        let test:Expression;
        match self.tokenizer.get_token() {
            Token::ParenthesesLeft  => {
                self.tokenizer.next_token();
                test = self.parse_expression();
            }
            _ => {
                panic!("[Error]: Condition Of If Statement Should be Wrapper In Parentheses, Lock of ParentheseLeft");
            }
       }
       match self.tokenizer.get_token() {
            Token::ParenthesesRight => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Condition Of If Statement Should be Wrapper In Parentheses, Lock of ParentheseRight");
            }
       }
       let consequence = self.parse_block_statement();
       match self.tokenizer.get_token() {
           Token::ElesKeyword => {
             self.tokenizer.next_token();
             match self.tokenizer.get_token() {
                Token::BracesLeft => {
                    Statement::IfState(IfStatement { test, consequent: Box::new(consequence), alter:Some(Box::new(self.parse_block_statement())) })
                }
                Token::IfKeyword => {
                    Statement::IfState(IfStatement { test, consequent: Box::new(consequence), alter:Some(Box::new(self.parse_if_statement())) })
                }
                _ => {
                    panic!("[Error]: Else Keyword Must Concat With Block Statement Or If Statement");
                }
             }
           }
           _ => {
            Statement::IfState(IfStatement { test, consequent: Box::new(consequence), alter: None })
           }
       }
    }
    fn parse_return_statement(&mut self) -> Statement {
        match self.tokenizer.get_token() {
            Token::ReturnKeyword => {
                self.tokenizer.next_token();
                return Statement::ReturnState(ReturnStatement {
                    argument: self.parse_expression()
                })
            }
            _ => {
                panic!("[Error]: Return Statement Should Start With Return Keyword.");
            }
        }
    }
    fn parse_variable_declaration(&mut self) -> Declaration {
        let identifier_name: String;
        let identifier_type: Type;
        match self.tokenizer.get_token() {
            Token::VarKeyword => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Variable Declaration Should Start With `var` keyword");
            }
        }
        match self.tokenizer.get_token() {
            Token::Identifier(value) => {
                self.tokenizer.next_token();
                identifier_name = value;
            }
            _ => {
                panic!("[Error]: Variable Delcaration Should Provide A Identifier.");
            }
        }
        match self.tokenizer.get_token() {
            Token::NumberKeyword => {
                self.tokenizer.next_token();
                identifier_type = Type::Number;
            }
            Token::StringKeyword => {
                self.tokenizer.next_token();
                identifier_type = Type::String;
            }
            Token::BoolKeyword => {
                self.tokenizer.next_token();
                identifier_type = Type::Bool;
            }
            _ => {
                panic!("[Error]: Variable Declaration Should Provide Type");
            }
        }
        return match self.tokenizer.get_token() {
            Token::Assign => {
                self.tokenizer.next_token();
                let init_expression = self.parse_expression();
                Declaration::Variable(VariableDeclaration { 
                    name: identifier_name, 
                    variable_type: identifier_type, 
                    init: Some(init_expression)
                })
            }
            _ => {
                Declaration::Variable(VariableDeclaration { 
                    name: identifier_name, 
                    variable_type: identifier_type, 
                    init: None
                })
            }
        }
    }
    fn parse_function_declaration(&mut self) -> Declaration {
        let function_name: String;
        let function_type: Type;
        match self.tokenizer.get_token() {
            Token::FunctionKeyword => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Function Declaration Should Start With `var` keyword");
            }
        }
        match self.tokenizer.get_token() {
            Token::Identifier(value) => {
                self.tokenizer.next_token();
                function_name = value;
            }
            _ => {
                panic!("[Error]: Function Delcaration Should Provide A Identifier.");
            }
        }
        let params = self.parse_function_declaration_params();
        match self.tokenizer.get_token() {
            Token::Colon => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Function Declaration Must Has Return Type With Colon, But Got {:?}", self.tokenizer.get_token())
            }
        }
        match self.tokenizer.get_token() {
            Token::NullKeyword => {
                self.tokenizer.next_token();
                function_type = Type::Null;
            }
            Token::NumberKeyword  => {
                self.tokenizer.next_token();
                function_type  = Type::Number;
             } 
            Token::StringKeyword => {
                self.tokenizer.next_token();
                function_type = Type::String; 
            } 
            Token::BoolKeyword  => {
                self.tokenizer.next_token();
                function_type  = Type::Bool;
             }
            _ => {
                panic!("[Error]: Function Declaration Must Has Return Type, But Got {:?}", self.tokenizer.get_token())
            }
        }
        let body = self.parse_block_statement();
        return Declaration::Function(FunctionDeclaration { 
            name:function_name , 
            return_type: function_type, 
            argument: params , 
            body
        })

    }
    fn parse_function_declaration_params(&mut self) -> Vec<FunctionParam> {
        match self.tokenizer.get_token() {
            Token::ParenthesesLeft => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Function Declaration Params Must Be Wrapped In ParenthesesLeft, But Got {:?}", self.tokenizer.get_token())
            }
        }
        let mut params = Vec::<FunctionParam>::new();
        loop {
            let param_name: String;
            let param_type: Type;
            match self.tokenizer.get_token() {
                Token::Identifier(name) => {
                    param_name = name;
                    self.tokenizer.next_token();
                }
                _ => {
                    break;
                }
            }
            match self.tokenizer.get_token() {
                Token::Colon => {
                    self.tokenizer.next_token();
                }
                _ => {
                    panic!("[Error]: Function Param Must Has A Type With Colon. But Got {:?}", self.tokenizer.get_token());
                }
            }
            match self.tokenizer.get_token() {
                Token::NullKeyword => { 
                    self.tokenizer.next_token();
                    param_type = Type::Null;
                }
                Token::NumberKeyword  => {
                    self.tokenizer.next_token();
                    param_type = Type::Number; 
                } 
                Token::StringKeyword => { 
                    self.tokenizer.next_token();
                    param_type = Type::String; 
                } 
                Token::BoolKeyword  => { 
                    self.tokenizer.next_token();
                    param_type = Type::Bool;
                }
                _ => {
                    panic!("[Error]: Function Param Must Has A Type. But Got {:?}", self.tokenizer.get_token());
                }
            }
            params.push(FunctionParam { name: param_name, argument_type: param_type });
            match self.tokenizer.get_token() {
                Token::Comma => {
                    self.tokenizer.next_token();
                }
                _ => {
                    break
                }
            }
        }
        match self.tokenizer.get_token() {
            Token::ParenthesesRight => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: Function Declaration Params Must Be Wrapped In ParenthesesRight, But Got {:?}", self.tokenizer.get_token())
            }
        }
        return params;
    }
    fn parse_expression(&mut self) -> Expression {
        let mut expressions = vec![self.parse_assignment_expression()];
        loop {
            match self.tokenizer.get_token() {
                Token::Comma => {
                    self.tokenizer.next_token();
                    expressions.push(self.parse_assignment_expression())
                }
                _ => {
                    break;
                }
            }
        }
        if expressions.len() == 1 {
            return expressions[0].clone()
        }else {
            return Expression::SequnceExpr(SequnceExpression {
                expressions
            })
        }
    }
    fn parse_assignment_expression(&mut self) -> Expression {
        let left = self.parse_condition_expression();
        return match self.tokenizer.get_token() {
            Token::Assign => {
                self.tokenizer.next_token();
                Expression::AssigmentExpr(AssigmentExpression { 
                    left: Box::<Expression>::new(left), 
                    right: Box::<Expression>::new(self.parse_condition_expression()), 
                    operator: String::from("="),
                }) 
            }
            _ => {
                left
            }
        }
    }
    fn parse_condition_expression(&mut self) -> Expression {
        let test = self.parse_binary_expression();
        match self.tokenizer.get_token() {
            Token::Qustion => {
                self.tokenizer.next_token();
            }
            _ => {
                return test;
            }
        }
        let consequence =  self.parse_binary_expression();
        match self.tokenizer.get_token() {
            Token::Colon => {
                self.tokenizer.next_token();
                return Expression::ConditionExpr(ConditionExpression {
                    test: Box::<Expression>::new(test),
                    consequnce: Box::<Expression>::new(consequence),
                    alter:Box::<Expression>::new(self.parse_binary_expression())

                })
            }
            _ => {
                panic!("[Error]: Conditional Expression Should And Consequnce And Alter Expression");
            }
        }
    }
    fn parse_binary_expression(&mut self)-> Expression {
        let atom = self.parse_unary_expression();
        let op = self.tokenizer.get_token();
        if is_binary_op(&op) {
            return self.parse_binary_ops(atom, -1)
        }
        return atom;
    }
    fn parse_binary_ops(&mut self, mut left: Expression, last_pre: i32) -> Expression {
        loop {
            let current_op = self.tokenizer.get_token();
            if !is_binary_op(&current_op) || get_pre_of_binary_op(&current_op) < last_pre {
                break;
            }
            self.tokenizer.next_token();
            let mut right = self.parse_unary_expression();
            let next_op = self.tokenizer.get_token();
            if  
                is_binary_op(&next_op) && 
                (get_pre_of_binary_op(&next_op) > get_pre_of_binary_op(&current_op)) 
            {
                
                right = self.parse_binary_ops(right, get_pre_of_binary_op(&next_op))
            }
            left = Expression::BinaryExpr(BinaryExpression { 
                left: Box::<Expression>::new(left.clone()), 
                right: Box::<Expression>::new(right.clone()), 
                operator: get_text_of_binary_op(&current_op)
            } );
        }
        return left;
    }
    fn parse_unary_expression(&mut self) -> Expression {
        return match self.tokenizer.get_token() {
            Token::Plus => {
                self.tokenizer.next_token();
                Expression::UnaryExpr(UnaryExpression {
                    operator: String::from("+"),
                    argument: Box::<Expression>::new(self.parse_primary_expression())
                })
            }
            Token::Minus => {
                self.tokenizer.next_token();
                Expression::UnaryExpr(UnaryExpression {
                    operator: String::from("-"),
                    argument: Box::<Expression>::new(self.parse_primary_expression())
                })
            }
            _ => {
                self.parse_primary_expression()
            }
        }
    }
    fn parse_primary_expression(&mut self) -> Expression {
        match self.tokenizer.get_token() {
            Token::Identifier(identifier) => {
                self.tokenizer.next_token();
                match self.tokenizer.get_token() {
                    Token::ParenthesesLeft => {
                        let params = self.parse_call_expression_param();
                        Expression::CallExpr(CallExpression { callee_name: identifier, params })
                    }
                    _ => {
                        Expression::Ident(Identifier {
                            name: identifier,
                        })
                    }
                }
            }
            Token::NumberLiteral(value) => {
                self.tokenizer.next_token();
                Expression::Number(NumberLiteral{
                    value
                })
            }
            Token::StringLiteral(value) => {
                self.tokenizer.next_token();
                Expression::String(StringLiteral {
                    value
                })
            }
            Token::BoolLiteral(value) => {
                self.tokenizer.next_token();
                Expression::Bool(BoolLiteral {
                    value
                })
            }
            Token::NullKeyword => {
                self.tokenizer.next_token();
                Expression::Null(NullLiteral {})
            }
            Token::ParenthesesLeft => {
                self.tokenizer.next_token();
                let expr = self.parse_expression();
                return match self.tokenizer.get_token() {
                    Token::ParenthesesRight => {
                        self.tokenizer.next_token();
                        expr
                    }
                    _ => {
                        panic!("[Error]: CoverParenthesizedExpression Must End With ParentheseRight, But Got {:?}", self.tokenizer.get_token());
                    }
                }
            }
            _ => {
                panic!("[Error]: Failed For Get Primary Expression, Unexecpted Token {:?}.", self.tokenizer.get_token())
            }
        }
    }
    fn parse_call_expression_param(&mut self) -> Vec<Expression> {
        let mut params = Vec::<Expression>::new();
        match self.tokenizer.get_token() {
            Token::ParenthesesLeft  => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: CallExpression's Param Call Be Wrapped By ParentheseLeft, But Got {:?}", self.tokenizer.get_token());
            }
        }
        loop {
            match self.tokenizer.get_token() {
                Token::ParenthesesRight => {
                    break;
                }
                _ => {
                    params.push(self.parse_condition_expression());
                }
            }
            match self.tokenizer.get_token() {
                Token::Comma => {
                    self.tokenizer.next_token();
                }
                _ => {
                    break;
                }
            }
        }
        match self.tokenizer.get_token() {
            Token::ParenthesesRight  => {
                self.tokenizer.next_token();
            }
            _ => {
                panic!("[Error]: CallExpression's Param Call Be Wrapped By ParentheseRight, But Got {:?}", self.tokenizer.get_token());
            }
        }
        return params;
    }
}