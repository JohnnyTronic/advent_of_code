use std::io::Error;

use day_11::part2::process;

fn main() -> Result<(), Error> {
    let file = include_str!("input.txt");
    let result = process(file)?;
    println!("Part 2 solution: {}", result);
    Ok(())
}
