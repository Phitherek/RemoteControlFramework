all:
	g++ -o SSLTestServer SSLTestServer.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o SSLTestClient SSLTestClient.cpp -lboost_system -lssl -lcrypto -lpthread
	g++ -o StringsTest StringsTest.cpp
clean:
	rm -rf SSLTestServer SSLTestClient StringsTest
