#pragma once

#include <utility>
#include <array>

#include "std_type.hpp"
#include "pid.hpp"

namespace yuru_tracer::controller::impl {
	inline auto sensor_weight = std::array<volatile float, 8>{-2.0, -1.5, -1.0, -0.5, 0.5, 1.0, 1.5, 2.0};
	inline volatile i32 neutral_speed = 6000;
	inline pid::Pid<volatile float> pid{-2.0, 0.0, -0.00001};
	inline std::pair<volatile i16, volatile i16> duty_output;

	[[nodiscard]]
	inline auto clip(const i32 value, const i32 min, const i32 max) noexcept -> i32 {
		if(value < min) {
			return min;
		} else if(value > max) {
			return max;
		} else {
			return value;
		}
	}

	inline auto calc_sensor(const i32 (&sensor_fixed)[8]) {
		i32 angle = 0;
		for(u32 i = 0; i < 8; ++i) {
			angle += sensor_weight[i] * sensor_fixed[i];
		}
		angle /= 8;

		return angle;
	}

	volatile i32 angle = 0;
	[[nodiscard]]
	inline auto calc_duty(const i32 (&sensor_fixed)[8], const float dt, const i16 max) noexcept -> std::pair<i32, i32> {
		
		i32 angle = 0;
		for(u32 i = 0; i < 8; ++i) {
			angle += sensor_weight[i] * sensor_fixed[i];
		}
		angle /= 8;

		impl::angle = angle;

		const i32 error = angle - 0;

		const i32 duty = pid.calc(error, dt);

		const i16 l_duty = clip(neutral_speed + duty, 6000, max);
		const i16 r_duty = clip(neutral_speed - duty, 6000, max);

		duty_output = {l_duty, r_duty};
		return {l_duty, r_duty};
	}
}

namespace yuru_tracer::controller {
	using impl::calc_duty;
	using impl::neutral_speed;
}

namespace cdbg = yuru_tracer::controller::impl;
