fn main() {
    println!("Advent of Code - Day 01!");

    let file = include_str!("../input.txt");
    let result1 = do_part_1(file);
    println!("Part 1 Answer - {}", result1);

    let result2 = do_part_2(file);
    println!("Part 2 Answer - {}", result2);
}

fn do_part_1(input: &str) -> String {
    let output = input
        .lines()
        .map(|line| {
            let mut it = line.chars().filter_map(|character| character.to_digit(10));
            let first = it.next().expect("Should be a number.");
            match it.last() {
                Some(num) => format!("{first}{num}"),
                None => format!("{first}{first}"),
            }
            .parse::<u32>()
            .expect("should be a valid number")
        })
        .sum::<u32>();

    output.to_string()
}

fn do_part_2(input: &str) -> String {
    input
        .lines()
        .map(|line| {
            let mut numbers = (0..line.len()).filter_map(|index| {
                let substring = &line[index..];
                let digit = if substring.starts_with("one") {
                    '1'
                } else if substring.starts_with("two") {
                    '2'
                } else if substring.starts_with("three") {
                    '3'
                } else if substring.starts_with("four") {
                    '4'
                } else if substring.starts_with("five") {
                    '5'
                } else if substring.starts_with("six") {
                    '6'
                } else if substring.starts_with("seven") {
                    '7'
                } else if substring.starts_with("eight") {
                    '8'
                } else if substring.starts_with("nine") {
                    '9'
                } else {
                    substring.chars().next().unwrap()
                };
                digit.to_digit(10)
            });
            let first = numbers.next().expect("should be a number");

            match numbers.last() {
                Some(num) => format!("{first}{num}"),
                None => format!("{first}{first}"),
            }
            .parse::<u32>()
            .expect("should be a valid number")
        })
        .sum::<u32>()
        .to_string()
}

#[cfg(test)]
mod tests {
    use super::*;

    const TEST_INPUT: &str = include_str!("../testInput.txt");

    #[test]
    fn test_part_1() {
        assert_eq!(do_part_1(TEST_INPUT), "142");
    }

    #[test]
    fn test_part_2() {
        assert_eq!(do_part_2(TEST_INPUT), "281");
    }
}
