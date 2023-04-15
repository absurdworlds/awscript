
function printf(str: u8*, var...args);

function main()
{
	var x: int = 5000;
	if (0 < x < 10000)
		printf("%d\n", x);
}
