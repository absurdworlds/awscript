
struct vec3 {
	x:int;
	y:int;
	z:int;
}

func add(a:vec3, b:vec3) : vec3
{
	return {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
	}
}

foreign import: C {
func printf(var fmt : u8*, var ...)
}

func print(v : vec3)
{
	printf("%d, %d, %d", v.x, v.y, v.z)
}
