use std::collections::HashSet;

use glam::{IVec2, UVec2};

#[derive(Debug, PartialEq, Eq)]
pub struct Region {
    pub char: char,
    pub cells: HashSet<UVec2>,
    pub perimeter: usize,
    pub side_pieces: Vec<SidePiece>,
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct SidePiece {
    pub position: UVec2,
    pub heading: IVec2,
}

pub struct RegionSide<'a> {
    pub pieces: Vec<SidePiece>,
    pub parent_region: &'a Region,
}
