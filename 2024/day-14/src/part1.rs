use std::io::Error;

use glam::IVec2;
use regex::Regex;

use crate::{print_map, robot::Robot};

pub fn process(
    input: &str,
    terrain_width: i32,
    terrain_height: i32,
    iteration_count: isize,
) -> std::result::Result<String, Error> {
    let regex = Regex::new(r"p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+)").expect("Regex definition error");

    let mut robots: Vec<Robot> = vec![];
    for line in input.lines() {
        let captures = regex.captures(line).expect("No captures?");
        let px = captures[1].parse::<i32>().unwrap();
        let py = captures[2].parse::<i32>().unwrap();
        let vx = captures[3].parse::<i32>().unwrap();
        let vy = captures[4].parse::<i32>().unwrap();

        robots.push(Robot {
            position: IVec2::new(px, py),
            velocity: IVec2::new(vx, vy),
        });
    }

    for i in 1..=iteration_count {
        for robot in &mut robots {
            robot.position += robot.velocity;
            robot.position.x = (robot.position.x + terrain_width) % terrain_width;
            robot.position.y = (robot.position.y + terrain_height) % terrain_height;
        }
        println!("Iteration [{}]", i);
        print_map(&robots, terrain_width, terrain_height);
    }

    let terrain_half_width = terrain_width / 2;
    let terrain_half_height = terrain_height / 2;
    dbg!(terrain_half_width, terrain_half_height);

    let mut quandrant_counts = [0; 4];
    for robot in robots {
        if robot.position.x < terrain_half_width && robot.position.y < terrain_half_height {
            quandrant_counts[0] += 1;
        }

        if robot.position.x > terrain_half_width && robot.position.y < terrain_half_height {
            quandrant_counts[1] += 1;
        }

        if robot.position.x < terrain_half_width && robot.position.y > terrain_half_height {
            quandrant_counts[2] += 1;
        }
        if robot.position.x > terrain_half_width && robot.position.y > terrain_half_height {
            quandrant_counts[3] += 1;
        }
    }
    dbg!(quandrant_counts);

    let safety_factor: i32 = quandrant_counts.iter().product();

    Ok(safety_factor.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("12", process(input, 11, 7, 100)?);
        Ok(())
    }
}
