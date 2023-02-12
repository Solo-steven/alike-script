#[derive(Debug, PartialEq, Clone)]
pub enum Token {
    Identifier(String),
    // Keywords
    WhileKeyword,   // while
    ForKeyword,     // for
    IfKeyword,      // if
    ElesKeyword,    // else
    ReturnKeyword,  // return
    VarKeyword,     // var
    NumberKeyword,  // number
    StringKeyword,  // string
    BoolKeyword,    // bool
    NullKeyword,    // null
    // Punctuation
    Semi,           // ;
    Colon,          // :
    Comma,          // ,
    BracesLeft,     // {
    BracesRight,    // }
    BracketLeft,    // [
    BracketRight,   // ]
    ParenthesesLeft,    // (
    ParenthesesRight,   // )
    HashTag,        // #
    Dot,            // .
    Qustion,        // ?
    // Operator
    Plus,           // +
    Minus,           // -
    Multply,        // *
    Divide,         // /
    Mod,            // %
    Assign,         // =
    Eq,             // ==
    NotEq,          // !=
    Gt,             // >
    Lt,             // <
    Gteq,           // >=
    Lteq,           // <=
    LogicalOR,      // ||
    LogicalAND,     // &&
    LogicalNOT,     // !
    // Operator Not Support
    BitwiseOR,      // |
    BitwiseAND,     // &
    BitwiseNOT,     // ~
    BitwiseXOR,     // ^
    BitwiseLeftShift,   // <<
    BitwiseRightShift,  // >>
    // Literal
    BoolLiteral(bool),      // true or false
    StringLiteral(String),  // String Data type
    NumberLiteral(f64),     // float64
    NullLiteral,            // null Value
    // EOF
    EOF,            
}

pub mod RservedWord {
    
}
