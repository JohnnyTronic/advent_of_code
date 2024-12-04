use regex::Regex;
use std::io::Error;

#[derive(Debug, PartialEq, Eq)]
struct Token {
    start_index: usize,
    left: isize,
    right: isize,
    token_type: TokenType,
}

#[derive(Debug, PartialEq, Eq)]
enum TokenType {
    Mul,
    Do,
    Dont,
}

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let mul_regex = Regex::new(r"mul\((\d+),(\d+)\)").unwrap();
    let do_regex = Regex::new(r"do\(\)").unwrap();
    let dont_regex = Regex::new(r"don't\(\)").unwrap();
    let mut instruction_sum: isize = 0;

    let mut tokens: Vec<Token> = vec![];

    for capture in mul_regex.captures_iter(input) {
        tokens.push(Token {
            start_index: capture.get(0).unwrap().start(),
            left: capture[1].parse::<isize>().unwrap(),
            right: capture[2].parse::<isize>().unwrap(),
            token_type: TokenType::Mul,
        })
    }

    for regmatch in do_regex.find_iter(input) {
        tokens.push(Token {
            start_index: regmatch.start(),
            token_type: TokenType::Do,
            left: 0,
            right: 0,
        })
    }

    for regmatch in dont_regex.find_iter(input) {
        tokens.push(Token {
            start_index: regmatch.start(),
            token_type: TokenType::Dont,
            left: 0,
            right: 0,
        })
    }

    tokens.sort_by_key(|token| token.start_index);

    let mut mul_enabled = true;
    for token in tokens {
        match token.token_type {
            TokenType::Mul => {
                if mul_enabled {
                    instruction_sum += token.left * token.right
                }
            }
            TokenType::Do => mul_enabled = true,
            TokenType::Dont => mul_enabled = false,
        }
    }

    Ok(instruction_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

        assert_eq!("48", process(input)?);
        Ok(())
    }
}
