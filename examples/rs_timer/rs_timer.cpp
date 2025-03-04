#include <librealsense2/rs.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third-party/stb_image_write.h"

int main() {
	int image_count = 0;
	
	rs2::colorizer color_map;
	rs2::pipeline pipe;
	pipe.start();

	for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();

	while(true) {
		for (auto&& frame : pipe.wait_for_frames()) {
			if (auto vf = frame.as<rs2::video_frame>()) {
				auto stream = frame.get_profile().stream_type();
				if (vf.is<rs2::depth_frame>()) continue;

				std::stringstream png_file;
				png_file << "rs-save-" << vf.get_profile().stream_name() << "-" << image_count++ << ".png";
				stbi_write_png(png_file.str().c_str(), vf.get_width(),
						vf.get_height(), vf.get_bytes_per_pixel(),
						vf.get_data(), vf.get_stride_in_bytes());
				std::cout << "Saved " << png_file.str() << std::endl;
			}
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}
