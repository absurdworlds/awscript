module std;

import std::alloc;
import std::lifetime;

public class<T> unique
{
	var ptr: T*;

	construct(this)
	{
		construct(T());
	}

	construct(this)
	{
		#lifetime::create(alloc::default, ptr);
	}

	destroy(this)
	{
		#lifetime::destroy(alloc::default, ptr);
	}
	
}
