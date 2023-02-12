use crate::Tokenizer;
use crate::token::Token;
use crate::ast::*;

pub struct  Parser {
    tokenizer: Tokenizer
}

impl Parser {
    pub fn new(code: String) -> Parser{
        Parser {
            tokenizer: Tokenizer::new(code)
        }
    }
}