CXX = g++
CXXFLAGS = -std=c++14 -Wall -c

express.o: response.o route.o server.o request.o utility.o
	${CXX} ${CXXFLAGS} ../../express/express.cpp

response.o: ../../express/response.cpp
	${CXX} ${CXXFLAGS} ../../express/response.cpp

route.o: ../../express/route.cpp
	${CXX} ${CXXFLAGS} ../../express/route.cpp

server.o: ../../express/server.cpp
	${CXX} ${CXXFLAGS} ../../express/server.cpp

request.o: ../../express/request.cpp
	${CXX} ${CXXFLAGS} ../../express/request.cpp

utility.o: ../../express/utility.cpp
	${CXX} ${CXXFLAGS} ../../express/utility.cpp

.PHONY: clean

clean:
	rm *.o