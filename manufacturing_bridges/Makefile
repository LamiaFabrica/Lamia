# Lamia Manufacturing Bridge Makefile v0.3.0
CXX = g++
CXXFLAGS = -std=gnu++26 -O3 -Wall -Wextra

all: cnc arduino iot

cnc:
	$(CXX) $(CXXFLAGS) -shared -fPIC -o libcnc_bridge.so cnc_bridge.cpp

arduino:
	$(CXX) $(CXXFLAGS) -shared -fPIC -o libarduino_bridge.so arduino_bridge.cpp

iot:
	$(CXX) $(CXXFLAGS) -shared -fPIC -o libiot_bridge.so iot_bridge.cpp

clean:
	rm -f *.so

.PHONY: all clean
