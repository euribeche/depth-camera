#include <librealsense2/rs.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third-party/stb_image_write.h"

void metadata_to_csv(const rs2::frame& frm, const std::string& filename);

int main() {
	rs2::colorizer color_map;
	rs2::pipeline pipe;
	pipe.start();

	for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();

	for (auto&& frame : pipe.wait_for_frames()) {
		if (auto vf = frame.as<rs2::video_frame>()) {
			auto stream = frame.get_profile().stream_type();
			if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);
		
			std::stringstream png_file;
			png_file << "rs-save-" << vf.get_profile().stream_name() << ".png";
			stbi_write_png(png_file.str().c_str(), vf.get_width(), 
					vf.get_height(), vf.get_bytes_per_pixel(), 
					vf.get_data(), vf.get_stride_in_bytes());
			std::cout << "Saved " << png_file.str() << std::endl;

			std::stringstream csv_file;
			csv_file << "rs-save-" << vf.get_profile().stream_name() 
				<< "-metadata.csv";
			metadata_to_csv(vf, csv_file.str());
		}
	}

	return 0;
}

void metadata_to_csv(const rs2::frame& frm, const std::string& filename) {
	std::ofstream csv;
	csv.open(filename);

	csv << "Stream," << rs2_stream_to_string(frm.get_profile().stream_type())
	       	<< "\nMetadata Attribute,Value\n";

	for (size_t i = 0; i < RS2_FRAME_METADATA_COUNT; i++) {
		if (frm.supports_frame_metadata((rs2_frame_metadata_value)i)) {
			csv << rs2_frame_metadata_to_string((rs2_frame_metadata_value)i) 
				<< "," 
				<< frm.get_frame_metadata((rs2_frame_metadata_value)i)
				<< "\n";
		}
	}
	csv.close();
}
