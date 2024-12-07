use std::{io::Error, str::FromStr};

use crate::{board::Board, guard::Guard, point2d::Point2D};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let board = Board::from_str(input).expect("Failed to parse Board from input string");

    let mut board_permutations = 0;

    for (y, row) in board.cells.iter().enumerate() {
        for (x, _cell) in row.iter().enumerate() {
            let target_point = Point2D {
                x: x as isize,
                y: y as isize,
            };
            let mut cloned_board = board.clone();
            let target_cell = cloned_board
                .get(&target_point)
                .expect("How is this not a cell?");
            if target_cell.terrain == '.' && board.guard_starting_position != target_point {
                target_cell.terrain = '#';
                println!("Mutating board @ ({},{})", target_point.x, target_point.y);
            }

            let mut guard = Guard {
                position: board.guard_starting_position.clone(),
                facing: Point2D { x: 0, y: -1 },
            };
            if guard.walk_to_exhaustion(&mut cloned_board) {
                board_permutations += 1;
            }
        }
    }

    Ok(board_permutations.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("6", process(input)?);
        Ok(())
    }
}
