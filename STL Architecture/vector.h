#pragma once

namespace my
{

struct container_base 
{
	constexpr container_base() noexcept = default;
	container_base(const container_base&) = delete;
	container_base& operator=(const container_base&) = delete;
};

template<typename T, typename Alloc>
class vector
{
public:
	using value_type = T;
	using allocator_type = Alloc;
	using pointer = typename 
};
}