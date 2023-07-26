#include <iostream>
#include "cppScrape.hpp"

int main(int argc, char** argv) {
	Scrape * request = new Scrape();
	if (argc > 1) {
		request->setURL(argv[1]);
	}
	if (argc > 2) {
		for (int x = 2; x < argc; x++) {
			request->addTag(argv[x]);
		}
	}
	else {
		request->addTag("<title>");
		request->addTag("<h1>");
		request->addTag("<h2>");
		request->addTag("<h3>");
		request->addTag("<h4>");
		request->addTag("<h5>");
		request->addTag("<h6>");
		request->addTag("<p>");
	}
	request->sentRequest();
	request->parseByTags();
	return(0);
}