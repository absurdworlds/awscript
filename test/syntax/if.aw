foreign import: C {
function printf(var str: u8*, var...args)
}

function main()
{
	const x = 1
	const y = 2
	// expect: no
	if x > y
		printf("yes")
	else
		printf("no")
}
