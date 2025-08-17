#pragma once

#include "tim.h"

#include "std_type.hpp"

namespace yuru_tracer::motor::impl {

	constexpr i32 duty_max = 24000;

	inline auto min(const auto l, const auto r) -> auto {
		return l < r ? l : r;
	}

	inline void init() {
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	}

	struct PwmChannel final {
		TIM_HandleTypeDef * htim;
		u32 channel;
	};

	inline void set_duty(const PwmChannel& cw, const PwmChannel& ccw, const i16 duty) {
		if(duty == 0) {
			__HAL_TIM_SET_COMPARE(cw.htim, cw.channel, 0);
			__HAL_TIM_SET_COMPARE(ccw.htim, ccw.channel, 0);
		} else if(duty > 0) {
			__HAL_TIM_SET_COMPARE(cw.htim, cw.channel, 0);
			__HAL_TIM_SET_COMPARE(ccw.htim, ccw.channel, min(duty, duty_max));
		} else {
			__HAL_TIM_SET_COMPARE(cw.htim, cw.channel, min(-duty, duty_max));
			__HAL_TIM_SET_COMPARE(ccw.htim, ccw.channel, 0);
		}
	}

	volatile i16 l_duty = 0;
	volatile i16 r_duty = 0;

	inline void set_l_duty(const i16 duty) {
		l_duty = duty;
		set_duty({&htim1, TIM_CHANNEL_1}, {&htim1, TIM_CHANNEL_2}, duty);
	}

	inline void set_r_duty(const i16 duty) {
		r_duty = duty;
		set_duty({&htim1, TIM_CHANNEL_3}, {&htim1, TIM_CHANNEL_4}, duty);
	}
}

namespace yuru_tracer::motor {
	using impl::init;
	using impl::set_l_duty;
	using impl::set_r_duty;
}

namespace mdbg = yuru_tracer::motor::impl;
