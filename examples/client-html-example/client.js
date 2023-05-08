const videoElement = document.getElementById("video");
const socket = new WebSocket("ws://localhost:8080/video_feed");

socket.binaryType = "arraybuffer";

socket.onopen = (event) => {
    console.log("WebSocket connected:", event);
    requestNextFrame();
};

socket.onclose = (event) => {
    console.log("WebSocket closed:", event);
};

socket.onerror = (event) => {
    console.error("WebSocket error:", event);
};

socket.onmessage = (event) => {
    if (typeof event.data === "string") {
        console.log("WebSocket message:", event.data);
    } else {
        const blob = new Blob([event.data], { type: "image/jpeg" });
        const url = URL.createObjectURL(blob);
        videoElement.src = url;
        requestNextFrame();
    }
};

function requestNextFrame() {
    socket.send("next_frame");
}
