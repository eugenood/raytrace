CXX = clang++
CXXFLAGS = @compile_flags.txt
SRCS = main.cpp camera.cpp hittable.cpp image.cpp interval.cpp material.cpp ray.cpp util.cpp vector.cpp
TARGET = raytrace

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)