use std::{collections::HashSet, io::Error, str::FromStr};

use glam::{IVec2, UVec2};

use crate::{
    region::{Region, SidePiece},
    terrain::Terrain,
    CARDINAL_OFFSETS,
};

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
    for region in &mut regions {
        println!("Calculating total fence cost or region [{}]", region.char);
        // Collect side pieces into contiguous sides
        let mut sides: Vec<Vec<SidePiece>> = vec![];

        println!("Starting side piece count: {}", region.side_pieces.len());
        while let Some(starting_piece) = region.side_pieces.pop() {
            if region.char == 'I' {
                println!("For starting_piece: {:?}", starting_piece);
            }
            let mut new_side = vec![starting_piece.clone()];

            let walk_direction_a = starting_piece.heading.perp();
            let walk_direction_b = walk_direction_a.saturating_mul(IVec2::new(-1, -1));

            {
                let mut walk_position_a = starting_piece.position.as_ivec2() + walk_direction_a;
                if region.char == 'I' {
                    println!("walk_position_a: {:?}", walk_position_a);
                }
                while let Some(walk_neighbour_a_index) =
                    region.side_pieces.iter().position(|piece| {
                        piece.heading == starting_piece.heading
                            && piece.position.as_ivec2() == walk_position_a
                    })
                {
                    let removed = region.side_pieces.remove(walk_neighbour_a_index);
                    if region.char == 'I' {
                        println!("Removed walked piece: {:?}", removed);
                    }
                    new_side.push(removed);
                    walk_position_a += walk_direction_a;
                }
            }
            {
                let mut walk_position_b = starting_piece.position.as_ivec2() + walk_direction_b;
                if region.char == 'I' {
                    println!("walk_position_b: {:?}", walk_position_b);
                }
                while let Some(walk_neighbour_b_index) =
                    region.side_pieces.iter().position(|piece| {
                        piece.heading == starting_piece.heading
                            && piece.position.as_ivec2() == walk_position_b
                    })
                {
                    let removed = region.side_pieces.remove(walk_neighbour_b_index);
                    if region.char == 'I' {
                        println!("Removed walked piece: {:?}", removed);
                    }
                    new_side.push(removed);
                    walk_position_b += walk_direction_b;
                }
            }
            sides.push(new_side);
        }

        total_fence_cost += region.cells.len() * sides.len();
    }

    Ok(total_fence_cost.to_string())
}
fn scan_neighbours(position: &UVec2, active_region: &mut Region, terrain: &Terrain) {
    for offset in CARDINAL_OFFSETS {
        let neighbour_position = offset + position.as_ivec2();

        match terrain.cells.get(&neighbour_position.as_uvec2()) {
            None => {
                active_region.perimeter += 1;
                let side_piece = SidePiece {
                    position: *position,
                    heading: offset,
                };
                active_region.side_pieces.push(side_piece);
            }
            Some(neighbour_char) => {
                if &active_region.char == neighbour_char {
                    if !&active_region.cells.contains(&neighbour_position.as_uvec2()) {
                        active_region.cells.insert(neighbour_position.as_uvec2());
                        scan_neighbours(&neighbour_position.as_uvec2(), active_region, terrain);
                    }
                } else {
                    active_region.perimeter += 1;
                    let side_piece = SidePiece {
                        position: *position,
                        heading: offset,
                    };
                    active_region.side_pieces.push(side_piece);
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
        assert_eq!("1206", process(input)?);
        Ok(())
    }
}
