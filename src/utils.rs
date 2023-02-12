#[derive(Debug, PartialEq, Clone)]
pub struct Position {
    pub row: usize, 
    pub col: usize,
}
#[derive(Debug, PartialEq, Clone)]
pub struct Location {
    start: Position,
    end: Position
}