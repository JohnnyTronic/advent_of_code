use std::{collections::HashMap, str::FromStr};

use glam::{IVec2, UVec2};

#[derive(Debug, Clone)]
pub struct Board {
    pub cells: Vec<Vec<char>>,
    pub row_count: usize,
    pub column_count: usize,
    pub antenna_map: HashMap<char, Vec<UVec2>>,
}

impl Board {
    pub fn get(&self, position: UVec2) -> Option<&char> {
        let row = self.cells.get(position.y as usize)?;
        row.get(position.x as usize)
    }

    pub fn is_in_bounds(&self, position: IVec2) -> bool {
        position.x >= 0
            && position.x < (self.column_count as i32)
            && position.y >= 0
            && position.y < (self.row_count as i32)
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParseBoardError;

impl FromStr for Board {
    type Err = ParseBoardError;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        let row_count = input.lines().count();
        let column_count = input.lines().next().unwrap().len();

        let mut board = Board {
            cells: vec![],
            row_count,
            column_count,
            antenna_map: HashMap::new(),
        };

        for (y, line) in input.lines().enumerate() {
            let mut new_row: Vec<char> = vec![];
            for (x, char) in line.chars().enumerate() {
                new_row.push(char);
                if char != '.' {
                    board.antenna_map.entry(char).or_default();
                    board
                        .antenna_map
                        .get_mut(&char)
                        .unwrap()
                        .push(UVec2::new(x as u32, y as u32));
                }
            }
            board.cells.push(new_row);
        }

        Ok(board)
    }
}
