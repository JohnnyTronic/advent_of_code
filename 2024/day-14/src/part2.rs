use std::io::Error;

use glam::IVec2;
use regex::Regex;

use crate::{calculate_safety_factor, print_map, robot::Robot};

pub fn process(
    input: &str,
    terrain_width: i32,
    terrain_height: i32,
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

    let mut min_safety_factor = i32::MAX;
    let mut buffer = String::new();

    for i in 1..=100000 {
        for robot in &mut robots {
            robot.position += robot.velocity;
            robot.position.x = (robot.position.x + terrain_width) % terrain_width;
            robot.position.y = (robot.position.y + terrain_height) % terrain_height;
        }

        let factor = calculate_safety_factor(&robots, terrain_width, terrain_height);
        if factor < min_safety_factor {
            min_safety_factor = factor;
            println!("Iteration [{}]", i);
            print_map(&robots, terrain_width, terrain_height);

            let _ = std::io::stdin().read_line(&mut buffer);
        }

        // let sleep_duration = std::time::Duration::from_millis(100);
        // std::thread::sleep(sleep_duration);
    }

    Ok(input.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("1206", process(input, 101, 103)?);
        Ok(())
    }
}
