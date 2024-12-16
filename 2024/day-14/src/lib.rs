use glam::IVec2;
use robot::Robot;

pub mod part1;
pub mod part2;
pub mod robot;

pub fn print_map(robots: &[Robot], width: i32, height: i32) {
    for y in 0..height {
        for x in 0..width {
            let robot_count: usize = robots
                .iter()
                .filter(|r| r.position == IVec2::new(x, y))
                .count();

            match robot_count {
                0 => print!("."),
                _ => print!("{}", robot_count),
            }
        }
        println!();
    }
    println!();
}

pub fn calculate_safety_factor(robots: &[Robot], terrain_width: i32, terrain_height: i32) -> i32 {
    let terrain_half_width = terrain_width / 2;
    let terrain_half_height = terrain_height / 2;
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

    let safety_factor: i32 = quandrant_counts.iter().product();
    safety_factor
}
