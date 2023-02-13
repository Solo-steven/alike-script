#[cfg(test)] 
mod test {
    use crate::Tokenizer;
    use crate::token::Token;
    /***************************************************************************
     *  Test Basic Char Operation
     * *************************************************************************
     */
    #[test] 
    fn test_get_eat_char() {
        let mut tokenizer = Tokenizer::new(String::from("Test"));
        let mut get_chars = Vec::<char>::new();
        let mut i = 0 ;
        while i < 4 {
            match tokenizer.get_char() {
                None => {
                    panic!("Error, Shouldn't has error")
                }
                Some(ch) => {
                    get_chars.push(ch);
                    tokenizer.eat_char(1)
                }
            }
            i += 1;
        }
        assert_eq!(get_chars, vec!['T', 'e', 's', 't']);
        assert_eq!(tokenizer.get_char(), None);
    }
    /***************************************************************************
     *  Test Lookahead Actions
     * *************************************************************************
     */
    #[test]
    fn test_lookahead() {
        let mut tokenizer = Tokenizer::new(String::from("var id = 10;"));

        assert_eq!(tokenizer.next_token(), Token::VarKeyword);
        let last_position = tokenizer.get_position();
        assert_eq!(tokenizer.lookahead(), Token::Identifier(String::from("id")));
        assert_eq!(tokenizer.get_position(), last_position);
        assert_eq!(tokenizer.next_token(),Token::Identifier(String::from("id")))
    }
    /***************************************************************************
     *  Test Operator Actions
     * *************************************************************************
     */
    #[test]
    fn test_plus_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("+"));
        assert_eq!(tokenizer.toknize(), Token::Plus);
        assert_eq!(tokenizer.toknize(), Token::EOF);
    }
    #[test]
    #[should_panic]
    fn test_plus_assigment_should_failed() {
        let mut tokenizer = Tokenizer::new(String::from("+="));
        assert_eq!(tokenizer.toknize(), Token::Plus);
    }
    #[test]
    #[should_panic]
    fn test_plusplus_update_operator_should_failed() {
        let mut tokenizer = Tokenizer::new(String::from("++"));
        assert_eq!(tokenizer.toknize(), Token::Plus);
    }
    /***************************************************************************
     *  Test Lieral Tokenize
     * *************************************************************************
     */
    #[test]
    fn test_string_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("\' Test String Literal XD \'"));
        assert_eq!(tokenizer.next_token(), Token::StringLiteral(String::from(" Test String Literal XD ")));
    }
    #[test]
    #[should_panic]
    fn test_string_should_failed() {
        let mut tokenizer = Tokenizer::new(String::from("\' Test String Literal XD \""));
        assert_eq!(tokenizer.next_token(), Token::StringLiteral(String::from(" Test String Literal XD ")));
    }
    #[test]
    /***************************************************************************
     *  Test Statement Tokenize
     * *************************************************************************
     */
    fn test_while_loop_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("
            while (n<=10) {
                n = n + 1;
            }
        "));
        assert_eq!(tokenizer.next_token(), Token::WhileKeyword);
        assert_eq!(tokenizer.next_token(), Token::ParenthesesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Lteq);
        assert_eq!(tokenizer.next_token(), Token::NumberLiteral(10.));
        assert_eq!(tokenizer.next_token(), Token::ParenthesesRight);
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Assign);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Plus);
        assert_eq!(tokenizer.next_token(), Token::NumberLiteral(10.));
        assert_eq!(tokenizer.next_token(), Token::Semi);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::EOF)
    }
    #[test]
    fn test_if_else_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("
            if (n == 1) {
                n + m;
            } else {
                n - m;
            }
        "));
        assert_eq!(tokenizer.next_token(), Token::IfKeyword);
        assert_eq!(tokenizer.next_token(), Token::ParenthesesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Eq);
        assert_eq!(tokenizer.next_token(), Token::NumberLiteral(10.));
        assert_eq!(tokenizer.next_token(), Token::ParenthesesRight);
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Plus);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("m")));
        assert_eq!(tokenizer.next_token(), Token::Semi);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::ElesKeyword);
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("n")));
        assert_eq!(tokenizer.next_token(), Token::Minus);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("m")));
        assert_eq!(tokenizer.next_token(), Token::Semi);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::EOF);
    }
    #[test]
    fn test_variable_declaration_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("
            var test number 
        "));
        assert_eq!(tokenizer.next_token(), Token::VarKeyword);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("test")));
        assert_eq!(tokenizer.next_token(), Token::NumberKeyword);
        assert_eq!(tokenizer.next_token(), Token::EOF);
    }
    #[test]
    fn test_block_statement_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("
            {
                {

                }
            }
        "));
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::EOF);
    }
    /***************************************************************************
     *  Test Declaration Tokenize
     * *************************************************************************
     */
    #[test]
    fn test_function_declaration_tokenize() {
        let mut tokenizer = Tokenizer::new(String::from("
            function Test(a: number) {
            }
        "));
        assert_eq!(tokenizer.next_token(), Token::FunctionKeyword);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("Test")));
        assert_eq!(tokenizer.next_token(), Token::ParenthesesLeft);
        assert_eq!(tokenizer.next_token(), Token::Identifier(String::from("a")));
        assert_eq!(tokenizer.next_token(), Token::Colon);
        assert_eq!(tokenizer.next_token(), Token::NumberKeyword);
        assert_eq!(tokenizer.next_token(), Token::ParenthesesRight);
        assert_eq!(tokenizer.next_token(), Token::BracesLeft);
        assert_eq!(tokenizer.next_token(), Token::BracesRight);
        assert_eq!(tokenizer.next_token(), Token::EOF);

    }
}
