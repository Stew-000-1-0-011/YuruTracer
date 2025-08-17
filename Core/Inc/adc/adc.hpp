#pragma once

namespace yuru_tracer::adc::adc {

	// HALのMspInit以外のDeInitを行い、MspInitを呼び出す
	void hal_init_deinit();
}