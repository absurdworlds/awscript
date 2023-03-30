
function putchar(var int ch);
function puts(var cstring str);
function printf(var cstring str, var int v);


function add(var int x, var int y) : int
{
	printf("x + y = %d", x+y)
	return x+y
}

function div(var int x, var int y) : int
	return if (y < 1)
		then 0
		else x/y;

function divd(var f64 x, var f64 y) : f64
	return if (y < 1)
		then 0
		else x/y;

function test_reassign(var int c)
{
	c=32
	putchar(c)

	c=65
	putchar(c)
}

function main(var int argc) : int
{
	hello();
	printf("argc = %d\n", argc);
	printf("div = %d\n", div(1,0));

	add(66, 67);

	test_reassign(argc);

	return 0;
}

function hello()
{
	puts("Hello, world!");
}
