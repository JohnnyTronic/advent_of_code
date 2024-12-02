use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");

    let mut left = vec![];
    let mut right = vec![];

    let lines = input.lines();
    // consumes the iterator, returns an (Optional) String
    for line in lines {
        let mut split = line.split_ascii_whitespace();
        let left_value = split.next().expect("Unexpected line split error");
        let right_value = split.next().expect("Unexpected line spit error");
        left.push(left_value.to_owned().parse::<isize>().unwrap());
        right.push(right_value.to_owned().parse::<isize>().unwrap());
    }

    left.sort();
    right.sort();

    let zipped: Vec<(&isize, &isize)> = left.iter().zip(&right).collect();

    let deltas: Vec<isize> = zipped
        .iter()
        .map(|(&left, &right)| (left - right).abs())
        .collect();

    let sum: isize = deltas.iter().sum();
    println!("Final sum: {}", sum);

    println!("[main end]");

    Ok(sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("11", process(input)?);
        Ok(())
    }
}
