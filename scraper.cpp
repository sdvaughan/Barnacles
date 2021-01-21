/// Barnacles List Scraper
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
#include <future>

//CURL* easy;	// Create cURL interface

std::vector<std::string> keywords;							// Vector of keywords from user list

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)		// Callback implementation
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string retrieveURL(const char* url){	                                	// Retrieve data from URL
    CURL* easy = curl_easy_init();							// Initialization

    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0);						// Non-verbose
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, WriteCallback);			// Attach callback

    std::string readBuffer;								// Create buffer to be read into
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, 10);					// Timeout
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &readBuffer);				// Attach buffer to interface
    curl_easy_setopt(easy, CURLOPT_URL, url);						// Set URL
    curl_easy_perform(easy);								// Execute cURL request
//  std::cout << url << std::endl;							// Print URL visited (**debug**)
    return readBuffer;									// Return buffer with response
}

int setProxy(std::string proxyURL, long port, CURL* easy){				// Set HTTP proxy to be used
    curl_easy_setopt(easy, CURLOPT_PROXY, proxyURL.c_str());				// Set proxy URL
    curl_easy_setopt(easy, CURLOPT_PROXYPORT, port);					// Set proxy port
}

int removeProxy(CURL* easy){								// Remove HTTP proxy
    curl_easy_setopt(easy, CURLOPT_PROXY, "");
}

std::vector<int> searchForKeyword(std::string haystack, std::string url){		// Search function
    std::vector<int> foundKWIndices;							// Vector of indices of keywords found

    for (int i = 0; i < keywords.size(); i++){						// Search URL and response for keywords
        std::size_t pos = haystack.find(keywords[i]);
        std::size_t inUrl = url.find(keywords[i]);
	if (pos != std::string::npos || inUrl != std::string::npos) {
            foundKWIndices.push_back(i);						// If found, add keyword index to vector
        }
    }
    return foundKWIndices;								// Return vector
}

int main(int argc, char * argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);							// Initialize cURL globally
    //CURL* easy;
    std::vector<std::string> foundUrls;							// Vector of found URLs

    if (argc < 3) {									// If incorrect parameters, print usage and exit
        std::cout << "Usage: ./scraper <URL> <keyword_file>" << std::endl;
        std::cout << "URL: URL pointing to list of URLs to be scraped" << std::endl;
        std::cout << "keyword_file: file with list of keywords to be searched for" << std::endl;
        exit(1);
    }

    std::cout << "List URL: " << argv[1] << std::endl;					// Print List URL provided
    std::cout << "Keyword File: " << argv[2] << std::endl;				// Print Keyword file provided

    //initializeCurl();									// Initialize
    std::string urlList = retrieveURL(argv[1]);						// Retrieve list of URLs from user-provided URL
    std::istringstream s(urlList);							// Create string stream for line-by-line iteration
    std::string line;    								// Line buffer

    std::vector<std::future<std::string>> responseFutures;				// Vecotr of futures for asynchronization
    std::vector<std::string> urls;							// URLs from list URL response

    std::ifstream keyFile(argv[2]);							// Open keyword file

    if (keyFile) { while (std::getline(keyFile, line)) {				// If valid file, store each keyword in vector
        keywords.push_back(line);
    }}

    while (std::getline(s, line)) {							// Retrieve each URL in list
        responseFutures.push_back(std::async(std::launch::async, retrieveURL, line.c_str()));	// Send URL to search function asynchronously
	urls.push_back(line);									// Add URL to vector
    }

    std::vector<std::pair<std::string, std::vector<int>>> resultWithInd;		// Vector for storage of search results


    for (auto i = 0; i < responseFutures.size(); i++) {
	std::string response = responseFutures[i].get();				// Retrieve data from URL
        std::cout << "Got response from: " << urls[i] << std::endl;
//      std::cout << response.c_str() << std::endl;					// Print data to stdout (**debug**)
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);	// Convert response to lowercase for search

        resultWithInd.push_back(std::make_pair(urls[i], searchForKeyword(response, urls[i])));	// Add result to vector
    }

    std::cout << std::endl << std::endl;

    std::vector<std::pair<std::string, std::vector<int>>>::iterator foundUrlsIt;	// Iterator for result vector

    for (foundUrlsIt = resultWithInd.begin(); foundUrlsIt != resultWithInd.end(); foundUrlsIt++){	// Print all found URLs with corresponding keywords
        if (foundUrlsIt->second.size() != 0) {
            std::string keywordString;
            int i = 0;
            for (i; i < foundUrlsIt->second.size() - 1; i++){
                keywordString += keywords[foundUrlsIt->second[i]];
                keywordString += ", ";
            }
            keywordString += keywords[foundUrlsIt->second[i]];
            std::cout << "URL: " << foundUrlsIt->first << std::endl << "Contained keywords: " << keywordString << std::endl << std::endl;
        }
    }

    return 0;
}

