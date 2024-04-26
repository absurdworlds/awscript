foreign import: C {
func printf(str: u8*, var...args);
}

func main()
{
	// error: multiple declarations on a single line is forbidden by common sense
	const a = 1 const b = 2 const c = 3
	var x = 0
	var y = 0
	var z = 0

	// error: multiple statements on a single line is forbidden by common sense
	x = a + b y = b + c; z = x * y
	printf("%d %d %d\n", x, y, z)
}
