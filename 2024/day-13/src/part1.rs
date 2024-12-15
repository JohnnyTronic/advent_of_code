use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    Ok(input.to_string())
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
