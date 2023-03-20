function putchar(var int ch)
function puts(var cstring str)
function printf(var cstring str, var int v)

function print(var cstring a, var cstring b)
{
	puts(a);
	puts(b);
}

function test_function(var int x, var int y) : int
{
	print("Hello, ", "world!");

	if (x>y)
	{
		x = y-1;
	}
	else
	{
		//y = x = 2+3*4;
		y = x*2+3*4;
	}

	printf("x:%d\n", x);
	printf("y:%d\n", y);

	return -3;
}


function main()
{
	return test_function(5, 10);
}
