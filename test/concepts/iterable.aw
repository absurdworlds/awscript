foreign import: C {
func printf(str: u8*, var...args);
}

func main()
{
	const strs: u8*[] = {
		"test1",
		"test2",
		"test3",
	};

	const str: u8* = "str";

	printf("%x\n", str)
	printf("%x\n", strs[0])
	printf("%x\n", strs[1])
	printf("%x\n", strs[2])
}
