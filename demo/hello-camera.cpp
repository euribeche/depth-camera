#include <iostream>
#include <librealsense2/rs.hpp> // realsense api

// create a pipeline - this serves as a top-level api for streaming and processing frames
rs2::pipeline p;

// configure and start the pipeline 
p.start();

// block program until frames arrive
rs2::frameset frames = p.wait_for_frames();

// try to get a frame of a depth image
rs2::depth_frame = frames.get_depth_frames();

// get the depth frame's dimensions
float width = depth.get_width();
float height = depth.get_height();

// query the distance from the camera to the object in the center of the image
float dist_to_center = depth.get_distance(width / 2, height / 2);

// print the distance
std::cout << "The camera is facing an object" << dist_to_center << " meters away \r";
