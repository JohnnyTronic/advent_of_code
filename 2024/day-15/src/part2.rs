use std::{io::Error, str::FromStr};

use crate::{big_world_state::BigWorldState, parse_instructions, world_state::WorldState};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let world_state = WorldState::from_str(input).expect("WorldState parse error");
    let mut big_world_state = BigWorldState::from_world_state(&world_state);
    let instructions = parse_instructions(input);

    for instruction in instructions {
        big_world_state.simulate(&instruction);
        big_world_state.print_map();
    }

    let mut box_gps_coordinate_sum = 0;
    for box_position in world_state.box_positions {
        let gps_score = 100 * box_position.y + box_position.x;
        box_gps_coordinate_sum += gps_score;
    }
    Ok(box_gps_coordinate_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("9021", process(input)?);
        Ok(())
    }
}
