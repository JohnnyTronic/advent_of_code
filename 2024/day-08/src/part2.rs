use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut total_calibration_result = 0;

    for line in input.lines() {
        let mut line_split = line.split(':');
        let test_value = line_split.next().unwrap().parse::<u64>().unwrap();

        let args_list = line_split.next().expect("Error extracting args list");
        let args: Vec<u64> = args_list
            .trim()
            .split(" ")
            .map(|arg| arg.parse::<u64>().expect("failed to parse arg into number"))
            .collect();

        let operator_count = args.len() - 1;
        let operators_sequence_permutations = generate_operator_sequences(operator_count as u64);

        for operator_sequence in operators_sequence_permutations {
            let mut arg_iter = args.iter().enumerate();
            let (_arg_index, first_arg) = arg_iter.next().expect("Empty args?!");
            let mut running_total: u64 = *first_arg;

            for (operator_index, operator) in operator_sequence.iter().enumerate() {
                let (_next_arg_index, next_arg) =
                    arg_iter.next().expect("Arg vs Operator count mismatch");

                match operator {
                    Operator::Add => {
                        running_total += next_arg;
                    }
                    Operator::Multiply => {
                        running_total *= next_arg;
                    }
                    Operator::Concatenate => {
                        running_total = (running_total.to_string() + &next_arg.to_string())
                            .parse::<u64>()
                            .expect("Could re-numberatize the concatenated arg strings");
                    }
                }
            }

            if running_total == test_value {
                total_calibration_result += test_value;
                break;
            }
        }
    }

    Ok(total_calibration_result.to_string())
}

#[derive(Debug, Clone)]
enum Operator {
    Add,
    Multiply,
    Concatenate,
}

fn generate_operator_sequences(operator_count: u64) -> Vec<Vec<Operator>> {
    let mut operator_chain_permutations: Vec<Vec<Operator>> = vec![
        vec![Operator::Add],
        vec![Operator::Multiply],
        vec![Operator::Concatenate],
    ];

    if operator_count > 1 {
        operator_chain_permutations =
            append_operators(operator_chain_permutations, operator_count - 1);
    }
    operator_chain_permutations
}

fn append_operators(
    operator_chain_permutations: Vec<Vec<Operator>>,
    depth: u64,
) -> Vec<Vec<Operator>> {
    let mut to_return: Vec<Vec<Operator>> = vec![];

    for permutation in operator_chain_permutations {
        let mut add_variant = permutation.clone();
        add_variant.push(Operator::Add);
        to_return.push(add_variant);
        let mut mul_variant = permutation.clone();
        mul_variant.push(Operator::Multiply);
        to_return.push(mul_variant);
        let mut concat_variant = permutation.clone();
        concat_variant.push(Operator::Concatenate);
        to_return.push(concat_variant);
    }

    if depth > 1 {
        to_return = append_operators(to_return, depth - 1)
    };
    to_return
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("11387", process(input)?);
        Ok(())
    }
}
