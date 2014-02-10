
g++ -I../../include/ -L../../lib $1 -Wl,-rpath-link,../../lib -lDepthSense -o $2

