all:
	g++ -std=c++11 -o Computing-Process/p22 Computing-Process/p22.cpp ../../src/DataAdaptor.cpp ../../src/Computing.cpp ../../src/Client.cpp ../../src/Reciever.cpp ../../src/Sender.cpp ../../src/ExecuterUnit.cpp -I ../../ -lzmq -lrt -lmsgpack -lpthread
	g++ -o Data-Process/p2 Data-Process/p2.cpp ../../src/Data.cpp -I ../../ -lzmq -lrt

clean:
	rm -rf Computing-Process/p22
	rm -rf Data-Process/p2
