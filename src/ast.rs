use crate::utils::Location;
/**
 * Entry Structures
 */
struct Program {
    body: Vec<ProgramItem>
}
enum ProgramItem {
    Statement(Statement),
    Declaration(Declaration),
    Expression(Expression)
}
enum Declaration {
    Variable(VariableDeclaration),
    Function(FunctionDeclaration)
}
enum Statement {
    WhileStatement,
    ForStatement,
    IfStatement,
    ReturnStatement,
    BlockStatement
}
pub enum Expression {
    Literal,
    Identifier,
    BinaryExpr,
    UnaryExpr,
    ConditionExpr,
    AssigmentExpr,
}
/**
 * Expression
 */
pub struct Identifier {
    name: String,
    start_index: usize,
    end_index: usize,
    location: Location
}
pub struct NumberLiteral {}
pub struct  BoolLiteral {}
pub struct  StringLiteral {}
pub struct  NullLiteral {}
pub enum  Literal {
    Number(NumberLiteral),
    String(StringLiteral),
    Bool(BoolLiteral),
    Null(NullLiteral)
}
/**
 * Delcrations
 */
enum Type {
    Number,
    String,
    Bool,
    Null
} 
struct VariableDeclaration {
    name: String,
    variable_type: Type,
    init: Option<Expression>
}
struct FunctionDeclaration {
    name: String,
    return_type: Type,
    argument: VariableDeclaration
}
/**
 *  Statement
 */
struct WhileStatement {
    test: Expression,
    body: BlockStatement
}
struct BlockStatement {
    body: Vec<ProgramItem>
}


