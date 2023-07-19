cppScrape, a simple curl program to extract specified html/xml tags from a given link.

By Kian Walsh 
***CONFIGURE***
So far, there is 2 major configuration options I have implemented, the taglist[] in the cppScrape.cpp file containes a array 
of html <tags>. to change how the output.tsv file formats tag content. to change, add or remove tags, changing indexes will
change what row the tag results will be placed, adding extra tags will command the program to check for those as well.

IE. taglist[] of [<h>,<p>] will return {all text marked with <h1>}\n{all text marked with <p>}

the second config is a work in progress, if the program is called by "cppScrape https://exampe.com u", the program will 
use the url given as the file name (https://example.com.tsv)

***COMPILE***
I have build this programs using msvc and g++, with the only non-standard library being libcurl (the c version, not the c++).
With your compiler and code installed, a simple command "g++ cppScrape.cpp -lcurl" command will build the program the executable.

***USAGE***
I wrote this to be as simple as possible for anyone else (or any other programs)to use, all you have to do is call "cppScrape <url>" 
with a valid url, and the program will output a .tsv file, with each row containing all text written with the tag specified in taglist[]

IE "<h1>test</h1><h1>some</h1><p>text</p>" will become "test, some, \n text"

by default, the program will find and record the title, h1-h6 tag content, and p tag content in a .tsv file
