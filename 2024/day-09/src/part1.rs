use std::io::Error;

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut files: Vec<(usize, u32)> = vec![];
    let mut spaces: Vec<u32> = vec![];

    let mut is_file = true;
    let mut file_id = 0;
    for (_index, char) in input.chars().filter(|i| i.is_ascii_digit()).enumerate() {
        if is_file {
            files.push((file_id, char.to_digit(10).unwrap()));
            file_id += 1;
        } else {
            spaces.push(char.to_digit(10).unwrap());
        }
        is_file = !is_file;
    }

    let mut expanded_form: Vec<String> = vec![];
    let mut space_iter = spaces.iter();
    for (file_id, file_size) in files.iter() {
        for _ in 0..*file_size {
            expanded_form.push(file_id.to_string());
        }

        if let Some(space_size) = space_iter.next() {
            for _ in 0..*space_size {
                expanded_form.push(".".to_string());
            }
        }
    }

    let mut rearranged: Vec<String> = vec![];

    let mut forward_index = 0;
    let mut reverse_index = expanded_form.len() - 1;

    while forward_index <= reverse_index {
        let forward_entry = expanded_form
            .get(forward_index)
            .expect("Ran off the end of expanded_form?!");
        if forward_entry != "." {
            rearranged.push(forward_entry.clone());
            forward_index += 1;
            continue;
        }

        loop {
            let reverse_entry = expanded_form
                .get(reverse_index)
                .expect("Ran off the front of expanded_form?!");

            if reverse_entry == "." {
                reverse_index -= 1;
                continue;
            }
            break;
        }

        rearranged.push(expanded_form.get(reverse_index).unwrap().clone());
        forward_index += 1;
        reverse_index -= 1;
    }

    // Calculate checksum
    let mut check_sum = 0;
    for (index, entry) in rearranged.iter().enumerate() {
        check_sum += index * entry.parse::<usize>().unwrap();
    }
    Ok(check_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        let input = include_str!("input-example.txt");
        assert_eq!("1928", process(input)?);
        Ok(())
    }
}
