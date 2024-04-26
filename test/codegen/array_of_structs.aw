foreign import: C {
func printf(str: u8*, var...args);
}

struct vec3 {
	x : int;
	y : int;
	z : int;
}

struct transform {
	pos : vec3;
	rot : vec3;
}

func test_nested_struct()
{
	var t : transform = {
		.pos = { 0, 0,  0 },
		.rot = { 0, 90, 0 },
	}

	printf("pos: %d, %d, %d, rot: %d, %d, %d\n",
		t.pos.x, t.pos.y, t.pos.z,
		t.rot.x, t.rot.y, t.rot.z)
}

func test_array_of_structs()
{
	var vecs : vec3[] = {
		{ 0, 0,  0 },
		{ 0, 90, 0 },
	}

	printf("v[0]: %d, %d, %d, v[1]: %d, %d, %d\n",
		vecs[0].x, vecs[0].y, vecs[0].z,
		vecs[1].x, vecs[1].y, vecs[1].z)
}

func test_array_of_nested()
{
	var t : transform[] = {
		{
			.pos = { 0, 0,  0 },
			.rot = { 0, 90, 0 },
		},
		{
			.pos = { 0, 5,  0 },
			.rot = { 0, 90, 0 },
		},
	}

	var i = 0
	while (i < 2) {
		printf("pos%d: %d, %d, %d, rot%d: %d, %d, %d\n",
			i, t[i].pos.x, t[i].pos.y, t[i].pos.z,
			i, t[i].rot.x, t[i].rot.y, t[i].rot.z);
		++i;
	}
}



func main()
{
	test_nested_struct();
	test_array_of_structs();
	test_array_of_nested();
}
