#pragma once

#include "std_type.hpp"

namespace yuru_tracer::dma::register_map::impl {
	struct Channel final {
		volatile u32 CCR;
		volatile u32 CNDTR;
		volatile u32 CPAR;
		volatile u32 CMAR;
	};

	struct DmaRegister final {
		volatile u32 ISR;
		volatile u32 IFCR;
		Channel channel[8];
	};

	inline DmaRegister *const dma1 = reinterpret_cast<DmaRegister *>(static_cast<u_ptr>(0x4002'0000U));
	inline DmaRegister *const dma2 = reinterpret_cast<DmaRegister *>(static_cast<u_ptr>(0x4002'0400U));

	namespace dma_register {
		// DMA registers
		// The peripheral registers have to be accessed by 32-bit words.

		// DMA interrupt status register
		namespace ISR {
			// Channel x transfer error flag
			// Channel x half transfer flag
			// Channel x transfer complete flag
			// Channel x global interrupt flag
		}

		// DMA interrupt flag clear register
		namespace IFCR {
			// Channel x transfer error clear
			// Channel x half transfer clear
			// Channel x transfer complete clear
			// Channel x global interrupt clear
		}

		// DMA channel x configuration register
		namespace CCRx {
			// Memory-to-memory mode
			inline constexpr u32 MEM2MEM = 0x00'00'40'00U;
			// Channel priority level
			inline constexpr u32 PL = 0x00'00'30'00U;
			inline constexpr u32 PLshift = 12U;
			// Memory size
			inline constexpr u32 MSIZE = 0x00'00'0C'00U;
			inline constexpr u32 MSIZEshift = 10U;
			// Peripheral size
			inline constexpr u32 PSIZE = 0x00'00'03'00U;
			inline constexpr u32 PSIZEshift = 8U;
			// Memory increment mode
			inline constexpr u32 MINC = 0x00'00'00'80U;
			// Peripheral increment mode
			inline constexpr u32 PINC = 0x00'00'00'40U;
			// Circular mode
			inline constexpr u32 CIRC = 0x00'00'00'20U;
			// Data transfer direction
			inline constexpr u32 DIR = 0x00'00'00'10U;
			// Transfer error interrupt enable
			inline constexpr u32 TEIE = 0x00'00'00'08U;
			// Half transfer interrupt enable
			inline constexpr u32 HTIE = 0x00'00'00'04U;
			// Transfer complete interrupt enable
			inline constexpr u32 TCIE = 0x00'00'00'02U;
			// Channel enable
			inline constexpr u32 EN = 0x00'00'00'01U;
		}

		// DMA channel x number of data register
		namespace CNDTRx {
			// Number of data to transfer
			inline constexpr u32 NDT = 0x00'00'FFFFU;
		}

		// DMA channel x peripheral address register
		namespace CPARx {
			// Peripheral address
			inline constexpr u32 PA = 0xFFFF'FFFFU;
		}

		// DMA channel x memory address register
		namespace CMARx {
			// Memory address
			inline constexpr u32 MA = 0xFFFF'FFFFU;
		}
	}

	struct ChannelPriorityLevel final {
		enum Enum : u32 {
			Low = 0b00U
			, Medium = 0b01U
			, High = 0b10U
			, VeryHigh = 0b11U
		};
	};

	struct DataSize final {
		enum Enum : u32 {
			bit8 = 0b00U
			, bit16 = 0b01U
			, bit32 = 0b10U
		};
	};
}

namespace yuru_tracer::dma::register_map {
	using impl::DmaRegister;
	using impl::Channel;
	using impl::dma1;
	using impl::dma2;
	namespace dma_register = impl::dma_register;
	using impl::ChannelPriorityLevel;
	using impl::DataSize;
}