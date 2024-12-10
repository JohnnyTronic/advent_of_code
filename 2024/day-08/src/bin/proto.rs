use std::{collections::HashSet, io::Error};

use glam::IVec2;
use itertools::Itertools;

fn main() -> Result<(), Error> {
    println!("Proto!");

    // let list: Vec<(i32, i32)> = vec![(0, 0), (1, 1), (2, 2)];
    //
    // let combinations: Vec<Vec<&(i32, i32)>> = list.iter().combinations(2).collect();
    // dbg!(combinations);

    // let it: Vec<Vec<i32>> = (0..5).combinations(2).collect();
    // dbg!(it);

    let vec_a = IVec2::new(0, 1);
    let vec_b = IVec2::new(2, 3);
    let vec_c = IVec2::new(0, 1);
    let mut hashset = HashSet::new();
    hashset.insert(vec_a);
    hashset.insert(vec_b);
    hashset.insert(vec_c);

    dbg!(hashset);

    Ok(())
}
