function printf(str: u8*, var...args);

function main()
{
	var x: int[3] = { 1,2,3 };

	// expect: 1
	printf("%d\n", x[0]);
	// expect: 2
	printf("%d\n", x[1]);
	// expect: 3
	printf("%d\n", x[2]);
}
