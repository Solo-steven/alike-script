
mod tokenizer;
mod token;
mod parser;
mod ast;
mod utils;

use tokenizer::*;

fn main() {
    // let mut tokenizer = Tokenizer::new(String::from("  
    //     a + b + c
    // "));
    // let a: HashSet<u16> = vec![1,2,3,4].into_iter().collect();
    // println!("{:?}", tokenizer.next_token());
    // println!("{:?}", tokenizer.next_token());
    // println!("{:?}", tokenizer.next_token());
    // println!("{:?}", tokenizer.next_token());
    // println!("{:?}", tokenizer.next_token());

    let mut parser = parser::Parser::new(String::from("
         c + test(a+b, c ,d * 8) * c
         if(c == 4) {
            
         }

    "));
    println!("{:?}", parser.parse_program());
}
