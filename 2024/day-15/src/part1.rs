use std::{io::Error, str::FromStr};

use glam::IVec2;

use crate::world_state::WorldState;

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

fn parse_instructions(input: &str) -> Vec<IVec2> {
    let instruction_input = input.split("\n\n").nth(1).unwrap();

    let mut instructions: Vec<IVec2> = vec![];
    for line in instruction_input.lines() {
        for char in line.chars() {
            match char {
                '<' => instructions.push(IVec2::NEG_X),
                '^' => instructions.push(IVec2::NEG_Y),
                '>' => instructions.push(IVec2::X),
                'v' => instructions.push(IVec2::Y),
                _ => panic!("Unexpected instruction char: [{}]", char),
            }
        }
    }

    instructions
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
