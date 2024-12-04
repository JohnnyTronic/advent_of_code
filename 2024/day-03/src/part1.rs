use regex::Regex;
use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let regex = Regex::new(r"mul\((\d+),(\d+)\)").unwrap();
    let mut instruction_sum: isize = 0;

    for capture in regex.captures_iter(input) {
        let left = &capture[1].parse::<isize>().unwrap();
        let right = &capture[2].parse::<isize>().unwrap();
        instruction_sum += left * right;
    }

    Ok(instruction_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("161", process(input)?);
        Ok(())
    }
}
