use std::fs::read_to_string;

fn main() {
    println!("Advent of Code - Day 01!");

    read_lines("testInput.txt");
}

fn read_lines(filename: &str) -> Vec<String> {
    let mut result = Vec::new();

    for line in read_to_string(filename).unwrap().lines() {
        result.push(line.to_string());
        println!("{}", line.to_string());
    }

    result
}
