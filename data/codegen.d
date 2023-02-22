
function putchar(var int ch);

function test(var int x, var int y) : int
{
	putchar(x)
	putchar(y)
}

function main(var int argc) : int
{
	putchar(83);
	putchar(72);
	putchar(if argc > 3 then 65 else 85);
	if argc > 1 putchar(66);
	else  putchar(85);
	putchar(83);
	putchar(72);
	if (argc > 2) putchar(10);

	argc=32
	putchar(argc)

	argc=65
	putchar(argc)

	test(66, 67);
}
