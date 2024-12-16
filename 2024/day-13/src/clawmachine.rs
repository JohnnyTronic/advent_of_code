use std::str::FromStr;

use glam::U64Vec2;
use regex::Regex;

#[derive(Debug)]
pub struct ClawMachine {
    pub a_delta: U64Vec2,
    pub b_delta: U64Vec2,
    pub target_position: U64Vec2,
}

const MAX_BUTTON_PRESSES: u64 = 100;
impl ClawMachine {
    pub fn find_cheapest_prize(&self) -> Option<ClawMachineOutcome> {
        let mut valid_outcomes: Vec<ClawMachineOutcome> = vec![];

        for a in 0..MAX_BUTTON_PRESSES {
            for b in 0..MAX_BUTTON_PRESSES {
                let position = self.a_delta.saturating_mul(U64Vec2::new(a, a))
                    + self.b_delta.saturating_mul(U64Vec2::new(b, b));
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
pub struct ClawMachineOutcome {
    pub a_presses: usize,
    pub b_presses: usize,
    pub credit_cost: usize,
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

        let ax_delta = &a_captures[1].parse::<u64>().unwrap();
        let ay_delta = &a_captures[2].parse::<u64>().unwrap();
        let bx_delta = &b_captures[1].parse::<u64>().unwrap();
        let by_delta = &b_captures[2].parse::<u64>().unwrap();
        let prize_x = &prize_captures[1].parse::<u64>().unwrap();
        let prize_y = &prize_captures[2].parse::<u64>().unwrap();

        Ok(ClawMachine {
            a_delta: U64Vec2::new(*ax_delta, *ay_delta),
            b_delta: U64Vec2::new(*bx_delta, *by_delta),
            target_position: U64Vec2::new(*prize_x, *prize_y),
        })
    }
}
