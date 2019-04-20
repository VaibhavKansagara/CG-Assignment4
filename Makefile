CXX = g++ -std=c++11
obj = main.o Point.o Color.o Texture.o stb_image.o Model.o Parser.o SceneGraph.o Controller.o Shader.o View.o 
LDFLAGS = -w -lGL -lGLU -lglfw -lGLEW

main : $(obj)
	$(CXX) $(obj) $(LDFLAGS) -o bin/main
	./bin/main

main.o : src/main.cpp include/SceneGraph.h include/Model.h include/View.h include/Controller.h include/Parser.h
		$(CXX) -c src/main.cpp

stb_image.o : include/stb_image.h
		$(CXX) -c src/stb_image.cpp

Point.o : include/Point.h
		$(CXX) -c src/Point.cpp

Color.o : include/Color.h
		$(CXX) -c src/Color.cpp

Texture.o : include/Texture.h
		$(CXX) -c src/Texture.cpp

Model.o : include/Model.h include/Point.h include/Color.h include/Texture.h
		$(CXX) -c src/Model.cpp

SceneGraph.o : include/Model.h
		$(CXX) -c src/SceneGraph.cpp

Parser.o : include/Model.h
		$(CXX) -c src/Parser.cpp

View.o : include/View.h include/Model.h include/Shader.h
		$(CXX) -c src/View.cpp

Controller.o : include/Model.h include/SceneGraph.h include/View.h
		$(CXX) -c src/Controller.cpp 

Shader.o : include/Shader.h
		$(CXX) -c src/Shader.cpp

clean :
	rm *.o bin/main
