#include "webcam-ip.h"
#include "configurations.h"

namespace po = boost::program_options;
namespace cfg = configurations;

bool parseCommandLineArguments(int argc, char* argv[]) {
    po::options_description description("Options");
    description.add_options()
        ("help", "Show help")
        ("port", po::value<int>(), "Server port [default: 8080]")
        ("camera", po::value<int>(), "Camera Index [default: 0]");

    po::variables_map variablesMap;
    po::store(po::parse_command_line(argc, argv, description), variablesMap);
    po::notify(variablesMap);

    if (variablesMap.count("help")) {
        std::cout << description << "\n";
        return true;
    }

    if (variablesMap.count("port")) {
        cfg::SERVER_PORT = variablesMap["port"].as<int>();
    }

    if (variablesMap.count("camera")) {
        cfg::CAMERA_INDEX = variablesMap["camera"].as<int>();
    }

    return false;
}

void setupRoutes(crow::SimpleApp& app, std::mutex& frame_mutex, cv::Mat& frame) {
    CROW_ROUTE(app, "/")
    ([] {
        return crow::response(200, "Webcam IP Server is Running!");
    });

    CROW_ROUTE(app, "/video_feed")
    .websocket()
    .onopen([&](crow::websocket::connection& conn) {
        std::cout << "WebSocket New Connection: " << &conn << std::endl;
    })
    .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
        std::cout << "WebSocket Connection Closed: " << &conn << " reason: " << reason << std::endl;
    })
    .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
        if (!is_binary && data == "next_frame") {
            cv::Mat local_frame;
            {
                std::unique_lock<std::mutex> lock(frame_mutex);
                local_frame = frame.clone();
            }

            if (!local_frame.empty()) {
                std::vector<uchar> buf;
                cv::imencode(".jpg", local_frame, buf);
                conn.send_binary(std::string(buf.begin(), buf.end()));
            }
        }
    });
}

int main(int argc, char* argv[]) {
    try {
        if (parseCommandLineArguments(argc, argv)) {
            return 0;
        }

        std::cout << "Starting Web Server..." << std::endl;
        crow::SimpleApp app;

        cv::VideoCapture cap;
        std::cout << "Starting Camera..." << std::endl;
        
        // Try different methods to open the camera
        if (!cap.open(cfg::CAMERA_INDEX)) {
            std::cerr << "Failed to open camera with index. Trying default camera..." << std::endl;
            if (!cap.open(0)) {
                std::cerr << "Failed to open default camera. Trying to open using AVFoundation..." << std::endl;
                if (!cap.open(cv::CAP_AVFOUNDATION + cfg::CAMERA_INDEX)) {
                    std::cerr << "Camera opening error." << std::endl;
                    return -1;
                }
            }
        }

        std::mutex frame_mutex;
        cv::Mat frame;

        std::thread frameCaptureThread([&] {
            cv::Mat tmp;
            while (true) {
                if (cap.read(tmp)) {
                    std::unique_lock<std::mutex> lock(frame_mutex);
                    frame = tmp.clone();
                } else {
                    std::cerr << "Failed to capture frame" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        });

        setupRoutes(app, frame_mutex, frame);

        app.port(cfg::SERVER_PORT).multithreaded().run();
        frameCaptureThread.join();

        return 0;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}