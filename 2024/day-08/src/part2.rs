use std::{collections::HashSet, io::Error, str::FromStr};

use glam::IVec2;
use itertools::Itertools;

use crate::board::Board;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut antinode_positions = HashSet::new();

    let board = Board::from_str(input).expect("Error parsing board");
    let max_repeats: isize = (std::cmp::max(board.row_count, board.column_count) * 2) as isize;

    for antenna_char in board.antenna_map.keys() {
        let antenna_list = board.antenna_map.get(antenna_char).unwrap();
        for combo in antenna_list.iter().combinations(2) {
            let antenna_a = combo[0].as_ivec2();
            let antenna_b = combo[1].as_ivec2();
            let diff = antenna_b - antenna_a;

            for i in 0..max_repeats {
                let potential_antinode_position =
                    antenna_a - diff.saturating_mul(IVec2::new(i as i32, i as i32));
                if board.is_in_bounds(potential_antinode_position) {
                    antinode_positions.insert(potential_antinode_position);
                } else {
                    break;
                }
            }
            for i in 0..max_repeats {
                let potential_antinode_position =
                    antenna_b + diff.saturating_mul(IVec2::new(i as i32, i as i32));
                if board.is_in_bounds(potential_antinode_position) {
                    antinode_positions.insert(potential_antinode_position);
                } else {
                    break;
                }
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
        assert_eq!("34", process(input)?);
        Ok(())
    }
}
