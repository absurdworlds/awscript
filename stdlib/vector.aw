module std;

import std::alloc;

const vector_default_capacity = 16;

// TODO: constraints, must be `Relocatable`
public class<T> vector
{
	type size_type = isize;

	// TODO: don't use null
	var data: T* = null;

	var capacity: size_type = 0;
 	var size:     size_type = 0;

	construct(this) { }

	public function push(this, value: T)
	{
	}

	private function reallocate(this, new_size: size_type)
	{
		const alloc_params: alloc::params = {
			.size  = #size_of(T),
			.align = #align_of(T),
			.count = capacity,
		};

		if data != null {
			data = alloc::default.reallocate( data, alloc_params );
		} else {
			data = alloc::default.allocate( alloc_params );
		}
	}
}
