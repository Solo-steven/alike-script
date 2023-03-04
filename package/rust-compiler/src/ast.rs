// use crate::utils::Location;
/**
 * Entry Structures
 */
#[derive(Debug, PartialEq, Clone)]
pub struct Program {
    pub body: Vec<ProgramItem>
}
#[derive(Debug, PartialEq, Clone)]
pub enum ProgramItem {
    Statement(Statement),
    Declaration(Declaration),
    Expression(Expression)
}
#[derive(Debug, PartialEq, Clone)]
pub enum Declaration {
    Variable(VariableDeclaration),
    Function(FunctionDeclaration)
}
#[derive(Debug, PartialEq, Clone)]
pub enum Statement {
    WhileState(WhileStatement),
    IfState(IfStatement),
    ReturnState(ReturnStatement),
    BlockState(BlockStatement),
}
#[derive(Debug, PartialEq, Clone)]
pub enum Expression {
    Number(NumberLiteral),
    String(StringLiteral),
    Bool(BoolLiteral),
    Null(NullLiteral),
    Ident(Identifier),
    UnaryExpr(UnaryExpression),
    BinaryExpr(BinaryExpression),
    ConditionExpr(ConditionExpression),
    AssigmentExpr(AssigmentExpression),
    SequnceExpr(SequnceExpression),
    CallExpr(CallExpression)
}
/**
 * Expression
 */
#[derive(Debug, PartialEq, Clone)]
pub struct NumberLiteral {
    pub value: f64
}
#[derive(Debug, PartialEq, Clone)]
pub struct  BoolLiteral {
    pub value: bool
}
#[derive(Debug, PartialEq, Clone)]
pub struct  StringLiteral {
    pub value: String
}
#[derive(Debug, PartialEq, Clone)]
pub struct  NullLiteral {}
#[derive(Debug, PartialEq, Clone)]
pub struct Identifier {
    pub name: String,
}
#[derive(Debug, PartialEq, Clone)]
pub struct BinaryExpression {
    pub left: Box<Expression>,
    pub right: Box<Expression>,
    pub operator: String
}
#[derive(Debug, PartialEq, Clone)]
pub struct UnaryExpression {
    pub argument: Box<Expression>,
    pub operator: String,
}
#[derive(Debug, PartialEq, Clone)]
pub struct  ConditionExpression {
    pub test: Box<Expression>,
    pub consequnce: Box<Expression>,
    pub alter: Box<Expression>
}
#[derive(Debug, PartialEq, Clone)]
pub struct  AssigmentExpression {
    pub left: Box<Expression>,
    pub right: Box<Expression>,
    pub operator: String,
}
#[derive(Debug, PartialEq, Clone)]
pub struct  SequnceExpression {
    pub expressions: Vec<Expression>
}
#[derive(Debug, PartialEq, Clone)]
pub struct CallExpression {
    pub callee_name: String,
    pub params: Vec<Expression>, 
}
/**
 * Delcrations
 */
#[derive(Debug, PartialEq, Clone)]
 pub enum Type {
    Number,
    String,
    Bool,
    Null
} 
#[derive(Debug, PartialEq, Clone)]
pub struct VariableDeclaration {
    pub name: String,
    pub variable_type: Type,
    pub init: Option<Expression>
}
#[derive(Debug, PartialEq, Clone)]
pub struct FunctionDeclaration {
    pub name: String,
    pub return_type: Type,
    pub argument: Vec<FunctionParam>,
    pub body: Statement
}
#[derive(Debug, PartialEq, Clone)]
pub struct FunctionParam {
    pub name: String,
    pub argument_type: Type
}
/**
 *  Statement
 */
#[derive(Debug, PartialEq, Clone)]
pub struct WhileStatement {
    pub test: Expression,
    pub body: Box<Statement>
}
#[derive(Debug, PartialEq, Clone)]
pub struct BlockStatement {
   pub body: Vec<ProgramItem>
}
#[derive(Debug, PartialEq, Clone)]
pub struct IfStatement {
    pub test: Expression,
    pub consequent: Box<Statement>,
    pub alter: Option<Box<Statement>>,
}
#[derive(Debug, PartialEq, Clone)]
pub struct  ReturnStatement {
    pub argument: Expression
}


