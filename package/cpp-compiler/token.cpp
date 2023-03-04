#ifndef CPP_COMPILER_TOKEN
#define CPP_COMPILER_TOKEN

enum TokenKinds {
    StartState,
    // Ident
    Identifier,
    // Keywords
    WhileKeyword,   // while
    ForKeyword,     // for
    IfKeyword,      // if
    ElesKeyword,    // else
    ReturnKeyword,  // return
    VarKeyword,     // var
    FunctionKeyword, // function
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
    Multiply,        // *
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
    // BitwiseOR,      // |
    // BitwiseAND,     // &
    // BitwiseNOT,     // ~
    // BitwiseXOR,     // ^
    // BitwiseLeftShift,   // <<
    // BitwiseRightShift,  // >>
    // Literal
    BoolLiteral,      // true or false
    StringLiteral,  // String Data type
    NumberLiteral,     // float64
    NullLiteral,            // null Value
    // EOF
    EOFToken,  
};


int get_binary_op_precedence(TokenKinds kind) {
    switch (kind)
    {
    case TokenKinds::LogicalOR:
        return 4;
    case TokenKinds::LogicalAND:
        return 5;
    case TokenKinds::Eq:
    case TokenKinds::NotEq:
        return 9;
    case TokenKinds::Gt:
    case TokenKinds::Lt:
    case TokenKinds::Gteq:
    case TokenKinds::Lteq:
        return 10;
    case TokenKinds::Minus:
    case TokenKinds::Plus:
        return 12;
    case TokenKinds::Multiply:
    case TokenKinds::Mod:
    case TokenKinds::Divide:
        return 13;
    default:
        return -1;
    }
}

bool is_binary_op(TokenKinds kind) {
    auto n = get_binary_op_precedence(kind);
    return n > -1;
}

std::string binary_op_to_string(TokenKinds kind) {
    switch (kind)
    {
    case TokenKinds::LogicalOR:
        return std::string("||");
    case TokenKinds::LogicalAND:
        return std::string("&&");
    case TokenKinds::Eq:
        return std::string("==");
    case TokenKinds::NotEq:
        return std::string("!=");
    case TokenKinds::Gt:
        return std::string(">");
    case TokenKinds::Lt:
        return std::string("<");
    case TokenKinds::Gteq:
        return std::string(">=");
    case TokenKinds::Lteq:
        return std::string("<=");
    case TokenKinds::Minus:
        return std::string("-");
    case TokenKinds::Plus:
        return std::string("+");
    case TokenKinds::Multiply:
        return std::string("*");
    case TokenKinds::Mod:
        return std::string("%");
    case TokenKinds::Divide:
        return std::string("/");
    default:
        throw "";
    }
}

std::string unary_op_to_string(TokenKinds kind) {
    switch (kind)
    {
    case TokenKinds::LogicalNOT:
        return std::string("!");
    case TokenKinds::Minus:
        return std::string("-");
    case TokenKinds::Plus:
        return std::string("+");
    default:
        throw "";
    }
}


#endif
