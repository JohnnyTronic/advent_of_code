use std::io::Error;

pub fn process(_input: &str) -> std::result::Result<String, Error> {
    let stones: Vec<usize> = vec![];

    Ok(stones.len().to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("55312", process(input)?);
        Ok(())
    }
}
