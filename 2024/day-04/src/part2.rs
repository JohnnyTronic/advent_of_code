use std::io::Error;

pub fn process(_input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");
    let xmas_count = 0;

    Ok(xmas_count.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

        assert_eq!("48", process(input)?);
        Ok(())
    }
}
