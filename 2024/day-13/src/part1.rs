use std::{io::Error, str::FromStr};

use glam::UVec2;
use regex::Regex;

const MAX_BUTTON_PRESSES: u32 = 100;

pub struct ClawMachine {
    pub a_delta: UVec2,
    pub b_delta: UVec2,
    pub target_position: UVec2,
}

impl ClawMachine {
    fn find_cheapest_prize(&self) -> Option<ClawMachineOutcome> {
        let mut valid_outcomes: Vec<ClawMachineOutcome> = vec![];

        for a in 0..MAX_BUTTON_PRESSES {
            for b in 0..MAX_BUTTON_PRESSES {
                let position = self.a_delta.saturating_mul(UVec2::new(a, a))
                    + self.b_delta.saturating_mul(UVec2::new(b, b));
                if position == self.target_position {
                    valid_outcomes.push(ClawMachineOutcome {
                        a_presses: a as usize,
                        b_presses: b as usize,
                        credit_cost: a as usize * 3 + b as usize,
                    });
                }
            }
        }

        valid_outcomes.sort_by(|a, b| a.credit_cost.cmp(&b.credit_cost));

        valid_outcomes.first().map(|outcome| outcome.to_owned())
    }
}

#[derive(Clone)]
struct ClawMachineOutcome {
    a_presses: usize,
    b_presses: usize,
    credit_cost: usize,
}

#[derive(Debug)]
pub struct ParseClawMachineError {}
impl FromStr for ClawMachine {
    type Err = ParseClawMachineError;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        let regex_a = Regex::new(r"Button A: X\+(\d+), Y\+(\d+)").expect("Regex A error");
        let regex_b = Regex::new(r"Button B: X\+(\d+), Y\+(\d+)").expect("Regex B error");
        let regex_prize = Regex::new(r"Prize: X=(\d+), Y=(\d+)").expect("Regex Prize error");

        let a_captures = regex_a.captures(input).expect("Regex A capture error");
        let b_captures = regex_b.captures(input).expect("Regex B capture error");
        let prize_captures = regex_prize
            .captures(input)
            .expect("Regex Prize capture error");

        let ax_delta = &a_captures[1].parse::<u32>().unwrap();
        let ay_delta = &a_captures[2].parse::<u32>().unwrap();
        let bx_delta = &b_captures[1].parse::<u32>().unwrap();
        let by_delta = &b_captures[2].parse::<u32>().unwrap();
        let prize_x = &prize_captures[1].parse::<u32>().unwrap();
        let prize_y = &prize_captures[2].parse::<u32>().unwrap();

        Ok(ClawMachine {
            a_delta: UVec2::new(*ax_delta, *ay_delta),
            b_delta: UVec2::new(*bx_delta, *by_delta),
            target_position: UVec2::new(*prize_x, *prize_y),
        })
    }
}

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut claw_machines: Vec<ClawMachine> = vec![];

    let chunks: Vec<&str> = input.split("\n\n").collect();
    for chunk in chunks {
        let claw_machine = ClawMachine::from_str(chunk).unwrap();
        claw_machines.push(claw_machine);
    }

    let mut total_credit_cost = 0;
    for claw_machine in claw_machines {
        match claw_machine.find_cheapest_prize() {
            None => {}
            Some(outcome) => {
                total_credit_cost += outcome.credit_cost;
            }
        }
    }

    Ok(total_credit_cost.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("480", process(input)?);
        Ok(())
    }
}
