use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    Ok("0".to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("123", process(input)?);
        Ok(())
    }
}
