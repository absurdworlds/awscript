foreign import: C {
function printf(var str: u8*, var...args);
}

function main()
{
	printf("+ %lld\n", 1000 + 2023);
	printf("- %lld\n", 1000 - 2023);
	printf("* %lld\n", 1000 * 2023);
	printf("/ %lld\n", 1000 / 2023);
	printf("% %lld\n", 1000 % 2023);

	printf("& %llx\n", 0b0101 & 0b0110);
	printf("^ %llx\n", 0b0101 ^ 0b0110);
	printf("| %llx\n", 0b0101 | 0b0110);

	printf("<< %llx\n", 0b0101 << 2);
	printf(">> %llx\n", 0b0101 >> 2);

	printf("<  %x\n", 1000 < 2023);
	printf(">  %x\n", 1000 < 2023);
	printf("<= %x\n", 1000 <= 2023);
	printf(">= %x\n", 1000 >= 2023);
	printf("== %x\n", 1000 == 2023);
	printf("!= %x\n", 1000 != 2023);

	printf("|| %x\n", true || false);
	printf("&& %x\n", true && false);

	printf("~ %llx\n", ~0b0111);
	printf("- %lld\n", -0b0111);
	printf("! %x\n", !true);

	var x: int = 1000;
	x += 100;
	x -= 10;
	x *= 100;
	x /= 3;
	x %= 49;
	x <<= 5;
	x >>= 3;
	x &= 0x2FEFE;
	x |= 0xEC0000;
	x ^= 0xA0000A;
	printf("%d", x);
}
