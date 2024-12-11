use day_05::part2::process;
use std::io::Error;

fn main() -> Result<(), Error> {
    let file = include_str!("input.txt");
    let result = process(file)?;
    println!("Result: {}", result);
    Ok(())
}
