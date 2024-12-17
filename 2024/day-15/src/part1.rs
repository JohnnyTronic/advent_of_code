use std::{io::Error, str::FromStr};

use crate::{parse_instructions, world_state::WorldState};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut world_state = WorldState::from_str(input).expect("WorldState parse error");
    let instructions = parse_instructions(input);

    for instruction in instructions {
        world_state.simulate(&instruction);
        world_state.print_map();
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
        assert_eq!("10092", process(input)?);
        Ok(())
    }

    #[test]
    fn test_process_small() -> Result<(), Error> {
        let input = include_str!("input-example-small.txt");
        assert_eq!("2028", process(input)?);
        Ok(())
    }
}
