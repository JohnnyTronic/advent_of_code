use crate::{board::Board, point::Point};
use std::{io::Error, str::FromStr};

static ALL_DIRECTIONS: [Point; 8] = [
    Point { x: 1, y: 0 },
    Point { x: 1, y: 1 },
    Point { x: 0, y: 1 },
    Point { x: -1, y: 1 },
    Point { x: -1, y: 0 },
    Point { x: -1, y: -1 },
    Point { x: 0, y: -1 },
    Point { x: 1, y: -1 },
];

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let mut xmas_count = 0;

    let board = Board::from_str(input).expect("Could not parse Board from input string");

    for (x, row) in board.iter().enumerate() {
        for (y, _) in row.iter().enumerate() {
            println!("find_xmas at x {} y {}", x, y);
            xmas_count += find_xmas(&board, x, y);
        }
    }

    Ok(xmas_count.to_string())
}

fn find_xmas(board: &Board, x: usize, y: usize) -> usize {
    if board[x][y] == 'X' {
        search_all_directions(board, x, y)
    } else {
        0
    }
}

struct Seeker<'a, 'b> {
    board: &'a Board,
    position: Point,
    offset: &'b Point,
    remaining_chars: String,
}

impl Seeker<'_, '_> {
    fn seek(&mut self) -> bool {
        if self.remaining_chars.is_empty() {
            return true;
        }
        let target_char = self.remaining_chars.remove(0);
        let next_position = &self.position + self.offset;
        let next_char = self.board.get(&next_position);
        let found_match = match next_char {
            None => false,
            Some(char) => char == target_char,
        };

        if found_match {
            self.position = next_position;
            self.seek()
        } else {
            false
        }
    }
}

fn search_all_directions(board: &Board, x: usize, y: usize) -> usize {
    let mut found_xmas_count = 0;
    for offset in ALL_DIRECTIONS.iter() {
        let mut seeker = Seeker {
            board,
            position: Point {
                x: x.try_into().unwrap(),
                y: y.try_into().unwrap(),
            },
            offset,
            remaining_chars: "MAS".into(),
        };
        if seeker.seek() {
            found_xmas_count += 1;
        }
    }
    found_xmas_count
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("18", process(input)?);
        Ok(())
    }
}
