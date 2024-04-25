foreign import: C {
func printf(var str: u8*, var...args);
}

struct test {
	x: int;
	y: float;
	z: double;
};

func main()
{
	var x: int = 1000;
	var f: f32 = 123.456;

	// expect: 1000.000000
	printf("%f\n", x as f64);
	// expect: 123.456
	printf("%g\n", f as f64);
	// expect: 7b
	printf("%x\n", (f as f64) as u32);

	// expect: false
	var z: f32 = -0.0;
	printf("%s\n", if (z as bool) "true!" else "false");

	var y = 1.0;
	var z: f32 = 1.0;

	var t: test = {
		.x = x,
		.z = y,
		.y = z,
	};

	// expect: 1000 1 1
	printf("%d %g %g\n", t.x, t.y as f64, t.z as f64);
}
