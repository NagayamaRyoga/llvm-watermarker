fn main() {
	(1 ..= 50).for_each(|i| match (i % 3, i % 5) {
		(0, 0) => println!("Fizz Buzz"),
		(0, _) => println!("Fizz"),
		(_, 0) => println!("Buzz"),
		(_, _) => println!("{}", i),
	});
}
