use std::io::Error;

pub fn process(_input: &str) -> std::result::Result<String, Error> {
    let total_calibration_result = 0;

    Ok(total_calibration_result.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("6", process(input)?);
        Ok(())
    }
}
