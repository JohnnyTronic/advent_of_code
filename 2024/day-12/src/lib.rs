use glam::IVec2;

pub mod part1;
pub mod part2;
pub mod region;
pub mod terrain;

const NORTH: IVec2 = glam::IVec2::NEG_Y;
const EAST: IVec2 = glam::IVec2::X;
const SOUTH: IVec2 = glam::IVec2::Y;
const WEST: IVec2 = glam::IVec2::NEG_X;
const CARDINAL_OFFSETS: [IVec2; 4] = [NORTH, EAST, SOUTH, WEST];
