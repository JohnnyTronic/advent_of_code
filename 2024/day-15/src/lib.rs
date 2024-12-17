use glam::IVec2;

pub mod big_world_state;
pub mod part1;
pub mod part2;
pub mod world_state;

pub fn parse_instructions(input: &str) -> Vec<IVec2> {
    let instruction_input = input.split("\n\n").nth(1).unwrap();

    let mut instructions: Vec<IVec2> = vec![];
    for line in instruction_input.lines() {
        for char in line.chars() {
            match char {
                '<' => instructions.push(IVec2::NEG_X),
                '^' => instructions.push(IVec2::NEG_Y),
                '>' => instructions.push(IVec2::X),
                'v' => instructions.push(IVec2::Y),
                _ => panic!("Unexpected instruction char: [{}]", char),
            }
        }
    }

    instructions
}

pub enum ShoveResult {
    Success,
    Blocked,
}
