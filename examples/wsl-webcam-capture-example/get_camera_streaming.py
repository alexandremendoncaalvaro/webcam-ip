import cv2
import numpy as np
from websocket import create_connection

SERVER_IP = "172.0.0.1"

def display_frame(frame_data):
    np_arr = np.frombuffer(frame_data, np.uint8)
    frame = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

    image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    cv2.imshow("Webcam Streaming", image)
    cv2.waitKey(1)


def main():
    url = f"ws://{SERVER_IP}:8080/video_feed"
    ws = create_connection(url)

    try:
        while True:
            ws.send("next_frame")
            frame_data = ws.recv()
            display_frame(frame_data)
    finally:
        ws.close()
        cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
