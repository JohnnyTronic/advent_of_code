use std::io::Error;

#[derive(Debug)]
struct File {
    pub id: usize,
    pub position: usize,
    pub length: usize,
}

#[derive(Debug)]
struct FreeSpace {
    pub position: usize,
    pub length: usize,
}

pub fn process(input: &str) -> std::result::Result<String, Error> {
    let mut files: Vec<File> = vec![];
    let mut spaces: Vec<FreeSpace> = vec![];

    let mut is_file = true;
    let mut file_id = 0;
    let mut position_pointer = 0;

    for char in input.chars().filter(|i| i.is_ascii_digit()) {
        if is_file {
            let new_file = File {
                id: file_id,
                length: char.to_digit(10).unwrap() as usize,
                position: position_pointer,
            };
            file_id += 1;
            position_pointer += &new_file.length;
            files.push(new_file);
        } else {
            let new_space = FreeSpace {
                position: position_pointer,
                length: char.to_digit(10).unwrap() as usize,
            };
            position_pointer += new_space.length;
            spaces.push(new_space);
        }
        is_file = !is_file;
    }

    for file in files.iter_mut().rev() {
        for space in spaces.iter_mut() {
            if space.length >= file.length && space.position < file.position {
                if space.position >= file.position {
                    println!("Why are we considering a space to the right?");
                }
                file.position = space.position;
                space.length -= file.length;
                if space.length > 0 {
                    space.position += file.length;
                } else {
                    println!("SPACE COMPLETELY FILLED");
                }
                break;
            }
        }
    }

    files.sort_by(|a, b| a.position.cmp(&b.position));
    // let last_file = files.last().unwrap();
    // let drive_size = last_file.position + last_file.length;
    // let mut drive_visual: Vec<String> = vec![".".to_string(); drive_size];
    // for file in &files {
    //     for i in 0..file.length {
    //         let index = file.position + i;
    //         drive_visual[index] = file.id.to_string();
    //     }
    // }
    // dbg!(drive_visual);

    // Calculate checksum
    let mut check_sum = 0_usize;
    for file in files {
        for i in 0..file.length {
            let index = file.position + i;
            let contribution = index.checked_mul(file.id).expect("Contribution overflow!");
            check_sum = check_sum
                .checked_add(contribution)
                .expect("Checksum overflow!");
            // dbg!(contribution, check_sum);
        }
    }
    Ok(check_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        // assert_eq!("2858-", process(input)?);
        let input = include_str!("input-example2.txt");
        assert_eq!("149706", process(input)?);
        Ok(())
    }
}
