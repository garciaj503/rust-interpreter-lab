
// This was the simplest algorithm I could find online:
// Bubble sort is a simple sorting algorithm that works by repeatedly stepping through a list of elements, comparing adjacent pairs, and swapping them if they are in the wrong order.

fn main() {
    // Hardcoded list of 6 integers
    let mut numbers = [42, 7, 19, 3, 25, 14];

    // Print the original list before sorting
    println!("Before sorting:");
    println!("{:?}", numbers);

    // Bubble Sort algorithm
    let len = numbers.len();

    for i in 0..len {
        for j in 0..len - i - 1 {
            if numbers[j] > numbers[j + 1] {
                let a = numbers[j];
                let b = numbers[j + 1];
                numbers[j] = b;
                numbers[j + 1] = a;
            }
        }
    }

    // Print the sorted list
    println!("After sorting:");
    println!("{:?}", numbers);
}

// Bubble Sort: https://en.wikipedia.org/wiki/Bubble_sort
// DSA Bubble Sort: https://www.w3schools.com/dsa/dsa_algo_bubblesort.php
