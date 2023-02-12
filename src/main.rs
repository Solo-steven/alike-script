
mod tokenizer;
mod token;
mod parser;
mod ast;
mod utils;

use tokenizer::*;

use std::collections::HashSet;

fn main() {
    let mut tokenizer = Tokenizer::new(String::from("  
        var id number;
    "));
    let a: HashSet<u16> = vec![1,2,3,4].into_iter().collect();
    println!("{:?}", tokenizer.next_token());
    println!("{:?}", tokenizer.next_token());
    println!("{:?}", tokenizer.next_token());
    println!("{:?}", tokenizer.next_token());
}
