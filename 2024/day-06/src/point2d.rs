#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Point2D {
    pub x: isize,
    pub y: isize,
}

impl AsRef<Point2D> for Point2D {
    fn as_ref(&self) -> &Point2D {
        self
    }
}

impl<'b> std::ops::Add<&'b Point2D> for &Point2D {
    type Output = Point2D;

    fn add(self, rhs: &'b Point2D) -> Point2D {
        Point2D {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

pub enum Directions {
    East,
    South,
    West,
    North,
}

pub const NORTH: Point2D = Point2D { x: 0, y: -1 };
pub const EAST: Point2D = Point2D { x: 1, y: 0 };
pub const SOUTH: Point2D = Point2D { x: 0, y: 1 };
pub const WEST: Point2D = Point2D { x: -1, y: 0 };
