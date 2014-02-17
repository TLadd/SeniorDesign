
g++ -Wall -I/usr/local/include/opencv/ -I/opt/softkinetic/DepthSenseSDK/include/ -L/opt/softkinetic/DepthSenseSDK/lib $1 -Wl,-rpath-link,../../lib -lDepthSense $(pkg-config opencv --libs) -o $2

