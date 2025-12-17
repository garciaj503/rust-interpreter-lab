fn main() {
    let a = 1;
    let b = -3;
    let c = 2;

    // Calculate the discriminant
    let discriminant: i32 = b * b - 4 * a * c;

    // Find and print solutions
    let root1 = (-b + discriminant.isqrt()) / (2 * a);
    let root2 = (-b - discriminant.isqrt()) / (2 * a);
    println!("Two distinct real roots:");
    println!("{}", root1);
    println!("{}", root2);
}