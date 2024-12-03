use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let mut safe_report_count = 0;

    let lines = input.lines();
    // consumes the iterator, returns an (Optional) String
    'lineloop: for line in lines {
        dbg!(line);
        let split: Vec<isize> = line
            .split_ascii_whitespace()
            .map(|val| val.parse::<isize>().unwrap())
            .collect();
        let mut numbers = split.iter();

        let mut target_value = numbers.next().unwrap();
        let next_value = numbers.next().unwrap();
        let mut delta = next_value - target_value;
        if delta.abs() < 1 || delta.abs() > 3 {
            continue 'lineloop;
        }

        let mut trend = (next_value - target_value).signum();
        target_value = next_value;
        while let Some(next_value) = numbers.next() {
            delta = next_value - target_value;
            if delta.abs() < 1 || delta.abs() > 3 {
                println!("Unsafe: out-of-spec delta: {}", delta);
                continue 'lineloop;
            }
            let next_trend = (delta).signum();
            if next_trend != trend {
                // This is an unsafe report
                println!("Unsafe: inconsistent trend");
                continue 'lineloop;
            }
            target_value = next_value;
            trend = next_trend;
        }
        safe_report_count += 1;
        println!("SAFE! Count: {}", safe_report_count);
    }

    Ok(safe_report_count.to_string())
    // left.sort();
    // right.sort();
    //
    // let zipped: Vec<(&isize, &isize)> = left.iter().zip(&right).collect();
    //
    // let deltas: Vec<isize> = zipped
    //     .iter()
    //     .map(|(&left, &right)| (left - right).abs())
    //     .collect();
    //
    // let sum: isize = deltas.iter().sum();
    // println!("Final sum: {}", sum);
    //
    // println!("[main end]");
    //
    // Ok(sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("2", process(input)?);
        Ok(())
    }
}
