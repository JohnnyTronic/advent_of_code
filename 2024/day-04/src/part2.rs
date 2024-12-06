use crate::{board::Board, point::Point};
use std::{io::Error, str::FromStr};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let mut crossmas_count = 0;

    let board = Board::from_str(input).expect("Could not parse Board from input string");

    for (x, row) in board.iter().enumerate() {
        for (y, _) in row.iter().enumerate() {
            println!("find_xmas at x {} y {}", x, y);
            let pos = Point {
                x: x as isize,
                y: y as isize,
            };
            if is_crossmas_position(&board, &pos) {
                crossmas_count += 1;
            }
        }
    }

    Ok(crossmas_count.to_string())
}

fn is_crossmas_position(board: &Board, position: &Point) -> bool {
    match board.get(position) {
        None => false,
        Some(char) => {
            if char == 'A' {
                check_northeast(board, position) && check_southeast(board, position)
            } else {
                false
            }
        }
    }
}

fn check_northeast(board: &Board, position: &Point) -> bool {
    let northeast_offset = Point { x: 1, y: -1 };
    let southwest_offset = Point { x: -1, y: 1 };

    let Some(northeast_char) = board.get(position + &northeast_offset) else {
        return false;
    };
    let Some(southwest_char) = board.get(position + &southwest_offset) else {
        return false;
    };

    if northeast_char == 'M' && southwest_char == 'S' {
        return true;
    }

    if northeast_char == 'S' && southwest_char == 'M' {
        return true;
    }

    false
}

fn check_southeast(board: &Board, position: &Point) -> bool {
    let southeast_offset = Point { x: 1, y: 1 };
    let northwest_offset = Point { x: -1, y: -1 };

    let Some(southeast_char) = board.get(position + &southeast_offset) else {
        return false;
    };
    let Some(northwest_char) = board.get(position + &northwest_offset) else {
        return false;
    };

    if southeast_char == 'M' && northwest_char == 'S' {
        return true;
    }

    if southeast_char == 'S' && northwest_char == 'M' {
        return true;
    }

    false
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("9", process(input)?);
        Ok(())
    }
}
