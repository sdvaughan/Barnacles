// Barnacles List Scraper
// Author: Steven D. Vaughan

// This program is used to retrieve a new-line separated list of URLs from a user-defined URL,
// then scrapes each URL for keywords of the user's choice.

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <thread>
#include <vector>
#include <bits/stdc++.h>

CURL* easy;	// Create cURL interface

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)	// Callback implementation
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int initializeCurl(){								// Initialize cURL interface settings
    curl_global_init(CURL_GLOBAL_ALL);
    easy = curl_easy_init();							// Initialization

    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0);					// Non-verbose
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, WriteCallback);		// Attach callback
}

std::string retrieveURL(const char* url){					// Retrieve data from URL
    std::string readBuffer;							// Create buffer to be read into
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &readBuffer);			// Attach buffer to interface
    curl_easy_setopt(easy, CURLOPT_URL, url);					// Set URL
    curl_easy_perform(easy);							// Execute cURL request
    std::cout << url << std::endl;						// Print URL visited (**debug**)
    return readBuffer;								// Return buffer with response
}

int setProxy(std::string proxyURL, long port){					// Set HTTP proxy to be used
    curl_easy_setopt(easy, CURLOPT_PROXY, proxyURL.c_str());			// Set proxy URL
    curl_easy_setopt(easy, CURLOPT_PROXYPORT, port);				// Set proxy port
}

int removeProxy(){								// Remove HTTP proxy
    curl_easy_setopt(easy, CURLOPT_PROXY, "");
}

int main(int argc, char * argv[])
{
    std::vector<std::string> foundUrls;

    if (argc < 2) {
        std::cout << "Usage: ./scraper <URL> <keyword>" << std::endl;
        std::cout << "URL: URL pointing to list of URLs to be scraped" << std::endl;
        std::cout << "keyword: keyword to be searched for" << std::endl;
        exit(1);
    }

    std::cout << "List URL: " << argv[1] << std::endl;
    std::cout << "Keyword: " << argv[2] << std::endl;

    initializeCurl();									// Initialize
    std::string urlList = retrieveURL(argv[1]);						// Retrieve list of URLs from user-provided URL
    std::istringstream s(urlList);							// Create string stream for line-by-line iteration
    std::string line;    								// Line buffer

    while (std::getline(s, line)) {							// Retrieve next URL in list
	std::string response = retrieveURL(line.c_str());				// Retrieve data from URL
//	std::cout << response.c_str() << std::endl;					// Print data to stdout (**debug**)
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);	// Convert response to lowercase for search
        std::size_t found = response.find(argv[2]);					// Search for keyword
        if (found != std::string::npos) {
            foundUrls.push_back(line);
	    std::cout << "Found Keyword!" << std::endl;
        }
    }

    std::cout << std::endl << std::endl;

    std::vector<std::string>::iterator foundUrlsIt;

    for (foundUrlsIt = foundUrls.begin(); foundUrlsIt != foundUrls.end(); foundUrlsIt++){
	std::cout << *foundUrlsIt << std::endl;
    }

    return 0;
}
