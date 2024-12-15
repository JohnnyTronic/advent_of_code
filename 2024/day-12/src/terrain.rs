use std::{collections::HashMap, str::FromStr};

use glam::UVec2;

pub struct Terrain {
    pub cells: HashMap<UVec2, char>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParseTerrainError {}

impl FromStr for Terrain {
    type Err = ParseTerrainError;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        // let height = input.lines().count();
        // let width = input.lines().next().unwrap().chars().count();
        let mut cells: HashMap<UVec2, char> = HashMap::new();
        for (y, row) in input.lines().enumerate() {
            for (x, char) in row.chars().enumerate() {
                cells.insert(UVec2::new(x as u32, y as u32), char);
            }
        }
        Ok(Terrain {
            cells,
            // width,
            // height,
        })
    }
}
