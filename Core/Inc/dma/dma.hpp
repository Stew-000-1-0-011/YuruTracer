#pragma once

namespace yuru_tracer::dma::dma {

	// HALのMspInit以外のDeInitを行い、MspInitを呼び出す
	void hal_init_deinit();
}