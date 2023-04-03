function putchar(var ch: int);
function puts(var str: cstring);
function printf(var str: cstring, var v: int);

function add(var x: int, var y: int) : int
{
	printf("x + y = %d\n", x+y)
	return x+y
}

function div(var x: int, var y: int) : int
	return if (y < 1)
		then 0
		else x/y;

function divd(var x: f64, var y: f64) : f64
	return if (y < 1)
		then 0
		else x/y;

function test_reassign(var c: int)
{
	c=66
	putchar(c)

	c=32
	putchar(c)

	c=65
	putchar(c)
}

function main(var argc: int) : int
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
