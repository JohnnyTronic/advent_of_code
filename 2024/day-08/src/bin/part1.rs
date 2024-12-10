use std::io::Error;

use day_08::part1::process;

fn main() -> Result<(), Error> {
    let file = include_str!("input.txt");
    let result = process(file)?;
    println!("{}", result);
    Ok(())
}
