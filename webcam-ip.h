#pragma once

#include <boost/program_options.hpp>
#include <thread>
#include <opencv2/opencv.hpp>
#include <crow.h>
#include <atomic>

bool parseCommandLineArguments(int argc, char* argv[]);
