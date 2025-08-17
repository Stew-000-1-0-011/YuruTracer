#pragma once

#include "std_type.hpp"

namespace yuru_tracer::adc::register_map::impl {

	struct AdcRegister final {
		volatile u32 SR;
		volatile u32 CR1;
		volatile u32 CR2;
		volatile u32 SMPR1;
		volatile u32 SMPR2;
		volatile u32 JOFRx[4];
		volatile u32 HTR;
		volatile u32 LTR;
		volatile u32 SQR1;
		volatile u32 SQR2;
		volatile u32 SQR3;
		volatile u32 JSQR;
		volatile u32 JDRx[4];
		volatile u32 DR;
	};

	inline AdcRegister *const adc1 = reinterpret_cast<AdcRegister *>(static_cast<u_ptr>(0x4001'2400U));
	inline AdcRegister *const adc2 = reinterpret_cast<AdcRegister *>(static_cast<u_ptr>(0x4001'2800U));
	inline AdcRegister *const adc3 = reinterpret_cast<AdcRegister *>(static_cast<u_ptr>(0x4001'3C00U));

	namespace adc_register {
		// ADC registers
		// The peripheral registers have to be accessed by 32-bit words.

		// ADC status register
		namespace SR {
			// Regular channel Start flag
			inline constexpr u32 STRT = 0x00'00'0'10U;
			// Injected channel Start flag
			inline constexpr u32 JSTRT = 0x00'00'0'08U;
			// Injected channel end of conversion
			inline constexpr u32 JEOC = 0x00'00'0'04U;
			// Regular channel end of conversion
			inline constexpr u32 EOC = 0x00'00'0'02U;
			// Analog watchdog flag
			inline constexpr u32 AWD = 0x00'00'0'01U;
		}

		// ADC control register 1
		namespace CR1 {
			// Analog watchdog enable on regular channels
			inline constexpr u32 AWDEN = 0x00'80'00'00U;
			// Analog watchdog enable on injected channels
			inline constexpr u32 JAWDEN = 0x00'40'00'00U;
			// Dual mode selection
			inline constexpr u32 DUALMOD = 0x00'0F'00'00U;
			// Discontinuous mode channel count
			inline constexpr u32 DISCNUM = 0x00'00'E0'00U;
			// Discontinuous mode on injected channels
			inline constexpr u32 JDISCEN = 0x00'00'10'00U;
			// Discontinuous mode on regular channels
			inline constexpr u32 DISCEN = 0x00'00'08'00U;
			// Automatic injected group conversion
			inline constexpr u32 JAUTO = 0x00'00'04'00U;
			// Enable the watchdog on a single channel in scan mode
			inline constexpr u32 AWDSGL = 0x00'00'02'00U;
			// Scan mode
			inline constexpr u32 SCAN = 0x00'00'01'00U;
			// Interrupt enable for injected channels
			inline constexpr u32 JEOCIE = 0x00'00'00'80U;
			// Analog watchdog interrupt enable
			inline constexpr u32 AWDIE = 0x00'00'00'40U;
			// Interrupt enable for EOC
			inline constexpr u32 EOCIE = 0x00'00'00'20U;
			// Analog watchdog channel select bits
			inline constexpr u32 AWDCH = 0x00'00'00'1FU;
		}

		// ADC control register 2
		namespace CR2 {
			// Temperature sensor and VREFINT enable
			inline constexpr u32 TSVREFE = 0x00'80'00'00U;
			// Start conversion of regular channels
			inline constexpr u32 SWSTART = 0x00'40'00'00U;
			// Start conversion of injected channels
			inline constexpr u32 JSWSTART = 0x00'20'00'00U;
			// External trigger conversion mode for regular channels
			inline constexpr u32 EXTTRIG = 0x00'10'00'00U;
			// External event select for regular group
			inline constexpr u32 EXTSEL = 0x00'0E'00'00U;
			// External trigger conversion mode for injected channels
			inline constexpr u32 JEXTTRIG = 0x00'00'80'00U;
			// External event select for injected group
			inline constexpr u32 JEXTSEL = 0x00'00'E0'00U;
			// Data alignment
			inline constexpr u32 ALIGN = 0x00'00'08'00U;
			// Direct memory access mode
			inline constexpr u32 DMA = 0x00'00'01'00U;
			// Reset calibration
			inline constexpr u32 RSTCAL = 0x00'00'00'08U;
			// A/D calibration
			inline constexpr u32 CAL = 0x00'00'00'04U;
			// Continuous conversion
			inline constexpr u32 CONT = 0x00'00'00'02U;
			// A/D converter ON / OFF
			inline constexpr u32 ADON = 0x00'00'00'01U;
		}

		// ADC sample time register 1
		namespace SMPR1 {
			// Channel 17-10 Sample time selection
		}

		// ADC sample time register 2
		namespace SMPR2 {
			// Channel 9-0 Sample time selection
		}

		// ADC injected channel data offset register x
		namespace JOFRx {
			// Data offset for injected channel x
			inline constexpr u32 JOFFSET = 0x00'00'0F'FFU;
		}

		// ADC watchdog high threshold register
		namespace HTR {
			// Analog watchdog high threshold
			inline constexpr u32 HT = 0x00'0F'FF'FFU;
		}

		// ADC watchdog low threshold register
		namespace LTR {
			// Analog watchdog low threshold
			inline constexpr u32 LT = 0x00'0F'FF'FFU;
		}

		// ADC regular sequence register 1
		namespace SQR1 {
			// Regular channel sequence length
			inline constexpr u32 L = 0x00'F0'00'00U;
			inline constexpr u32 Lshift = 20U;
			// 16-13th conversion in regular sequence
		}

		// ADC regular sequence register 2
		namespace SQR2 {
			// 12-7th conversion in regular sequence
		}

		// ADC regular sequence register 3
		namespace SQR3 {
			// 6-1st conversion in regular sequence
		}

		// ADC injected sequence register
		namespace JSQR {
			// Injected channel sequence length
			inline constexpr u32 JL = 0x00'00'03'00U;
			// 4-1st conversion in injected sequence
		}

		// ADC injected data register x
		namespace JDRx {
			// Injected data
			inline constexpr u32 JDATA = 0x00'00'FF'FFU;
		}

		// ADC regular data register
		namespace DR {
			// ADC2 data
			inline constexpr u32 ADC2DATA = 0xFF'FF'00'00U;
			// Regular data
			inline constexpr u32 DATA = 0x00'00'FF'FFU;
		}
	}

	struct SampleTime final {
		enum Enum : u32 {
			cycles1_5 = 0b000U
			, cycles7_5 = 0b001U
			, cycles13_5 = 0b010U
			, cycles28_5 = 0b011U
			, cycles41_5 = 0b100U
			, cycles55_5 = 0b101U
			, cycles71_5 = 0b110U
			, cycles239_5 = 0b111U
		};
	};
}

namespace yuru_tracer::adc::register_map {
	using impl::AdcRegister;
	using impl::adc1;
	using impl::adc2;
	using impl::adc3;
	namespace adc_register = impl::adc_register;
	using impl::SampleTime;
}