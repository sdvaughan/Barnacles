# Barnacles List Scraper
Author: Steven Daniel Vaughan (daniel@vaughan.codes)

This tool is used to retrieve a new-line separated list of URLs from a user-defined URL, then scrapes each URL for keywords of the user's choice.
It benefits from asynchronous multithreading with futures to allow for quick processing of URLs and retrieval of results in a shorter amount of time.


This can be especially useful for:
  - Tracking important topics from URL feeds
  - Retrieving data remotely using a self-hosted feed of URLs
  - Detecting phishing attacks
  
    ...and much more!
    
TODO:
  - Use list of keywords from user-provided file
  - Configure cURL requests with proxy (if requested by user)
  - ~~Keyword searching functionality~~
  - ~~Multi-threading~~
