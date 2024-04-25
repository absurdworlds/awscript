foreign import: C {
func printf(str: u8*, var...args);
}

func main()
{
	var x: int = 5000;

	// expect: 5000
	if (0 < x < 10000)
		printf("%d\n", x);

	var y = x*2;

	// expect: 10000
	if (10000 >= y > x)
		printf("%d\n", y);
	else
		printf("%d\n", x);

	// expect: 5000
	if (10000 > y > x)
		printf("%d\n", y);
	else
		printf("%d\n", x);
}
