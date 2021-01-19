CPP=g++

scraper: scraper.cpp
	$(CPP) -o scraper scraper.cpp -std=c++11 -lcurl

clean:
	rm -f ./scraper
