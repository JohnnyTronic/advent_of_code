use std::sync::{LazyLock, Mutex};
use std::{
    cell::RefCell,
    collections::HashMap,
    io::Error,
    rc::{Rc, Weak},
};

static COMPUTE_MAP: LazyLock<Mutex<HashMap<String, usize>>> =
    LazyLock::new(|| Mutex::new(HashMap::new()));

#[derive(Debug)]
struct Stone {
    value: usize,
    depth: usize,
    cumulative_child_count: Option<usize>,
    parent_stone: Option<Weak<RefCell<Stone>>>,
    child_stone_a: Option<Rc<RefCell<Stone>>>,
    child_stone_b: Option<Rc<RefCell<Stone>>>,
}

impl Stone {
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

    // let mut compute_map: HashMap<usize, (usize, Option<usize>)> = HashMap::new();
    // let mut resolved_stones: HashMap<String, Stone> = HashMap::new();
    let unresolved_stones: Vec<Rc<RefCell<Stone>>> = original_stone_values
        .iter()
        .map(|original_stone_value| {
            Rc::new(RefCell::new(Stone {
                value: *original_stone_value,
                depth: 0,
                cumulative_child_count: None,
                parent_stone: None,
                child_stone_a: None,
                child_stone_b: None,
            }))
        })
        .collect();

    // let target_depth = 75;
    let mut total_stones = 0;

    for stone in unresolved_stones {
        let total_child_count = compute_cumulative_child_count(stone);
        total_stones += total_child_count;
    }

    Ok(total_stones.to_string())
}

fn compute_cumulative_child_count(stone: Rc<RefCell<Stone>>) -> usize {
    let mut child_totals = 0;

    if stone.borrow().depth == 75 {
        println!("Bottomed out");
        return 1;
    }

    let hash = &stone.borrow().hash();
    if let Some(precomputed) = COMPUTE_MAP.lock().unwrap().get(hash) {
        println!(
            "Returning precomputed score [{}] for hash [{}]",
            precomputed, hash
        );
        return *precomputed;
    }

    let (a_value, b_value) = compute_next_values(&stone.borrow().value);

    let child_stone_a = Rc::new(RefCell::new(Stone {
        value: a_value,
        depth: stone.borrow().depth + 1,
        cumulative_child_count: None,
        parent_stone: Some(Rc::downgrade(&stone)),
        child_stone_a: None,
        child_stone_b: None,
    }));
    stone.borrow_mut().child_stone_a = Some(Rc::clone(&child_stone_a));

    println!("Recurse A - Depth {}", child_stone_a.borrow().depth);
    let stone_a_totals = compute_cumulative_child_count(child_stone_a);
    child_totals += stone_a_totals;

    if let Some(b_value) = b_value {
        let child_stone_b = Rc::new(RefCell::new(Stone {
            value: b_value,
            depth: stone.borrow().depth + 1,
            cumulative_child_count: None,
            parent_stone: Some(Rc::downgrade(&stone)),
            child_stone_a: None,
            child_stone_b: None,
        }));
        stone.borrow_mut().child_stone_b = Some(Rc::clone(&child_stone_b));

        println!("Recurse B - Depth {}", child_stone_b.borrow().depth);
        let stone_b_totals = compute_cumulative_child_count(child_stone_b);
        child_totals += stone_b_totals;
    }

    let hash = stone.borrow().hash();
    println!(
        "Recording precomputed score [{}] for hash [{}]",
        child_totals, hash
    );
    COMPUTE_MAP.lock().unwrap().insert(hash, child_totals);

    child_totals
}

fn compute_next_values(
    // compute_map: &mut HashMap<usize, (usize, Option<usize>)>,
    value: &usize,
) -> (usize, Option<usize>) {
    // if let Some(precomputed) = compute_map.get(value) {
    //     return *precomputed;
    // }
    // println!("Cache miss: {}", &value);
    if *value == 0 {
        let answer = (1, None);
        // compute_map.insert(*value, answer);
        return answer;
    }

    let value_as_string = value.to_string();
    let char_count = value_as_string.chars().count();
    if char_count % 2 == 0 {
        let (first, second) = value_as_string.split_at(char_count / 2);
        let first_num = first.parse::<usize>().unwrap();
        let second_num = second.parse::<usize>().unwrap();
        let answer = (first_num, Some(second_num));
        // compute_map.insert(*value, answer);
        return answer;
    }

    (value * 2024, None)
    // compute_map.insert(*value, answer);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "125 17";
        assert_eq!("55312", process(input)?);
        Ok(())
    }
}
