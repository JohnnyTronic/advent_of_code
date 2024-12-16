use std::{collections::HashMap, str::FromStr};

use glam::IVec2;

pub struct WorldState {
    pub width: usize,
    pub height: usize,
    pub terrain: HashMap<IVec2, char>,
    pub box_positions: Vec<IVec2>,
    pub robot_position: IVec2,
}

impl WorldState {
    pub fn simulate(&mut self, robot_movement: &IVec2) {}

    pub fn print_map(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let pos = IVec2::new(x as i32, y as i32);
                if self.robot_position == pos {
                    print!("@");
                } else if self
                    .box_positions
                    .iter()
                    .any(|box_position| box_position == &pos)
                {
                    print!("O");
                } else {
                    print!(
                        "{}",
                        self.terrain.get(&IVec2::new(x as i32, y as i32)).unwrap()
                    );
                }
            }
            println!();
        }
    }
}

#[derive(Debug)]
pub struct ParseWorldStateError {}

impl FromStr for WorldState {
    type Err = ParseWorldStateError;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        let mut first_split = input.split("\n\n");
        let map_input = first_split.next().unwrap();
        let instruction_input = first_split.next().unwrap();

        let mut world_state = WorldState {
            width: map_input.lines().next().unwrap().chars().count(),
            height: map_input.lines().count(),
            terrain: HashMap::new(),
            box_positions: vec![],
            robot_position: IVec2::new(0, 0),
        };

        for (y, row) in map_input.lines().enumerate() {
            for (x, char) in row.chars().enumerate() {
                let x = x as i32;
                let y = y as i32;
                match char {
                    '#' => {
                        world_state.terrain.insert(IVec2::new(x, y), '#');
                    }
                    '.' => {
                        world_state.terrain.insert(IVec2::new(x, y), '.');
                    }
                    'O' => {
                        world_state.box_positions.push(IVec2::new(x, y));
                    }
                    '@' => {
                        world_state.robot_position = IVec2::new(x, y);
                    }
                    _ => {
                        panic!("Unexpected terrain type: [{}]", char);
                    }
                }
                world_state.terrain.insert(IVec2::new(x, y), char);
            }
        }

        Ok(world_state)
    }
}
