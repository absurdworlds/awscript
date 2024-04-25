module double_colon;

func f() : vec::vec3
{
	return { 0, 1, 0 }
}

func main()
{
	common::hello_world();

	var v = f();
}
