all:
	g++ -std=c++11 -o Computing-Process/p33 Computing-Process/p33.cpp ../../src/DataAdaptor.cpp ../../src/Computing.cpp ../../src/Client.cpp ../../src/Reciever.cpp ../../src/Sender.cpp ../../src/ExecuterUnit.cpp -I ../../ -lzmq -lrt -lmsgpack -lpthread
	g++ -o Data-Process/p3 Data-Process/p3.cpp ../../src/Data.cpp -I ../../ -lzmq -lrt

clean:
	rm -rf Computing-Process/p33
	rm -rf Data-Process/p3
