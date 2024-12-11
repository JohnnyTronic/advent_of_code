use std::{collections::HashMap, io::Error};

#[derive(Debug)]
struct MetaStone {
    value: usize,
    depth: usize,
    final_count: Option<usize>,
}

impl MetaStone {
    pub fn hash(&self) -> String {
        hash_metastone_values(self.value, self.depth)
    }
}

fn hash_metastone_values(value: usize, depth: usize) -> String {
    format!("{}:{}", value, depth)
}

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("Processing input: {}", input);
    let original_stone_values: Vec<usize> = input
        .split(' ')
        .map(|i| i.trim().parse::<usize>().expect("Parse error"))
        .collect();

    let mut compute_map: HashMap<usize, (usize, Option<usize>)> = HashMap::new();
    let mut resolved_metastones: HashMap<String, MetaStone> = HashMap::new();
    let mut unresolved_metastones: Vec<MetaStone> = original_stone_values
        .iter()
        .map(|original_stone_value| MetaStone {
            value: *original_stone_value,
            depth: 0,
            final_count: None,
        })
        .collect();

    let target_depth = 75;

    while !unresolved_metastones.is_empty() {
        unresolved_metastones.sort_by(|a, b| a.depth.cmp(&b.depth));
        let mut metastone = unresolved_metastones.pop().unwrap();
        println!("Investigating unresolve stone {:?}", &metastone);

        if metastone.depth == target_depth {
            println!("HIT FINAL DEPTH: {:?}", metastone);
            metastone.final_count = Some(1);
            resolved_metastones.insert(metastone.hash(), metastone);
            continue;
        }

        let next_depth = metastone.depth + 1;
        let (next_value_a, next_value_b) = compute_next_values(&mut compute_map, metastone.value);

        let mut next_metastone_a: Option<&MetaStone> = None;
        let mut next_metastone_b: Option<&MetaStone> = None;

        if let Some(resolved_metastone_a) =
            resolved_metastones.get(&hash_metastone_values(next_value_a, next_depth))
        {
            next_metastone_a = Some(resolved_metastone_a);
            continue;
        } else {
            unresolved_metastones.push(MetaStone {
                value: next_value_a,
                depth: next_depth,
                final_count: None,
            });
        };

        if let Some(next_value_b) = next_value_b {
            if let Some(resolved_metastone_b) =
                resolved_metastones.get(&hash_metastone_values(next_value_b, next_depth))
            {
                next_metastone_b = Some(resolved_metastone_b);
            } else {
                unresolved_metastones.push(MetaStone {
                    value: next_value_b,
                    depth: next_depth,
                    final_count: None,
                });
            };
        }

        if next_metastone_a.is_some_and(|i| i.final_count.is_some())
            && next_metastone_b.is_some_and(|i| i.final_count.is_some())
        {
            let final_count_a = next_metastone_a.unwrap().final_count.unwrap();
            let mut final_count_b = 0;

            if next_metastone_b.is_some_and(|i| i.final_count.is_some()) {
                final_count_b = next_metastone_b.unwrap().final_count.unwrap();
            }

            metastone.final_count = Some(final_count_a + final_count_b);
            resolved_metastones.insert(metastone.hash(), metastone);
        } else {
            unresolved_metastones.push(metastone);
        }
    }

    // dbg!(&unresolved_metastones, &resolved_metastones);
    println!("Resolved count: {}", resolved_metastones.len());

    let mut total_stones = 0;
    for original_stone_value in original_stone_values {
        if let Some(resolved_metastone) =
            resolved_metastones.get(&hash_metastone_values(original_stone_value, 0))
        {
            total_stones += resolved_metastone.final_count.unwrap();
        } else {
            panic!("Could not find Resolved MetaStone!")
        };
    }

    Ok(total_stones.to_string())
}

fn compute_next_values(
    compute_map: &mut HashMap<usize, (usize, Option<usize>)>,
    value: usize,
) -> (usize, Option<usize>) {
    if let Some(precomputed) = compute_map.get(&value) {
        return *precomputed;
    }
    println!("Cache miss: {}", &value);
    if value == 0 {
        let answer = (1, None);
        compute_map.insert(value, answer);
        return answer;
    }

    let value_as_string = value.to_string();
    let char_count = value_as_string.chars().count();
    if char_count % 2 == 0 {
        let (first, second) = value_as_string.split_at(char_count / 2);
        let first_num = first.parse::<usize>().unwrap();
        let second_num = second.parse::<usize>().unwrap();
        let answer = (first_num, Some(second_num));
        compute_map.insert(value, answer);
        return answer;
    }

    let answer = (value * 2024, None);
    compute_map.insert(value, answer);
    answer
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "400 30 121";
        assert_eq!("55312", process(input)?);
        Ok(())
    }
}
