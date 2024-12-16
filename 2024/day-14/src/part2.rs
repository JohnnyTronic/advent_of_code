use std::io::Error;

type NumType = f64;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let answer = (-3 + 4) % 2;
    dbg!(answer);
    Ok(input.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("1206", process(input)?);
        Ok(())
    }
}
