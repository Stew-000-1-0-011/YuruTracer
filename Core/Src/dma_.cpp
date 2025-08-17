#include "dma.h"

extern "C" {
	extern DMA_HandleTypeDef hdma_adc1;
}

namespace yuru_tracer::dma::dma {
	void hal_init_deinit() {
		HAL_DMA_DeInit(&hdma_adc1);
	}
}