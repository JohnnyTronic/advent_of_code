use crate::point2d::{EAST, NORTH, SOUTH, WEST};
use crate::{board::Board, point2d::Point2D};

pub struct Guard {
    pub position: Point2D,
    pub facing: Point2D,
}

impl Guard {
    pub fn complete_patrol(&mut self, board: &mut Board) {
        if self.position.x < 0 {
            return;
        }

        if self.position.y < 0 {
            return;
        }
        while let Some(current_cell) = board.get(&self.position) {
            current_cell.visited = true;
            println!("Vistied: ({},{})", self.position.x, self.position.y);

            let next_position = &self.position + &self.facing;
            if let Some(next_cell) = board.get(&next_position) {
                println!("Next cell: {}", next_cell.terrain);
                match next_cell.terrain {
                    '.' => self.position = next_position.clone(),
                    '#' => self.turn_right(),
                    _ => panic!("Unexpected terrain: {}", next_cell.terrain),
                }
            } else {
                return;
            }
        }
    }

    pub fn turn_right(&mut self) {
        println!("@ {:?}, turning right", self.position);
        match self.facing {
            NORTH => self.facing = EAST,
            EAST => self.facing = SOUTH,
            SOUTH => self.facing = WEST,
            WEST => self.facing = NORTH,
            _ => panic!("Unexpected facing: {:?}", self.facing),
        }
    }
}
