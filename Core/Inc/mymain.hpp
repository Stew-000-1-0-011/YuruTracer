#pragma once

#include <array>

#include "main.h"
#include "gpio.h"

#include "dma/dma.hpp"
#include "dma/register_map.hpp"
#include "adc/adc.hpp"
#include "adc/register_map.hpp"
#include "std_type.hpp"
#include "motor.hpp"
#include "controller.hpp"

namespace yuru_tracer::mymain::impl {
	inline constexpr u32 channel_num = 8;

	inline void hal_init_deinit() noexcept {
		dma::dma::hal_init_deinit();
		adc::adc::hal_init_deinit();
	}

	template<u16 buffer_size_>
	inline void init_dma(volatile u16 (& buffer)[buffer_size_]) noexcept {
		using namespace dma::register_map;
		using namespace dma_register;

		// ADC1が使えるチャンネルはDMA1の1番(channel[0])
		
		// ペリフェラルアドレスにADC1のDRレジスタを設定
		dma1->channel[0].CPAR = reinterpret_cast<u_ptr>(&adc::register_map::adc1->DR);
		// メモリアドレスにバッファの先頭アドレスを設定
		dma1->channel[0].CMAR = reinterpret_cast<u_ptr>(buffer);
		// 転送データ数を設定
		dma1->channel[0].CNDTR = buffer_size_;
		
		{
			u32 ccr = 0;

			// MEM2MEMではない
			ccr &= ~CCRx::MEM2MEM;
			// 優先度を設定
			ccr |= ChannelPriorityLevel::Low << CCRx::PLshift;
			// メモリサイズを設定
			ccr |= DataSize::bit16 << CCRx::MSIZEshift;
			// ペリフェラルサイズを設定
			ccr |= DataSize::bit32 << CCRx::PSIZEshift;
			// メモリインクリメントモードする
			ccr |= CCRx::MINC;
			// ペリフェラルインクリメントモードしない
			ccr &= ~CCRx::PINC;
			// 循環モードにする
			ccr |= CCRx::CIRC;
			// データ転送方向をペリフェラルからメモリに設定
			ccr &= ~CCRx::DIR;
			// 諸々の割り込みは無効化
			ccr &= ~CCRx::TEIE;
			ccr &= ~CCRx::HTIE;
			ccr &= ~CCRx::TCIE;
			// チャンネルを有効化
			ccr |= CCRx::EN;

			// CCRレジスタに設定を書き込む
			dma1->channel[0].CCR = ccr;
		}
	}

	template<size_t n_>
	inline void init_adc(const std::array<u8, n_>& scan_order) noexcept {
		using namespace adc::register_map;
		using namespace adc_register;

		{
			u32 cr1 = 0;
			
			// Scan modeを有効化
			cr1 |= CR1::SCAN;

			// CR1レジスタに設定を書き込む
			adc1->CR1 = cr1;
		}

		{
			u32 cr2 = 0;

			// DMAを有効化
			cr2 |= CR2::DMA;
			// continuous conversion modeを有効化
			cr2 |= CR2::CONT;
			// RSTCALビットをセット
			cr2 |= CR2::RSTCAL;
			
			// CR2レジスタに設定を書き込む
			adc1->CR2 = cr2;
		}

		{
			u32 smpr2 = 0;

			// サンプリング時間を全て最小に設定
			for(u32 i = 0; i < channel_num; ++i) {
				smpr2 |= 0b000U << (3 * i);
			}

			// SMPR2レジスタに設定を書き込む
			adc1->SMPR2 = smpr2;
		}

		{
			const u32 sqr1 = (channel_num - 1) << adc_register::SQR1::Lshift;
			u32 sqr2 = 0;
			u32 sqr3 = 0;

			for(u32 i = 0; i < 6; ++i) {
				sqr3 |= scan_order[i] << (5 * i);
			}
			for(u32 i = 6; i < 8; ++i) {
				sqr2 |= scan_order[i] << (5 * (i - 6));
			}

			// SQRxレジスタに設定を書き込む
			adc1->SQR1 = sqr1;
			adc1->SQR2 = sqr2;
			adc1->SQR3 = sqr3;
		}

		// ADCを有効化
		adc1->CR2 = adc1->CR2 | CR2::ADON;

		// まずTstabだけ待つ
		// ループの中、実は2クロック分ではないか？という気もするがまあいいだろう
		volatile u64 tstab_counter = 2 * 72'000'000 / 1'000'000;
		while(tstab_counter > 0) {
			tstab_counter = tstab_counter - 1;
		}

		// RSTCALがクリアされるまで待つ
		while(adc1->CR2 & CR2::RSTCAL);

		// ADCをキャリブレーション
		adc1->CR2 = adc1->CR2 | CR2::CAL;

		// キャリブレーションが終わるまで待つ
		while(adc1->CR2 & CR2::CAL);

		// AD変換を開始
		adc1->CR2 = adc1->CR2 | CR2::ADON;
	}
	
	inline void wait(const u32 duration) noexcept {
		u32 d = duration;
		u32 tick = HAL_GetTick();
		while(true) {
			const auto now = HAL_GetTick();
			if(now < tick) {
				d = u32(-1) - tick + 1;
				tick = 0;
			}
			if(now - tick > d) {
				return;
			}
		}
	}

	inline void calibrate_photos(volatile u16 (&buffer)[channel_num], u16 (&output)[channel_num]) noexcept {
		u32 sum[channel_num]{};
		for(u32 i = 0; i < 200; ++i) {
			for(u32 i = 0; i < channel_num; ++i) {
				sum[i] += buffer[i];
			}
			wait(1);
		}

		for(u32 i = 0; i < channel_num; ++i) {
			output[i] = sum[i] / 200;
		}
	}

	inline void init(volatile u16 (&buffer)[channel_num]) noexcept {
		hal_init_deinit();
		
		init_dma(buffer);
		
		constexpr std::array<u8, channel_num> scan_order = {0, 1, 2, 3, 4, 5, 6, 7};
		init_adc(scan_order);

		motor::init();
	}

	inline void wait_button(const GPIO_PinState state) noexcept {
		u32 tick = HAL_GetTick();
		while(true) {
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) != state) {
				tick = HAL_GetTick();
			}
			if(HAL_GetTick() - tick > 500) {
				break;
			}
		}
	}



	constexpr auto fix_value(const u16 v, const u16 smin, const u16 srange) noexcept -> i32 {
		return (static_cast<i32>(v) - smin) * 10000 / srange;
	}

	// live expressionでのデバッグ用
	inline volatile u16 buffer[channel_num] = {};
	inline u16 sensor_min[channel_num] = {};
	inline u16 sensor_max[channel_num] = {};
	inline i32 sensor_fixed[channel_num] = {};
	inline volatile u32 control_duration = 0;
	// メイン処理
	inline void mymain() noexcept {
		// 初期化
		init(buffer);

		// キャリブレーション
		wait_button(GPIO_PIN_SET);
		calibrate_photos(buffer, sensor_min);
		wait_button(GPIO_PIN_RESET);
		wait(1000);
		calibrate_photos(buffer, sensor_max);
		wait_button(GPIO_PIN_SET);
		wait(1000);

		u16 sensor_range[channel_num]{};
		for(u32 i = 0; i < channel_num; ++i) {
			sensor_range[i] = sensor_max[i] - sensor_min[i];
		}

		u32 last_tick = HAL_GetTick();
		while(true) {
			const u32 current_tick = HAL_GetTick();
			const i32 dt = current_tick - last_tick;
			control_duration = dt;
			last_tick = current_tick;

			for(u32 i = 0; i < channel_num; ++i) {
				sensor_fixed[i] = fix_value(buffer[i], sensor_min[i], sensor_range[i]);
			}

			const auto [l_duty, r_duty] = controller::calc_duty(sensor_fixed, dt, motor::impl::duty_max);

			motor::set_l_duty(l_duty);
			motor::set_r_duty(r_duty);
		}
	}
}

namespace yuru_tracer::mymain {
	using impl::mymain;
}

namespace ydbg = yuru_tracer::mymain::impl;
