all:
	g++ -std=c++11 -o Computing-Process/p11 Computing-Process/p11.cpp ../../src/DataAdaptor.cpp ../../src/Computing.cpp ../../src/Data.cpp ../../src/Client.cpp ../../src/Reciever.cpp ../../src/Sender.cpp ../../src/ExecuterUnit.cpp -I ../../ -lzmq -lrt -lmsgpack -lpthread
	g++ -o Data-Process/p1 Data-Process/p1.cpp ../../src/Data.cpp -I ../../ -lzmq -lrt

clean:
	rm -rf Computing-Process/p11
	rm -rf Data-Process/p1
