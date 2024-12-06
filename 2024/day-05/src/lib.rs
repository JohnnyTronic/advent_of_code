pub mod part1;
pub mod part2;

#[derive(Debug)]
struct PageOrderingRule {
    before: usize,
    after: usize,
}

impl PageOrderingRule {
    fn try_get_indexes(&self, page_update_list: &PageUpdateList) -> Option<(usize, usize)> {
        let before_index = page_update_list
            .page_list
            .iter()
            .position(|item| item == &self.before)?;

        let after_index = page_update_list
            .page_list
            .iter()
            .position(|item| item == &self.after)?;

        Some((before_index, after_index))
    }
}

#[derive(Debug)]
struct PageUpdateList {
    page_list: Vec<usize>,
}

impl PageUpdateList {
    fn get_middle_number(&self) -> usize {
        let middle_index = (self.page_list.len() as f32 / 2_f32).floor();
        *self.page_list.get(middle_index as usize).unwrap()
    }
}

fn check_validity(
    page_ordering_rules: &Vec<PageOrderingRule>,
    page_update_list: &PageUpdateList,
) -> bool {
    for rule in page_ordering_rules {
        let before_index_result = page_update_list
            .page_list
            .iter()
            .position(|item| item == &rule.before);

        let Some(before_index) = before_index_result else {
            continue;
        };

        let Some(after_index) = page_update_list
            .page_list
            .iter()
            .position(|item| item == &rule.after)
        else {
            continue;
        };

        if after_index < before_index {
            return false;
        }
    }

    println!("UpdateList is VALID!!! {:?}", page_update_list);
    true
}
