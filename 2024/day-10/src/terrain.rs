use std::str::FromStr;

use glam::{IVec2, UVec2};
pub type Cell = u8;

#[derive(Debug)]
pub struct Terrain {
    pub cells: Vec<Vec<Cell>>,
    pub height: usize,
    pub width: usize,
    pub start_positions: Vec<UVec2>,
    pub end_positions: Vec<UVec2>,
}

#[derive(Debug)]
pub struct ParseTerrainError {}

impl FromStr for Terrain {
    type Err = ParseTerrainError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let rows = s.lines().count();
        let columns = s.lines().next().unwrap().chars().count();

        let mut terrain = Self {
            cells: vec![],
            height: rows,
            width: columns,
            start_positions: vec![],
            end_positions: vec![],
        };

        for (y, line) in s.lines().enumerate() {
            let mut new_row: Vec<Cell> = vec![];
            for (x, char) in line.chars().enumerate() {
                let value = char.to_digit(10).unwrap() as Cell;
                if value == 0 {
                    terrain.start_positions.push(UVec2 {
                        x: x as u32,
                        y: y as u32,
                    });
                }
                if value == 9 {
                    terrain.end_positions.push(UVec2 {
                        x: x as u32,
                        y: y as u32,
                    });
                }
                new_row.push(value);
            }
            terrain.cells.push(new_row);
        }

        Ok(terrain)
    }
}

impl Terrain {
    pub fn get(&self, position: &IVec2) -> Option<&Cell> {
        let row = self.cells.get(position.y as usize)?;
        let cell = row.get(position.x as usize)?;
        Some(cell)
    }

    pub fn print_map(&self) {
        for row in &self.cells {
            for cell in row {
                print!("{}", cell);
            }
            println!();
        }
    }

    pub fn is_in_bounds(&self, position: IVec2) -> bool {
        position.x >= 0_i32
            && position.x < self.width as i32
            && position.y >= 0_i32
            && position.y < self.height as i32
    }
}
