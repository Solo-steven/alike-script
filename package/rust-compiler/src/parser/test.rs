#[cfg(test)]
mod test {
    use crate::parser::Parser;
    use crate::ast::*;

    #[test]
    fn test_parse_binary_expression_1() {
        let mut parser = Parser::new(String::from("a+b"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::Ident(Identifier { name: String::from("a") })), 
                    right:  Box::<Expression>::new(Expression::Ident(Identifier { name: String::from("b") })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_binary_expression_2() {
        let mut parser = Parser::new(String::from("a*b+c"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") })), 
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("*"),
                        })), 
                    right:  Box::<Expression>::new(Expression::Ident(Identifier { name: String::from("c") })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_binary_expression_3() {
        let mut parser = Parser::new(String::from("a*b+c*d"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") })), 
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("*"),
                        })), 
                    right:  Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                        left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("c") })), 
                        right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("d") })),
                        operator: String::from("*"),
                    })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_binary_with_unary_expression_1() {
        let mut parser = Parser::new(String::from("a*b+c*-d"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") })), 
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("*"),
                        })), 
                    right:  Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                        left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("c") })), 
                        right: Box::<Expression>::new(Expression::UnaryExpr(UnaryExpression {
                            operator: String::from("-"),
                            argument: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("d") }))
                        })),
                        operator: String::from("*"),
                    })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_binary_with_unary_expression_2() {
        let mut parser = Parser::new(String::from("-a/b+c*-d"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::UnaryExpr(UnaryExpression {
                                operator: String::from("-"),
                                argument: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") }))
                            })),
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("/"),
                        })), 
                    right:  Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                        left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("c") })), 
                        right: Box::<Expression>::new(Expression::UnaryExpr(UnaryExpression {
                            operator: String::from("-"),
                            argument: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("d") }))
                        })),
                        operator: String::from("*"),
                    })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_parentheses_expression_1() {
        let mut parser = Parser::new(String::from("(a+b)+c*d"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") })),
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("+"),
                        })), 
                    right:  Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                        left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("c") })), 
                        right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("d") })),
                        operator: String::from("*"),
                    })), 
                    operator: String::from("+")
                }))
            ]
        });
    }
    #[test]
    fn test_parse_parentheses_expression_2() {
        let mut parser = Parser::new(String::from("-(a+b)+c*d"));
        assert_eq!(parser.parse_program(), Program{
            body: vec![
                ProgramItem::Expression(Expression::BinaryExpr(BinaryExpression { 
                    left:Box::<Expression>::new(Expression::UnaryExpr(UnaryExpression { 
                        operator: String::from("-"),
                        argument: Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                            left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("a") })),
                            right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("b") })),
                            operator: String::from("+"),
                        }))
                    })), 
                    right:  Box::<Expression>::new(Expression::BinaryExpr(BinaryExpression { 
                        left: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("c") })), 
                        right: Box::<Expression>::new(Expression::Ident(Identifier{ name: String::from("d") })),
                        operator: String::from("*"),
                    })), 
                    operator: String::from("+")
                }))
            ]
        });
    }

}


