
function putchar(var int ch)
function puts(var cstring str)
function printf(var cstring str, var int v)

function test(var int x, var int y) : int
{
	putchar(x)
	putchar(y)
}

function main(var int argc) : int
{
	printf("argc = %d\n", argc);
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
