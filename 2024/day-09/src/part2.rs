use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut check_sum = 0;
    Ok(check_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("34", process(input)?);
        Ok(())
    }
}
