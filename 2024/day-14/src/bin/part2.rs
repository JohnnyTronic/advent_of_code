use std::io::Error;

use day_14::part2::process;

fn main() -> Result<(), Error> {
    let file = include_str!("input.txt");
    // let file = include_str!("input2.txt");
    let result = process(file, 101, 103)?;
    println!("Part 2 solution: {}", result);
    Ok(())
}
