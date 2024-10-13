
<h2>Introduction</h2>

cppScrape is a basic and flexible web scraper made with c++ and curl, Designed to be useable as the most minimal web browser.


***CPP LIBRARY FUNCTIONS***

below are the various functions provided by the object `Scrape` . 

  - <code>Scrape *<name> = new Scrape()</code> initialize a new scraoe object

  - <code>Scrape->setURL(std::string name)</code> : sets a target webpage to get html data from.

  - <code>Scrape->setFileName(std::string filename)</code> : sets the name of the .tsv file outputed by <code>parseByTag()</code>

  - <code>Scrape->addTag(std::string tag)</code> : adds a line to the bottem of the output file where html tags named after tag will be stored

  - <code>Scrape->addTag(std::string tag, int index)</code> : inserts a line to the output file at index, where the corresponding tag will be stored

  - <code>Scrape->URL</code> : string containing the current target url, default value is "http://example.com"

  - <code>Scrape->FILENAME</code> : string containing the filename of the output file, default valud is "out.tsv"

  - <code>Scrape->TAGS</code> : a string vector containing all html tags, with each item denoting a row in out.tsv <code>parseByHTML()</code> 

  - <code>Scrape->makeEndTag(std::string tag)</code> : converts any html tag passed to it to a valid closed version of the tag

  - <code>Scrape->sendRequest()</code> : sends a GET request to the webpage at `URL`, stores the response. returns the raw response as a string

  - <code>Scrape->parseByTags()</code> : sorts the internal responce value, and makes a .tsv file with

      1) filename as defined by FILENAME value
      2) each row will store values of its corresponding tag in TAGS (ie a h1 tag at index 2 will turn row 2 of the tsv file into a h2 row
      3) each column will contain a instance of the tag. (ie : {text from header1 on page} /t {text from a different header on page}


***CPP LIBRARY EXAMPLE USAGE***

<code>#include "cppScrape.hpp"
int main(){
  Scrape request = new Scrape();
  Scrape->setURL("http://example.com"); Scrape->addTag("\<p>");
  Scrape-> sendRequest();
  std::vector<std::string> output= (Scrape-> parseByTags());
  for (int i = 0; i < static_cast<int>(output.size()); i++){std::cout<<output.at(i)<<", "}
}
</code>

***WRAPPER EXECUTABLE USAGE***
If you want to run this utility as a command, download and compile the included main.hpp, then usage of the executable is as follows:

  - `./cppScrape <url>` : will use default <code>TAGS</code> and <code>FILENAME</code> to generate out.tsv from the given url
    
  - `./cppScrape <url>, <html tags>` : will generate a out.tsv from the given url and format out.tsv using the given html tags
