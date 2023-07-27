#include <curl/curl.h>
#include <fstream>
#include <vector>
#include <iostream>

class Scrape {
private:
	std::string returnedHTML = "";//actual recorded responce by external functions 
public:
	~Scrape () {
		delete& TAGS;
		delete& URL;
		delete& FILENAME;
	}

	//Configuration Functions 
	std::string URL = "http://example.com";//The target html pages address.
	std::string FILENAME = "out.tsv";//filename of new file created by parseByTags().
	std::vector<std::string> TAGS;//A list of tags to look for on the target webpage 
	void setURL(std::string link) {//Set a target address to read from.
		URL = link;
	}
	void setFileName(std::string name) {//set the filename of the new file created by parseByTags()
		FILENAME = name;
	}
	void addTag(std::string tag) {//Add tags to be looked for by parseByTags. at least one tag is needed to call parseByTags. will be added to the lowest row of output file 
		TAGS.push_back(tag);
	}
	void addTag(std::string tag, int index) {//Add tags to be looked for by parseByTags. at least one tag is needed to call parseByTags. will be added to the lowest row of output file 
		TAGS.insert(TAGS.begin() + index + index, tag);
	}

	//Helper functions for CURL
	static size_t writeRes(char* contents, size_t size, size_t nmemb, void* userp)//helper function to write the data from GET to a string 
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
	std::string makeEndTag(std::string tag) {//quick function that converts <tags> to closed </tags>
		return{ "</" + tag.substr(1) };
	}

	//core functons 
	std::string sentRequest() {//GET the address at URL, allowing for parseByTags() to run. returns A COPY of the stream of data returned from GET
		CURL* curl;
		CURLcode output;

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Scrape::writeRes);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &returnedHTML);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			output = curl_easy_perform(curl);
			if (output != CURLE_OK) {
				fprintf(stderr, "E: curl_easy_preform() failed: %s\n", curl_easy_strerror(output));
			}
			curl_easy_cleanup(curl);
		}
		else {
			std::cout << "E: curl initialization error\n";
		}
		return(returnedHTML);
	}
	std::vector<std::string>* parseByTags() {//Find each tag in TAGS, and writes every instance in order of occurence in filename.tsv
		if (TAGS.size() == 0) {
			std::cout << "CONFIGURATION ERROR, no tags given, aborting";
			return(new std::vector<std::string>);
		}
		if (returnedHTML == "") {
			std::cout << "CONFIGURATION ERROR, you must sendrequest() before you can call parseByTag(), aborting";
			return(new std::vector<std::string>);
		}
		//all lowercase
		for (char& c : returnedHTML) {
			c = tolower(c);
		}
		while (returnedHTML.find("\n") != std::string::npos) {
			int index = returnedHTML.find("\n");
			int length = 1;
			returnedHTML.erase(index, length);
		}


		//remove all tabs (tabs denote next column in .tsv formats)
		while (returnedHTML.find("\t") != std::string::npos) {
			int index = returnedHTML.find("\t");
			int length = 1;
			returnedHTML.erase(index, length);
		}


		//removing style and script to make prossessing easier 
		while (returnedHTML.find("<script") != std::string::npos) {
			int index = returnedHTML.find("<script");
			int length = returnedHTML.find("</script>") - index + 9;
			returnedHTML.erase(index, length);
		}
		while (returnedHTML.find("<style") != std::string::npos) {
			int index = returnedHTML.find("<style");
			int length = returnedHTML.find("</style>") - index + 8;
			returnedHTML.erase(index, length);
		}

		//remove yahoo finances horrible quote markers
		while (returnedHTML.find("&quot;") != std::string::npos) {
			int index = returnedHTML.find("&quot;");
			int length = 6;
			returnedHTML.erase(index, length);
		}

		std::ofstream out(FILENAME.c_str());//output file 

		std::vector<std::string> toWrite(TAGS.size(), "");//containes list of strings, each corresponding to a html tag

		for (int i = 0; i < static_cast<int>(TAGS.size()); i++) {
			std::string tag = TAGS.at(i);
			std::string endTag = makeEndTag(tag);
			while (returnedHTML.find(tag) != std::string::npos) {
				int currentIndex = returnedHTML.find(tag);
				int nextIndex = returnedHTML.find(endTag);
				if (currentIndex < nextIndex) {
					toWrite.at(i) += returnedHTML.substr(currentIndex, nextIndex - currentIndex + endTag.length()) + "\t";
					returnedHTML.erase(currentIndex, nextIndex - currentIndex + endTag.length());
				}
				else {
					break;
				}
			}
			//remove any extra long spaces
			while (toWrite.at(i).find("  ") != std::string::npos) {
				int index = toWrite.at(i).find("  ");
				toWrite.at(i).erase(index, 1);
			}
			//remove and nested tags within target tags
			while (toWrite.at(i).find("<") != std::string::npos) {
				int index = toWrite.at(i).find("<");
				int length = toWrite.at(i).find(">") - index + 1;
				toWrite.at(i).erase(index, length);
			}
		}
		//write queue to file 
		for (int i = 0; i < static_cast<int>(toWrite.size()); i++) {
			out << toWrite[i] << "\n";
		}
		return(&toWrite);
		out.close();
	}


};