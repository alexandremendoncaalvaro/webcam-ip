# Webcam IP

A simple server application to for Streaming Webcam Image via WebSocket.  

## Motivation
As WSL2 still doesn't have a simplified native support for using a Webcam, I decided to create this very basic C++ application for Windows and capture the Webcam signal via WebSocket in my OpenCV applications running on WSL2.  

# Build
## Pre-Requisites

- Windows 7 or newer
- Git
- Visual Studio 2015 Update 3 or newer

The easiest way to build C and C++ apps applications on Windows is by using Visual Studio Community and VcPkg to manage the extensions.  

- Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/features/cplusplus/) with C++ features;  
- Clone [VcPkg GitHub repository](https://github.com/Microsoft/vcpkg) in a short path directory like **'C:\tools\vcpkg'**:
	```cmd
	git clone https://github.com/microsoft/vcpkg.git C:\tools\vcpkg
	```
- Run the VcPkg bootstrapping process
	```cmd
	cd C:\tools\vcpkg
	.\bootstrap-vcpkg.bat
	```
- Make all installed packages available to all VS projects:  
	```cmd
	vcpkg integrate install
	```

## Project requeriments
```cmd
vcpkg install opencv crow boost-program-options
```


There is no need for any additional configuration. Just build the application.

# Run Server
Go to the application folder using the terminal then run the camera server:
```cmd
.\webcam-ip.exe
```
> It will use the default options  

Options:  
  --help: Show help  
  --port **arg**: Server port [default: 8080]  
  --camera **arg**: Windows Camera Index [default: 0]  

Example:
```cmd
.\webcam-ip.exe --port 8081 --camera 1
```

## Client Example
In the folder *wsl-webcam-capture-example* there is an example for the client in python to run inside WSL2.  
The example applies a Grayscale filter in the received image fom the server.  
Change the **SERVER_IP** value in **get_camera_streaming.py** before run. 

> You will need the server runnig to test the client

### Accessing Windows networking apps from Linux WSL2 (SERVER IP):
Obtain the IP address of your host machine by running this command from your Linux distribution: 
```bash
cat /etc/resolv.conf
```
Copy the IP address following the term: nameserver.  
Connect to any Windows server using the copied IP address.  
The picture below shows an example of this by connecting to a Node.js server running in Windows via curl.  
![wsl2-network-l2w](https://learn.microsoft.com/en-us/windows/wsl/media/wsl2-network-l2w.png)