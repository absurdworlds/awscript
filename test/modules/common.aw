foreign import: C {
func puts(str: u8*);
}

func hello_world()
{
	puts("Hello, world!")
}
