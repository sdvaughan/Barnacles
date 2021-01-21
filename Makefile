CPP=g++

scraper: scraper.cpp
	$(CPP) -g -o scraper scraper.cpp -std=c++11 -lcurl -lpthread

scraper_thread: scraper_thread.cpp
	$(CPP) -g -o scraper_thread scraper_thread.cpp -std=c++11 -lcurl -lpthread

clean:
	rm -f ./scraper ./scraper_thread
