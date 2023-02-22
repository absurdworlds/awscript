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
}
