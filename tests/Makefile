all:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o StringsTest StringsTest.cpp
	g++ -o MHashEngineTest MHashEngineTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system
	g++ -o ServerPermissionTest ServerPermissionTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon`
	g++ -o ServerUserTest ServerUserTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon` -lboost_filesystem -lboost_system
	g++ -o HelperFunctionsTest HelperFunctionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system
	g++ -o CommonExceptionsTest CommonExceptionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system
debug:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread -g
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread -g
	g++ -o StringsTest StringsTest.cpp -g
	g++ -o MHashEngineTest MHashEngineTest.cpp `pkg-config --cflags --libs librcfcommon` -lmhash -lboost_filesystem -lboost_system -g
	g++ -o ServerPermissionTest ServerPermissionTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon` -g
	g++ -o ServerUserTest ServerUserTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon` -lboost_filesystem -lboost_system -g
	g++ -o HelperFunctionsTest HelperFunctionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -g
	g++ -o CommonExceptionsTest CommonExceptionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -g
clean:
	rm -rf SSLTestServer SSLTestClient StringsTest MHashEngineTest ServerPermissionTest ServerUserTest HelperFunctionsTest CommonExceptionsTest
