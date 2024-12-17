use std::{
    collections::{HashMap, HashSet},
    ops::Index,
    str::FromStr,
};

use glam::IVec2;

pub struct WorldState {
    pub width: usize,
    pub height: usize,
    pub terrain: HashMap<IVec2, char>,
    pub box_positions: HashSet<IVec2>,
    pub robot_position: IVec2,
}

enum ShoveResult {
    Success,
    Blocked,
}

impl WorldState {
    pub fn simulate(&mut self, robot_movement: &IVec2) {
        let robot_next_position = self.robot_position + robot_movement;
        let box_in_way = self.box_positions.contains(&robot_next_position);
        if box_in_way {
            // Recurse on box
            let shove_result = self.try_shove(&robot_next_position, robot_movement);
            match shove_result {
                ShoveResult::Blocked => {
                    // Robot does not move
                }
                ShoveResult::Success => {
                    self.robot_position = robot_next_position;
                }
            }
        } else {
            let next_terrain = self.terrain.get(&robot_next_position);
            match next_terrain {
                None => {
                    // We're off the edge, don't move
                    panic!("How did we fall off the edge?");
                }
                Some(char) => {
                    match char {
                        '.' => {
                            // Open space, just move in
                            self.robot_position = robot_next_position;
                        }
                        '#' => {
                            // Wall, can't move
                        }
                        _ => panic!("Unexpected terrain char: [{}]", char),
                    }
                }
            }
        }
    }

    fn try_shove(&mut self, position: &IVec2, direction: &IVec2) -> ShoveResult {
        let next_position = position + direction;
        let next_terrain = self.terrain.get(&next_position);
        match next_terrain {
            None => panic!("Should not be possible"),
            Some(char) => {
                match char {
                    '.' => {
                        if self.box_positions.contains(&next_position) {
                            // Recurse into box
                            let shove_result = self.try_shove(&next_position, direction);
                            match shove_result {
                                ShoveResult::Blocked => return ShoveResult::Blocked,
                                ShoveResult::Success => {
                                    self.box_positions.remove(position);
                                    self.box_positions.insert(next_position);
                                    return ShoveResult::Success;
                                }
                            }
                        }
                        // Space is empty, shove succeeds
                        let remove_success = self.box_positions.remove(position);
                        if !remove_success {
                            panic!("How was there no box at [{position}]");
                        }
                        self.box_positions.insert(next_position);
                        return ShoveResult::Success;
                    }
                    '#' => {
                        // Wall, cannot shove
                        return ShoveResult::Blocked;
                    }
                    _ => panic!("Unexpected terrain [{}]", char),
                }
            }
        }
    }

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

        let mut world_state = WorldState {
            width: map_input.lines().next().unwrap().chars().count(),
            height: map_input.lines().count(),
            terrain: HashMap::new(),
            box_positions: HashSet::new(),
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
                        world_state.box_positions.insert(IVec2::new(x, y));
                        world_state.terrain.insert(IVec2::new(x, y), '.');
                    }
                    '@' => {
                        world_state.robot_position = IVec2::new(x, y);
                        world_state.terrain.insert(IVec2::new(x, y), '.');
                    }
                    _ => {
                        panic!("Unexpected terrain type: [{}]", char);
                    }
                }
            }
        }

        Ok(world_state)
    }
}
