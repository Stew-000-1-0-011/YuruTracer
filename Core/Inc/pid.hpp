#pragma once

#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>


namespace yuru_tracer::pid::impl {
	template<class T_>
	struct Pid final {
		T_ p;
		T_ i;
		T_ d;

		T_ integral;
		T_ prev_error;

		static constexpr std::remove_volatile_t<T_> max_integral = std::sqrt(std::numeric_limits<T_>::max());

		constexpr Pid(const std::remove_volatile_t<T_> p, const std::remove_volatile_t<T_> i, const std::remove_volatile_t<T_> d) noexcept
			: p{p}, i{i}, d{d}, integral{0}, prev_error{0} {
		}

		[[nodiscard]]
		constexpr auto calc(const std::remove_volatile_t<T_> error, float dt) noexcept -> std::remove_volatile_t<T_> {
			if(dt < 0.000001f) dt = 0.000001f;
			integral = std::max<std::remove_cvref_t<T_>>(integral + error * dt, max_integral);
			const std::remove_volatile_t<T_> derivative = (error - prev_error) / dt;
			prev_error = error;
			return p * error + i * integral + d * derivative;
		}
	};
}

namespace yuru_tracer::pid {
	using impl::Pid;
}
