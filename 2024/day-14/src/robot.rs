use glam::IVec2;

#[derive(Debug, PartialEq, Eq)]
pub struct Robot {
    pub position: IVec2,
    pub velocity: IVec2,
}
