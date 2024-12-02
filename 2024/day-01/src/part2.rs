use std::io::Result;

pub fn process(input: &str) -> Result<String> {
    println!("[main start]");

    let mut left = vec![];
    let mut right = vec![];

    let lines = input.lines();

    for line in lines {
        let mut split = line.split_ascii_whitespace();
        left.push(split.next().unwrap().parse::<isize>().unwrap());
        right.push(split.next().unwrap().parse::<isize>().unwrap());
    }

    let similarity_sum = left.iter().fold(0isize, |acc, l| {
        let match_count: isize = right.iter().filter(|&r| r == l).count().try_into().unwrap();
        let similarity_value = l * match_count;
        acc + similarity_value
    });

    println!("[main end]");
    Ok(similarity_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<()> {
        let input = include_str!("input-example.txt");
        assert_eq!("31", process(input)?);
        Ok(())
    }
}
