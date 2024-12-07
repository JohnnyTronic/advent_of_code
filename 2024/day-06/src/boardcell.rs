#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BoardCell {
    pub terrain: char,
    pub visited: bool,
}
