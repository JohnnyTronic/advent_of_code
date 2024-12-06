pub struct Point {
    pub x: isize,
    pub y: isize,
}

impl Point {
    pub fn add(&self, other: &Point) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl<'b> std::ops::Add<&'b Point> for &Point {
    type Output = Point;

    fn add(self, rhs: &'b Point) -> Point {
        Point {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

impl AsRef<Point> for Point {
    fn as_ref(&self) -> &Self {
        self
    }
}
