foreign import: C {
func puts(str: u8*);
}

func main()
{
	var s : u8* = "passed!";

	// expect: test
	puts(s);
}
