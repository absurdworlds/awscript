function putchar(var ch: int);
function puts(var str: u8*);
function printf(var str: u8*, var...args);

struct vec2 {
	var x: int;
	var y: int;
}

var point: vec2 = { .x = 10, .y = 20 };
var double: double = 4.0;
var global: int = add(point.x, point.y);

const big: i128 = 0x1EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;

function main(var argc: int) : int
{
	hello();
	test(argc);

	var test: i128 = big*2 + big / 3;

	printf("\nbig:", test);
	print(test);

	while test > 0 {
		test = test - 1;
		printf("\nbig:", test);
		print(test);
	}

	return 0;
}

function test(var argc: int)
{
	printf("argc = %d\n", argc);
	printf("div = %d\n", div(global,1));

	var x: int = 66;
	printf("x = %d\n", x);

	x = add(x, 67);
	printf("x = %d\n", x);

	test_reassign(argc);
}

function print(var v: i128)
{
	printf("%2$llx%1$llx", v);
}

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

function hello()
{
	puts("Hello, world!");
}
