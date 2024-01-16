use regex::Regex;
use std::cmp::max;

struct Sample {
    red: i16,
    green: i16,
    blue: i16,
}

// impl Sample {
//     fn new() -> Sample{
//         Sample{
//             red: 0,
//             green: 0,
//             blue: 0
//         }
//     }
// }

struct Game {
    id: i16,
    samples: Vec<Sample>,
}

impl Game {
    fn is_possible_given_limits(&self, red_limit: i16, green_limit: i16, blue_limit: i16) -> bool {
        for sample in &self.samples {
            if sample.red > red_limit {
                return false;
            }

            if sample.green > green_limit {
                return false;
            }

            if sample.blue > blue_limit {
                return false;
            }
        }
        true
    }

    fn calculate_minimum_power(&self) -> i16 {
        let mut min_red = 0;
        let mut min_green = 0;
        let mut min_blue = 0;

        for sample in &self.samples {
            min_red = max(min_red, sample.red);
            min_green = max(min_green, sample.green);
            min_blue = max(min_blue, sample.blue);
        }

        return min_red * min_green * min_blue;
    }
}

fn do_part_1(input: &str) -> String {
    // Cosntraints
    let max_red = 12;
    let max_green = 13;
    let max_blue = 14;

    let mut games: Vec<Game> = Vec::new();
    let single_color_pattern = Regex::new(r"(?<count>\d+) (?<color_name>\w+)").unwrap();
    for line in input.lines() {
        let (game_label, game_observations) = line.split_once(": ").unwrap();
        let (_, game_number_str) = game_label.split_once(" ").unwrap();
        let game_number = game_number_str.parse::<i16>().unwrap();

        let mut game = Game {
            id: game_number,
            samples: Vec::new(),
        };
        let observations = game_observations.split("; ");
        for observation in observations {
            let mut sample = Sample {
                red: 0,
                green: 0,
                blue: 0,
            };
            for single_color_observation in single_color_pattern.captures_iter(observation) {
                let count = single_color_observation["count"].parse::<i16>().unwrap();
                let color_name = &single_color_observation["color_name"];
                match color_name {
                    "red" => sample.red = count,
                    "green" => sample.green = count,
                    "blue" => sample.blue = count,
                    _ => panic!("Unrecognized color_name {color_name}"),
                }
            }
            game.samples.push(sample);
        }
        games.push(game);
    }

    let possible_game_count = games
        .iter()
        .filter(|game| game.is_possible_given_limits(max_red, max_green, max_blue))
        .count();

    return possible_game_count.to_string();
}

fn do_part_2(input: &str) -> String {
    input.to_string()
}

fn main() {
    println!("Advent of Code 2023 - Day 02!");

    let input = include_str!("../input.txt");
    let result1 = do_part_1(input);
    println!("Answer Part 1 - {result1}");

    //let result2 = do_part_2(input);
    //println!("Answer Part 2 - {result2}");
}

#[cfg(test)]
mod tests {
    use super::*;

    const TEST_INPUT: &str = include_str!("../test_input.txt");

    #[test]
    fn test_part_1() {
        println!("Test Part 1");
        let result1 = do_part_1(TEST_INPUT);
        assert_eq!(result1, "8");
    }
}
