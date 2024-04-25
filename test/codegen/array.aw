foreign import: C {
func printf(str: u8*, var...args);
func puts(str: u8*);
}

func main()
{
	var s: u8[] = { 65, 66, 67, 0x20, 68, 0xA, 0 };
	printf("%s", &s);

	var t: u8[8] = s;
	printf("%s", &t);
	printf("%d, %d, %d, %d, %d, %d, %d, %d\n", t);
}
