use std::str::FromStr;

use crate::{boardcell::BoardCell, point2d::Point2D};

pub struct Board {
    pub cells: Vec<Vec<BoardCell>>,
    pub guard_starting_position: Point2D,
}

impl Board {
    pub fn count_distinct_visited_cells(&self) -> usize {
        let mut visited_count = 0;
        for column in &self.cells {
            for cell in column {
                if cell.visited {
                    visited_count += 1;
                }
            }
        }

        visited_count
    }

    pub fn get(&mut self, position: impl AsRef<Point2D>) -> Option<&mut BoardCell> {
        let x = position.as_ref().x;
        let y = position.as_ref().y;
        if x < 0 || y < 0 {
            return None;
        }

        let x = x as usize;
        let y = y as usize;
        if let Some(column) = self.cells.get_mut(y) {
            column.get_mut(x)
        } else {
            None
        }
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParseBoardError;

impl FromStr for Board {
    type Err = ParseBoardError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let column_count = s.lines().next().unwrap().len();
        let row_count = s.lines().count();
        let cell = BoardCell {
            terrain: '.',
            visited: false,
        };

        let mut board = Board {
            cells: vec![vec![cell; row_count]; column_count],
            guard_starting_position: Point2D { x: 0, y: 0 },
        };

        for (y, row) in s.lines().enumerate() {
            for (x, char) in row.chars().enumerate() {
                match char {
                    '.' => board.cells[y][x].terrain = '.',
                    '#' => board.cells[y][x].terrain = '#',
                    '^' => {
                        board.guard_starting_position.x = x as isize;
                        board.guard_starting_position.y = y as isize;
                    }
                    _ => panic!("Unexpected terrain char: {}", char),
                }
            }
        }

        for (y, row) in board.cells.iter().enumerate() {
            for (x, cell) in row.iter().enumerate() {
                print!("{}", cell.terrain);
            }
            print!("\n");
        }

        Ok(board)
    }

    // for row in s.lines(){
    //
    // }
}
