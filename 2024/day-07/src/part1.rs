use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut total_calibration_result = 0;

    for line in input.lines() {
        let mut line_split = line.split(':');
        let test_value = line_split.next().unwrap().parse::<u64>().unwrap();
        // println!("Line sum: {}", target_sum);

        let args_list = line_split.next().expect("Error extracting args list");
        let args: Vec<u64> = args_list
            .trim()
            .split(" ")
            .map(|arg| arg.parse::<u64>().expect("failed to parse arg into number"))
            .collect();

        let operator_count = args.len() - 1;
        let total_permutations = 2_u64.pow(operator_count as u32);

        for perm in 0..total_permutations {
            let mut arg_iter = args.iter().enumerate();
            let (_arg_index, starting_value) = arg_iter.next().expect("Empty args?!");
            let mut running_total: u64 = *starting_value;

            for (next_arg_index, next_arg) in arg_iter {
                let slot_mask = 1 << (next_arg_index - 1);
                let extracted_value = perm & slot_mask;
                if extracted_value > 0 {
                    running_total += next_arg;
                } else {
                    running_total *= next_arg;
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("3749", process(input)?);
        Ok(())
    }
}
