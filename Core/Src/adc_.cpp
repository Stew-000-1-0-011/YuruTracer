#include "adc.h"

namespace yuru_tracer::adc::adc {
	void hal_init_deinit() {
		HAL_ADC_DeInit(&hadc1);
		HAL_ADC_MspInit(&hadc1);
	}
}