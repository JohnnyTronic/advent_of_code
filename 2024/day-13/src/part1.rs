use std::{io::Error, str::FromStr};

use crate::clawmachine::ClawMachine;

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
