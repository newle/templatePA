#include <string>
#include <vector>
#include <utility> //std::pair
#include <stdio.h>
#include "applyXSLT.hpp"

//使用leveldb做kv库，两个kv库，一个是html的kv库，一个是xml的kv库。
std::string applyTemplate(std::string& xml, const std::vector<std::pair<std::string, std::string> >& urls);
std::string applyTemplate(std::string url) {
	std::vector<std::pair<std::string, std::string> > urls;
	urls.push_back(std::make_pair(std::string("mainpage"), url));
	std::string xml("");
	return applyTemplate(xml, urls);
}

//===============================================================================================================
std::string getHTMLDB(std::string url, std::string& htmlheader);
std::string getHTML(std::string url, std::string& htmlheader);
bool storeHTML(const std::string& url, const std::string& html, const std::string& htmlheader);

std::string getXMLResult(std::string url, std::vector<std::pair<std::string, std::string> >& attaches);
std::string parse(const std::string& xml, const std::string& name, const std::string& url, const std::string& html, const std::string& htmlheader, std::vector<std::pair<std::string, std::string> >& attaches);
std::string mergeXML(std::string xml, std::string x);
bool storeXML(std::string url, std::string xml);

//子问题：多个子url，利用父辈解析结果，各自每个解析完成之后，合并父辈结果返回解析结果
std::string applyTemplate(std::string& xml, const std::vector<std::pair<std::string, std::string> >& urls)
{
	if(urls.size() == 0) {
		return std::string("");
	}

	for(size_t i = 0; i < urls.size(); i++) {
		std::string name = urls[i].first;
		std::string url = urls[i].second;
		std::vector<std::pair<std::string, std::string> > attaches;
		std::string x;
		if((x = getXMLResult(url, attaches)) == std::string("")) {
			std::string html;
			std::string htmlheader;
			if((html = getHTMLDB(url, htmlheader)) == std::string("")) {
				html = getHTML(url, htmlheader);
				//fprintf(stderr, "html=\n%s\n", html.c_str());
				storeHTML(url, html, htmlheader);
			}
			x = parse(xml, name, url, html, htmlheader, attaches);
		}
		xml = mergeXML(xml, x);
		applyTemplate(xml, attaches);
		storeXML(url, xml);
	}
	return xml;
}

std::string getXMLDB(std::string url);
std::vector<std::pair<std::string, std::string> > getAttachFromXML(std::string xml);
//从数据库中获取特定url对应的xml以及他们的attach
std::string getXMLResult(std::string url, std::vector<std::pair<std::string, std::string> >& attaches) {
	std::string x = getXMLDB(url);
	attaches = getAttachFromXML(x);

	return std::string("");
}

//从数据库中获取特定url对应的xml
std::string getXMLDB(std::string url) {
	return std::string("");
}
//从特定的xml中解析获得attach的url
std::vector<std::pair<std::string, std::string> > getAttachFromXML(std::string xml) {
	std::vector<std::pair<std::string, std::string> > ret;
	return ret;
}

//从数据库中获取特定url对应的html
std::string getHTMLDB(std::string url, std::string& htmlheader) {
	return std::string("");
}

extern std::string craw_url(std::string url, std::string& htmlheader, std::string encoding);
extern std::string getEncoding(std::string url);
//crawl html content
std::string getHTML(std::string url, std::string& htmlheader) {
//	return std::string("");
	return craw_url(url, htmlheader, getEncoding(url));
}

//store html to db
bool storeHTML(const std::string& url, const std::string& html, const std::string& htmlheader) {
	if(html.size() == 0) {
		return false;
	}
	
	return true;
}

//store xml to db
bool storeXML(std::string url, std::string xml) {
	if(xml.size() == 0) {
		return false;
	}
	
	return true;
}

//===============================================================================================================
//parse html to xml
std::string parse(const std::string& xml, const std::string& name, const std::string& url, const std::string& html, const std::string& htmlheader, std::vector<std::pair<std::string, std::string> >& attaches) {
	std::string x("");

	static applyXSLT* axslt = new applyXSLT();
	x = axslt->parse(xml, name, url, html, htmlheader, attaches);

	return x;
}

//merge many html
std::string mergeXML(std::string xml, std::string x) {
	return xml+x;
}



int main() {
	//std::string url("http://movie.douban.com/subject/25873036/"); 
	std::string url("http://book.douban.com/subject/26656532/"); 
	printf("xml=%s\nurl=%s\n", applyTemplate(url).c_str(), url.c_str());
	return 0;
}
