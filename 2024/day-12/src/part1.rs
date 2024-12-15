use glam::UVec2;
use std::{collections::HashSet, io::Error, str::FromStr};

use crate::{region::Region, terrain::Terrain, CARDINAL_OFFSETS};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut regions: Vec<Region> = vec![];

    let terrain: Terrain = Terrain::from_str(input).expect("Input parse error");

    for (y, row) in input.lines().enumerate() {
        for (x, char) in row.chars().enumerate() {
            let position = UVec2::new(x as u32, y as u32);
            let target_region = regions
                .iter()
                .find(|region| region.cells.contains(&position));
            match target_region {
                None => {
                    let mut new_region = Region {
                        char,
                        cells: HashSet::new(),
                        perimeter: 0,
                        side_pieces: vec![],
                    };
                    let position = UVec2::new(x as u32, y as u32);
                    new_region.cells.insert(position);
                    scan_neighbours(&position, &mut new_region, &terrain);
                    regions.push(new_region);
                }
                Some(_region) => {
                    // This terrain cell has already been added to a region
                }
            };
        }
    }

    let mut total_fence_cost = 0;
    for region in regions {
        dbg!(&region);
        let region_fence_cost = region.cells.len() * region.perimeter;
        total_fence_cost += region_fence_cost;
    }

    Ok(total_fence_cost.to_string())
}

fn scan_neighbours(position: &UVec2, active_region: &mut Region, terrain: &Terrain) {
    for offset in CARDINAL_OFFSETS {
        let neighbour_position = offset + position.as_ivec2();

        match terrain.cells.get(&neighbour_position.as_uvec2()) {
            None => {
                active_region.perimeter += 1;
            }
            Some(neighbour_char) => {
                if &active_region.char == neighbour_char {
                    if !&active_region.cells.contains(&neighbour_position.as_uvec2()) {
                        active_region.cells.insert(neighbour_position.as_uvec2());
                        scan_neighbours(&neighbour_position.as_uvec2(), active_region, terrain);
                    }
                } else {
                    active_region.perimeter += 1;
                    continue;
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("1930", process(input)?);
        Ok(())
    }
}
