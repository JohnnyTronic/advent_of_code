use std::{io::Error, str::FromStr};

use crate::{board::Board, guard::Guard, point2d::Point2D};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut board = Board::from_str(input).expect("Failed to parse Board from input string");

    let mut guard = Guard {
        position: board.guard_starting_position.clone(),
        facing: Point2D { x: 0, y: -1 },
    };

    guard.complete_patrol(&mut board);

    Ok(board.count_distinct_visited_cells().to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("41", process(input)?);
        Ok(())
    }
}
