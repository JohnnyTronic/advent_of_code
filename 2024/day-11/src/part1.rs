use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut stones: Vec<usize> = input
        .split(' ')
        .map(|i| i.trim().parse::<usize>().expect("Parse error"))
        .collect();

    let blink_count = 25;

    for _blink in 0..blink_count {
        println!("Blink: {}", _blink);
        let mut new_stones: Vec<usize> = vec![];
        for stone in stones {
            if stone == 0 {
                new_stones.push(1);
                continue;
            }

            let stone_string = &stone.to_string();
            let char_count = stone_string.chars().count();
            if char_count % 2 == 0 {
                let (first, second) = stone_string.split_at(char_count / 2);
                new_stones.push(first.parse::<usize>().unwrap());
                new_stones.push(second.parse::<usize>().unwrap());
                continue;
            }

            new_stones.push(stone * 2024);
            continue;
        }
        stones = new_stones;
    }

    dbg!(&stones);
    Ok(stones.len().to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("55312", process(input)?);
        Ok(())
    }
}
