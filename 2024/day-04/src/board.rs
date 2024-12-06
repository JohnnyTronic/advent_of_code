use std::fmt::Debug;
use std::iter::Iterator;
use std::ops::Deref;
use std::str::FromStr;

use crate::point::Point;

pub struct Board {
    data: Vec<Vec<char>>,
    row_count: usize,
    column_count: usize,
}

impl Board {
    pub fn new(rows: usize, columns: usize, default_char: char) -> Self {
        Board {
            data: vec![vec![default_char; columns]; rows],
            row_count: rows,
            column_count: columns,
        }
    }
    // pub fn get(&self, x: usize, y: usize) -> char {
    //     self.data[x][y]
    // }

    pub fn get(&self, position: &Point) -> Option<char> {
        if !self.check_is_in_bounds(position.x, position.y) {
            return None;
        }

        Some(self.data[position.x as usize][position.y as usize])
    }

    pub fn set(&mut self, x: usize, y: usize, new_char: char) {
        self.data[x][y] = new_char;
    }

    pub fn check_is_in_bounds(&self, x: isize, y: isize) -> bool {
        if x < 0 {
            return false;
        }

        if y < 0 {
            return false;
        }

        if x as usize >= self.column_count {
            return false;
        }

        if y as usize >= self.row_count {
            return false;
        }

        true
    }
}

#[derive(Debug)]
pub struct ParseBoardError;

impl FromStr for Board {
    type Err = ParseBoardError;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        let row_count: usize = input.lines().count();
        let column_count = input.lines().next().unwrap().chars().count();

        dbg!(row_count, column_count);

        let mut board: Board = Board::new(row_count, column_count, '.');

        for (row, line) in input.lines().enumerate() {
            for (col, char) in line.chars().enumerate() {
                board.set(row, col, char);
            }
        }

        Ok(board)
    }
}

impl Deref for Board {
    type Target = Vec<Vec<char>>;

    fn deref(&self) -> &Self::Target {
        &self.data
    }
}
