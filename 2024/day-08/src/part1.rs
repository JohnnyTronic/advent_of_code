use itertools::Itertools;
use std::{collections::HashSet, io::Error, str::FromStr};

use crate::board::Board;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut antinode_positions = HashSet::new();

    let board = Board::from_str(input).expect("Error parsing board");

    for antenna_char in board.antenna_map.keys() {
        let antenna_list = board.antenna_map.get(antenna_char).unwrap();
        for combo in antenna_list.iter().combinations(2) {
            let antenna_a = combo[0].as_ivec2();
            let antenna_b = combo[1].as_ivec2();
            let diff = antenna_b - antenna_a;

            let antinode_pos_a = antenna_a - diff;
            let antinode_pos_b = antenna_b + diff;
            if board.is_in_bounds(antinode_pos_a) {
                antinode_positions.insert(antinode_pos_a);
            }
            if board.is_in_bounds(antinode_pos_b) {
                antinode_positions.insert(antinode_pos_b);
            }
        }
    }

    let total_antinodes = antinode_positions.len();
    Ok(total_antinodes.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("14", process(input)?);
        Ok(())
    }
}
