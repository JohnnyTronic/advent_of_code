use std::io::Error;

use crate::{check_validity, PageOrderingRule, PageUpdateList};

pub fn process(input: &str) -> std::result::Result<String, Error> {
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

    let mut valid_update_lists = vec![];

    for page_update_list in &mut page_update_lists {
        if check_validity(&page_ordering_rules, page_update_list) {
            valid_update_lists.push(page_update_list);
        }
    }

    for valid_list in &valid_update_lists {
        let middle_number = valid_list.get_middle_number();
        middle_page_number_sum += middle_number;
    }

    Ok(middle_page_number_sum.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_process() -> Result<(), Error> {
        // let input = include_str!("input-example.txt");
        let input = "47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47";
        assert_eq!("143", process(input)?);
        Ok(())
    }

    #[test]
    fn test_check_validity_passes() -> Result<(), Error> {
        let page_ordering_rules = vec![PageOrderingRule {
            before: 10,
            after: 12,
        }];

        let page_update_list = PageUpdateList {
            page_list: vec![10, 12],
        };
        let is_valid = check_validity(&page_ordering_rules, &page_update_list);
        assert!(is_valid);

        Ok(())
    }

    #[test]
    fn test_check_validity_fails() -> Result<(), Error> {
        let page_ordering_rules = vec![PageOrderingRule {
            before: 10,
            after: 12,
        }];

        let page_update_list = PageUpdateList {
            page_list: vec![12, 10],
        };
        let is_valid = check_validity(&page_ordering_rules, &page_update_list);
        assert!(!is_valid);

        Ok(())
    }
}
