use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let mut safe_report_count = 0;

    let lines = input.lines();
    // consumes the iterator, returns an (Optional) String
    for line in lines {
        dbg!(line);
        let levels: Vec<isize> = line
            .split_ascii_whitespace()
            .map(|val| val.parse::<isize>().unwrap())
            .collect();

        let default_check = check_report(&levels);
        if default_check == ReportOutcome::Safe {
            safe_report_count += 1;
            dbg!(default_check, safe_report_count);
            continue;
        }
        let outcome = check_report_variants(levels);
        safe_report_count += match outcome {
            ReportOutcome::Safe => 1,
            _ => 0,
        }
    }

    Ok(safe_report_count.to_string())
}

#[derive(Debug, PartialEq)]
enum ReportOutcome {
    Safe,
    Unsafe,
}

fn check_report<'a, I>(levels: I) -> ReportOutcome
where
    I: IntoIterator<Item = &'a isize>,
{
    let mut levels = levels.into_iter();
    let mut trend: Option<isize> = None;
    let mut target_value = levels.next().unwrap();
    for next_value in levels {
        let delta = next_value - target_value;
        if delta.abs() < 1 || delta.abs() > 3 {
            println!(
                "Unsafe delta: {}, next_value: {}, target_value: {}",
                delta, next_value, target_value
            );
            return ReportOutcome::Unsafe;
        }
        target_value = next_value;

        match trend {
            None => {
                trend = Some(delta.signum());
                continue;
            }
            Some(trend_value) => {
                let next_trend = delta.signum();
                if next_trend != trend_value {
                    println!("Unsafe trend");
                    return ReportOutcome::Unsafe;
                }
                trend = Some(next_trend);
            }
        }
    }

    ReportOutcome::Safe
}

fn check_report_variants<I>(levels: I) -> ReportOutcome
where
    I: IntoIterator<Item = isize>,
{
    let levels: Vec<isize> = levels.into_iter().collect();
    for index in 0..levels.len() {
        let modified_report: Vec<&isize> = levels
            .iter()
            .enumerate()
            .filter(|(i, _)| i != &index)
            .map(|(_, v)| v)
            .collect();
        dbg!(index, &modified_report);
        let result = check_report(modified_report);
        if result == ReportOutcome::Safe {
            return result;
        }
    }

    ReportOutcome::Unsafe
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("4", process(input)?);
        Ok(())
    }
}
