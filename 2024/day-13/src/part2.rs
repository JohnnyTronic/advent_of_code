use std::{io::Error, str::FromStr};

use crate::clawmachine::ClawMachine;

type NumType = f64;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut claw_machines: Vec<ClawMachine> = vec![];

    let chunks: Vec<&str> = input.split("\n\n").collect();
    for chunk in chunks {
        let mut claw_machine = ClawMachine::from_str(chunk).unwrap();
        claw_machine.target_position.x += 10000000000000;
        claw_machine.target_position.y += 10000000000000;
        claw_machines.push(claw_machine);
    }

    println!("Claw machine count: {}", claw_machines.len());

    let mut total_credit_cost = 0;
    for claw_machine in claw_machines.iter() {
        println!("For claw machine [{:?}]...", claw_machine);
        let px = claw_machine.target_position.x as NumType;
        let py = claw_machine.target_position.y as NumType;
        let adx = claw_machine.a_delta.x as NumType;
        let ady = claw_machine.a_delta.y as NumType;
        let bdx = claw_machine.b_delta.x as NumType;
        let bdy = claw_machine.b_delta.y as NumType;

        let b = (py - (ady * (px / adx))) / (bdy - (ady * (bdx / adx)));
        let a = (px / adx) - ((bdx / adx) * b);

        println!("... a [{:?}] b[{:?}]", a, b);
        let b_epsilon = b.fract().round() - b.fract();
        let a_epsilon = a.fract().round() - a.fract();
        let b_closeenough = b_epsilon.abs() < 0.001;
        let a_closeenough = a_epsilon.abs() < 0.001;
        // dbg!(
        //     b_epsilon.abs(),
        //     a_epsilon.abs(),
        //     b_closeenough,
        //     a_closeenough
        // );

        if b_closeenough && a_closeenough {
            let credit_cost = a.round() as isize * 3 + b.round() as isize;
            dbg!(credit_cost);
            total_credit_cost += credit_cost;
        }
    }

    Ok(total_credit_cost.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example2.txt");
        assert_eq!("1206", process(input)?);
        Ok(())
    }
}
