use std::io::Error;

use day_14::part1::process;

fn main() -> Result<(), Error> {
    let file = include_str!("input.txt");
    let result = process(file, 101, 103, 100)?;
    println!("Final answer: [{}]", result);
    Ok(())
}
