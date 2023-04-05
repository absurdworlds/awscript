module std;

import std::assert;
import std::alloc;
import std::lifetime;

struct<T> shared_control_block
{
	var strong_ref: atomic<int>;
	var weak_ref:   atomic<int>;
	var ptr: T*;
}

public class<T> shared
{
	var control: shared_control_block<T>*;
	var ptr: T*;

	private function add_ref(control: shared_control_block<T>*)
	{
		var& ctr = control.deref();
		++ctr.strong_ref;
		++ctr.weak_ref;
	}

	construct(this)
	{
		construct(T());
	}

	construct(this, value: &T)
	{
		#lifetime::create(alloc::default, ptr, value);

		#lifetime::create(alloc::default, ptr, shared_control_block<T>{
			.ptr = ptr,
			.strong_ref = 1,
			.weak_ref = 1,
		});
	}

	public(std)
	construct(this, control: shared_control_block<T>*)
	{
		add_ref(control);
		this.control = control;
		ptr = ctr.ptr;
	}

	copy(this, other: &shared)
	{
		add_ref(other.control);
		control = other.control;
		ptr = other.control;
	}

	destroy(this)
	{
		var& ctr = control.deref();

		if --ctr.strong_ref == 0
			#lifetime::destroy(alloc::default, ptr);

		if --ctr.weak_ref == 0
			#lifetime::destroy(alloc::default, control);
	}

	function is_unique(const this) : bool
		return control.deref().strong_ref == 1;

	function weak_ref(const this) : weak<T>
		return weak<T>( control );

	function get(this) : &T
		return ptr.deref();

	operator .(this) : &T
		return ptr.deref();
}

public class<T> weak
{
	var control: shared_control_block<T>*;

	public(std)
	construct(this, control: shared_control_block*)
	{
		this.control = control;

		++control.deref().weak_ref;
	}

	destroy(this)
	{
		var& ctr = control.deref();

		if --ctr.weak_ref == 0 {
			assert( ctr.strong_ref == 0 );
			#lifetime::destroy(alloc::default, ptr);
		}
	}

	function is_alive(this) : bool
		return ctr.deref().strong_ref > 0;

	// TODO: this might require CAS loop rather then
	// a simple return
	function promote(this) : maybe<strong<T>>
		return if is_alive()
		       then strong<T>(control)
		       else nothing;
}
