use std::io::Error;

use crate::{check_validity, PageOrderingRule, PageUpdateList};

pub fn process(input: &str) -> std::result::Result<String, Error> {
    println!("[main start]");

    let mut middle_page_number_sum = 0;

    let mut page_ordering_rules: Vec<PageOrderingRule> = vec![];
    let mut page_update_lists: Vec<PageUpdateList> = vec![];

    for line in input.lines() {
        if line.is_empty() {
            continue;
        }

        if line.contains('|') {
            let mut iter = line.split('|');
            let before = iter.next().unwrap().parse::<usize>().unwrap();
            let after = iter.next().unwrap().parse::<usize>().unwrap();
            page_ordering_rules.push(PageOrderingRule { before, after });
        }

        if line.contains(',') {
            let mut page_list: Vec<usize> = vec![];
            for page_as_char in line.split(',') {
                page_list.push(page_as_char.parse::<usize>().unwrap());
            }

            page_update_lists.push(PageUpdateList { page_list });
        }
    }

    println!("Page Update lists count: {}", page_update_lists.len());

    let mut invalid_update_lists = vec![];

    for page_update_list in &mut page_update_lists {
        if !check_validity(&page_ordering_rules, page_update_list) {
            invalid_update_lists.push(page_update_list);
        }
    }

    println!("Invalid update list count: {}", invalid_update_lists.len());

    let mut sorted_invalid_update_lists = vec![];
    for invalid_list in &mut invalid_update_lists {
        sort_by_rules(invalid_list, &page_ordering_rules);
        sorted_invalid_update_lists.push(invalid_list);
    }

    for sorted_invalid_list in &sorted_invalid_update_lists {
        let middle_number = sorted_invalid_list.get_middle_number();
        middle_page_number_sum += middle_number;
    }

    Ok(middle_page_number_sum.to_string())
}

fn sort_by_rules(invalid_list: &mut PageUpdateList, page_ordering_rules: &Vec<PageOrderingRule>) {
    while !check_validity(page_ordering_rules, invalid_list) {
        for rule in page_ordering_rules {
            let Some((before, after)) = rule.try_get_indexes(invalid_list) else {
                continue;
            };
            if before > after {
                // Need to switch numbers
                let before_element = invalid_list.page_list.remove(before);
                invalid_list.page_list.insert(after, before_element);
            }
        }
    }
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

    #[test]
    fn test_sorting_by_rules() -> Result<(), Error> {
        let page_ordering_rules: Vec<PageOrderingRule> = vec![PageOrderingRule {
            before: 10,
            after: 12,
        }];
        let mut page_update_list: PageUpdateList = PageUpdateList {
            page_list: vec![12, 10],
        };

        assert!(!check_validity(&page_ordering_rules, &page_update_list));

        sort_by_rules(&mut page_update_list, &page_ordering_rules);

        assert!(check_validity(&page_ordering_rules, &page_update_list));

        Ok(())
    }
}
