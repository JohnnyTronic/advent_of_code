use std::{collections::HashSet, io::Error, str::FromStr};

use glam::{IVec2, UVec2};

use crate::terrain::Terrain;

const NORTH: IVec2 = glam::IVec2::NEG_Y;
const EAST: IVec2 = glam::IVec2::X;
const SOUTH: IVec2 = glam::IVec2::Y;
const WEST: IVec2 = glam::IVec2::NEG_X;
const CARDINAL_OFFSETS: [IVec2; 4] = [NORTH, EAST, SOUTH, WEST];

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let terrain = Terrain::from_str(input).expect("Failed to parse terrain");

    let mut score = 0;
    for start_position in &terrain.start_positions {
        let hike_result = hike(&terrain, start_position);
        score += hike_result.len();
    }
    Ok(score.to_string())
}

fn hike(terrain: &Terrain, current_position: &UVec2) -> HashSet<UVec2> {
    let current_cell = terrain
        .get(&current_position.as_ivec2())
        .expect("We're already out of bounds!");

    let mut to_return = HashSet::new();
    for offset in CARDINAL_OFFSETS {
        let next_position: IVec2 = current_position.as_ivec2() + offset;
        if let Some(next_cell) = terrain.get(&next_position) {
            let target_height = *current_cell + 1;
            if next_cell.cmp(&target_height).is_eq() {
                if target_height == 9 {
                    to_return.insert(next_position.as_uvec2());
                    continue;
                }
                let result = hike(terrain, &next_position.as_uvec2());
                to_return.extend(result);
            }
        }
    }

    to_return
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("36", process(input)?);
        Ok(())
    }
}
