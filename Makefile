CXX = g++ -std=c++11
obj = main.o Point.o Color.o Texture.o stb_image.o Model.o Parser.o SceneGraph.o Controller.o Shader.o View.o 
LDFLAGS = -w -lGL -lGLU -lglfw -lGLEW

main : $(obj)
	$(CXX) $(obj) $(LDFLAGS) -o bin/main
	./bin/main

main.o : src/main.cpp include/SceneGraph.h include/Model.h include/View.h include/Controller.h include/Parser.h
		$(CXX) -c src/main.cpp

stb_image.o : src/stb_image.cpp
		$(CXX) -c src/stb_image.cpp

Point.o : src/Point.cpp
		$(CXX) -c src/Point.cpp

Color.o : src/Color.cpp
		$(CXX) -c src/Color.cpp

Texture.o : src/Texture.cpp
		$(CXX) -c src/Texture.cpp

Model.o : src/Model.cpp include/Point.h include/Color.h include/Texture.h
		$(CXX) -c src/Model.cpp

SceneGraph.o : src/SceneGraph.cpp include/Model.h
		$(CXX) -c src/SceneGraph.cpp

Parser.o : src/Parser.cpp include/Model.h
		$(CXX) -c src/Parser.cpp

View.o : src/View.cpp include/Model.h include/Shader.h
		$(CXX) -c src/View.cpp

Controller.o : src/Controller.cpp include/Model.h include/SceneGraph.h include/View.h
		$(CXX) -c src/Controller.cpp 

Shader.o : src/Shader.cpp
		$(CXX) -c src/Shader.cpp

clean :
	rm *.o bin/main
