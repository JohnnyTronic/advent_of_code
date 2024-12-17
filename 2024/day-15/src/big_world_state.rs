use std::collections::{HashMap, HashSet};

use glam::IVec2;

use crate::{world_state::WorldState, ShoveResult};

pub struct BigWorldState {
    pub width: usize,
    pub height: usize,
    pub terrain: HashMap<IVec2, char>,
    pub box_positions: HashSet<IVec2>,
    pub robot_position: IVec2,
}

pub enum CollisionResult {
    Wall(IVec2),
}

impl BigWorldState {
    pub fn from_world_state(world_state: &WorldState) -> BigWorldState {
        let mut big_world_state = BigWorldState {
            width: world_state.width * 2,
            height: world_state.height,
            terrain: HashMap::new(),
            box_positions: HashSet::new(),
            robot_position: IVec2::new(0, 0),
        };

        for (position, char) in &world_state.terrain {
            let big_position = IVec2::new(position.x * 2, position.y);
            let big_position_plus = big_position + IVec2::new(1, 0);
            match char {
                '.' => {
                    big_world_state.terrain.insert(big_position, '.');
                    big_world_state.terrain.insert(big_position_plus, '.');
                }
                '#' => {
                    big_world_state.terrain.insert(big_position, '#');
                    big_world_state.terrain.insert(big_position_plus, '#');
                }
                _ => panic!("Unexpected terrain [{}]", char),
            }
        }

        for box_position in &world_state.box_positions {
            let big_box_position = IVec2::new(box_position.x * 2, box_position.y);
            big_world_state.box_positions.insert(big_box_position);
        }

        big_world_state.robot_position =
            world_state.robot_position.saturating_mul(IVec2::new(2, 2));

        big_world_state
    }

    fn check_collision_narrow(position: &IVec2) -> CollisionResult {}

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
                        ShoveResult::Success
                    }
                    '#' => {
                        // Wall, cannot shove
                        ShoveResult::Blocked
                    }
                    _ => panic!("Unexpected terrain [{}]", char),
                }
            }
        }
    }

    pub fn print_map(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let print_position = IVec2::new(x as i32, y as i32);
                if print_position == self.robot_position {
                    print!("@");
                } else if self
                    .box_positions
                    .iter()
                    .any(|box_position| box_position == &print_position)
                {
                    print!("[");
                } else if self.box_positions.iter().any(|box_position| {
                    let box_position_plus = IVec2::new(box_position.x + 1, box_position.y);
                    print_position == box_position_plus
                }) {
                    print!("]");
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
