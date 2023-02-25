
function putchar(var int ch)
function puts(var cstring str)
function printf(var cstring str, var int v)


function add(var int x, var int y) : int
{
	printf("x + y = %d", x+y)
	return x+y
}

function div(var int x, var int y) : int
{
	if (y < 1)
		return 0;
	return x/y
}

function test_reassign(var int c)
{
	c=32
	putchar(c)

	c=65
	putchar(c)
}

function main(var int argc) : int
{
	printf("argc = %d\n", argc);

	add(66, 67);

	test_reassign(argc);

	return 0;
}
