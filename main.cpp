#include <CkSpider.h>
#include <exception> 
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include "Utils.h"

void inline crawlUrl(const std::string urlToCrawl, const int numAdditionalLinks){
  constexpr int MIN_NUM_LINKS = 10;
  std::chrono::steady_clock::time_point start, end;
  std::chrono::duration<double> duration;
  double totalTime = 0;
  CkSpider spider;

  spider.Initialize(urlToCrawl.c_str());

  search_engine::utils::assertTrue(spider.get_NumUnspidered() > 0, 
    "Error: the first parameter must be a valid URL.");
  search_engine::utils::assertTrue(numAdditionalLinks > 0, 
    "Error: the second parameter must be a positive number.");

  bool crawlSuccess;

  start = std::chrono::steady_clock::now();
  crawlSuccess = spider.CrawlNext();
  end = std::chrono::steady_clock::now();
  duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
  totalTime += duration.count();

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());
  search_engine::utils::assertTrue(spider.get_NumUnspidered() >= MIN_NUM_LINKS, 
    "Error: the url must contain " + std::to_string(MIN_NUM_LINKS) + 
    " or more links.");
  
  std::cout << spider.lastUrl() << std::endl;
  std::cout << spider.lastHtmlTitle() << std::endl;

  for(int i = 0; i < numAdditionalLinks; i++){
    start = std::chrono::steady_clock::now();
    crawlSuccess = spider.CrawlNext();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    totalTime += duration.count();

    search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());

    std::cout << spider.lastUrl() << std::endl;
    std::cout << spider.lastHtmlTitle() << std::endl;
    spider.SleepMs(1000);
  }
  std::cout << std::fixed << std::setprecision(6);
  std::cout << "Average time (seconds): " << totalTime / (numAdditionalLinks + 1) << std::endl;
}

int main(const int argc, const char **argv){
  try{
    search_engine::utils::assertTrue(argc > 2,
      "Error: an url and a number must be passed by parameter.");

    const std::string urlToCrawl = argv[1];
    int numAdditionalLinks = 0;
    
    try{
      numAdditionalLinks = std::stoi(argv[2]);
    } 
    catch(std::exception &e){
      search_engine::utils::assertTrue(false, 
        "Error: the second parameter must be a number.");
    }

    crawlUrl(urlToCrawl, numAdditionalLinks);
  } 
  catch(std::exception &e){
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
