all:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o StringsTest StringsTest.cpp
	g++ -o MHashEngineTest MHashEngineTest.cpp `pkg-config --cflags --libs librcfcommon`
	g++ -o ServerPermissionTest ServerPermissionTest.cpp `pkg-config --cflags --libs librcfserver`
clean:
	rm -rf SSLTestServer SSLTestClient StringsTest MHashEngineTest
