use std::io;

fn main() {
    let mut input = String::new();

    println!("Press a Lab Score: ");
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let lab: f32 = input.trim().parse().expect("Please enter a valid number");

    input.clear();
    println!("Press an Exam Score: ");
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let exam: f32 = input.trim().parse().expect("Please enter a valid number");

    let hlab = lab*0.4;
    let hexam = exam*0.6;

    let total = (hlab + hexam).round();

    let grade = match total as u32 {
        80..=100 => "A",
        75..=79 => "B+",
        70..=74 => "B",
        65..=69 => "C+",
        60..=64 => "C",
        55..=59 => "D+",
        50..=54 => "D",
        _ => "F",
    };

    println!("Lab Score(%): {}, Exam Score(%): {}, Total Score: {}, Grade: {}",
             hlab, hexam, total, grade);
}