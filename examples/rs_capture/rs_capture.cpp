#include <librealsense2/rs.hpp>
#include "../example.hpp"

int main() {
	window app(1280, 720, "RealSense Capture Example");

	rs2::colorizer color_map;
	rs2::rates_printer printer;

	rs2::pipeline pipe;
	pipe.start();

	while(app) {
		rs2::frameset data = pipe.wait_for_frames().
			apply_filter(printer).
			apply_filter(color_map);
		app.show(data);
				
	}
	return 0;
}
