all:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o StringsTest StringsTest.cpp
	g++ -o MHashEngineTest MHashEngineTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system
	g++ -o ServerPermissionTest ServerPermissionTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon`
	g++ -o ServerUserTest ServerUserTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o HelperFunctionsTest HelperFunctionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o CommonExceptionsTest CommonExceptionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o ServerGroupTest ServerGroupTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o ServerCommandTest ServerCommandTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o testcmd testcmd.cpp
	g++ -o testcmdd testcmdd.cpp
	g++ -o testcmd2 testcmd2.cpp
	g++ -o ExecuteImplTest ExecuteImplTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o ServerCommandGroupTest ServerCommandGroupTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread
	g++ -o ClientServerDefinitionTest ClientServerDefinitionTest.cpp `pkg-config --cflags --libs librcfclient librcfcommon openssl` -lboost_filesystem -lboost_system
debug:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread -g
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread -g
	g++ -o StringsTest StringsTest.cpp -g
	g++ -o MHashEngineTest MHashEngineTest.cpp `pkg-config --cflags --libs librcfcommon` -lmhash -lboost_filesystem -lboost_system -g
	g++ -o ServerPermissionTest ServerPermissionTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon` -g
	g++ -o ServerUserTest ServerUserTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o HelperFunctionsTest HelperFunctionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o CommonExceptionsTest CommonExceptionsTest.cpp `pkg-config --cflags --libs librcfcommon` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o ServerGroupTest ServerGroupTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o ServerCommandTest ServerCommandTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o testcmd testcmd.cpp -g
	g++ -o testcmdd testcmdd.cpp -g
	g++ -o testcmd2 testcmd2.cpp -g
	g++ -o ExecuteImplTest ExecuteImplTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o ServerCommandGroupTest ServerCommandGroupTest.cpp `pkg-config --cflags --libs librcfserver librcfcommon openssl` -lboost_filesystem -lboost_system -lboost_thread -g
	g++ -o ClientServerDefinitionTest ClientServerDefinitionTest.cpp `pkg-config --cflags --libs librcfclient librcfcommon openssl` -lboost_filesystem -lboost_system -g
clean:
	rm -rf SSLTestServer SSLTestClient StringsTest MHashEngineTest ServerPermissionTest ServerUserTest HelperFunctionsTest CommonExceptionsTest ServerGroupTest ServerCommandTest testcmd testcmdd testcmd2 ExecuteImplTest ServerCommandGroupTest ClientServerDefinitionTest
