all:
	g++ -o Test Tester.cpp CArExtract.hpp
	./Test

clean:
	rm Test
	rm obj_*