
func main()
{
	var p1 : u8* = "test\n";

	var i1: u64 = p1 as u64;

	var p2: u64 = i1 as u8*;

	//#assert( p2 == p1 )
}
