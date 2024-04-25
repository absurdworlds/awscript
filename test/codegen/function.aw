foreign import: C {
func printf(str: u8*, ...);
}

func void(var i : int)
{
	printf("test, %d\n", i);
}

func int() : int
{
	return 1;
}

func main()
{
	void(int());
}
