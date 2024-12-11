use std::{collections::HashMap, io::Error};

struct MetaStone {
    value: usize,
    depth: usize,
    post_blink: (usize, Option<usize>),
    final_count: usize,
}

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("Processing input: {}", input);
    let mut stones: Vec<usize> = input
        .split(' ')
        .map(|i| i.trim().parse::<usize>().expect("Parse error"))
        .collect();

    let blink_count = 25;

    let mut compute_map: HashMap<usize, (usize, Option<usize>)> = HashMap::new();
    compute_map.insert(0, (1, None));

    for _blink in 0..blink_count {
        println!("BLINK: {}", _blink);
        let mut new_stones: Vec<usize> = vec![];
        for stone in stones {
            if let Some(precomputed) = compute_map.get(&stone) {
                new_stones.push(precomputed.0);
                if let Some(second) = precomputed.1 {
                    new_stones.push(second);
                }
                continue;
            }

            println!("Uncached: {}", stone);

            let stone_string = &stone.to_string();
            let char_count = stone_string.chars().count();
            if char_count % 2 == 0 {
                let (first, second) = stone_string.split_at(char_count / 2);
                let first_num = first.parse::<usize>().unwrap();
                let second_num = second.parse::<usize>().unwrap();
                compute_map.insert(stone, (first_num, Some(second_num)));
                new_stones.push(first_num);
                new_stones.push(second_num);

                continue;
            }

            let answer = stone * 2024;
            compute_map.insert(stone, (answer, None));
            new_stones.push(answer);
            continue;
        }
        stones = new_stones;
    }

    println!("Beginning recursion...");
    let mut child_total = 0;
    for stone in &stones {
        child_total += calculate_total_leaves(stone, 75, &compute_map);
    }

    Ok(child_total.to_string())
}

fn calculate_total_leaves(
    stone: &usize,
    depth: usize,
    compute_map: &HashMap<usize, (usize, Option<usize>)>,
) -> usize {
    let next_gen = compute_map
        .get(stone)
        .unwrap_or_else(|| panic!("Compute map missing value: {}", stone));
    if depth == 1 {
        return if next_gen.1.is_some() { 2 } else { 1 };
    }

    let mut child_total = 0;
    child_total += calculate_total_leaves(&next_gen.0, depth - 1, compute_map);
    if let Some(next_gen_b) = next_gen.1 {
        child_total += calculate_total_leaves(&next_gen_b, depth + 1, compute_map);
    }
    child_total
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "30";
        assert_eq!("55312", process(input)?);
        Ok(())
    }
}
