#include <curl/curl.h>
#include <string>
#include <vector>
#include <utility>

#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


static int convertEncoding(std::string& str, const std::string& from, const std::string& to);

class getCurl {
public:
	static CURL* instance() {
		if(curl == NULL) {
			curl = curl_easy_init();
			struct curl_slist *list = NULL;
			list = curl_slist_append(list, "Connection: keep-alive");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writehtml); 
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writehtmlheader);
		}
		return curl;
	}
	static void clear() {
        curl_easy_cleanup(curl);
		curl = NULL;
	}

	static size_t writehtml(void *ptr, size_t size, size_t nmemb, void *stream)  
	{
		//printf("size*nmemb=%d\n", size*nmemb);
		std::string piece((char*)ptr, size*nmemb);
		html = html + piece;
		//fprintf(stderr, "html=\n%s\n", html.c_str());
		return size*nmemb;
	}

	static size_t writehtmlheader(void *ptr, size_t size, size_t nmemb, void *stream)  
	{
		//printf("size*nmemb=%d\n", size*nmemb);
		std::string piece((char*)ptr, size*nmemb);
		htmlheader = htmlheader + piece;
		//fprintf(stderr, "html=\n%s\n", html.c_str());
		return size*nmemb;
	}


	static std::string gethtml(const std::string& encoding) {
		//printf("html.length=%d\n", html.length());
		std::string ret("");
		if(convertEncoding(html, encoding, "gb18030") == 0) {
			ret = html;
		}
		
		html = "";
		return ret;
	}

	static std::string gethtmlheader(const std::string& encoding) {
		//printf("html.length=%d\n", html.length());
		std::string ret("");
		if(convertEncoding(htmlheader, encoding, "gb18030") == 0) {
			ret = htmlheader;
		}
		
		htmlheader = "";
		return ret;
	}


private:
	static CURL *curl;  
	static std::string html;
	static std::string htmlheader;
};

CURL* getCurl::curl = NULL;
std::string getCurl::html = std::string("");
std::string getCurl::htmlheader = std::string("");

std::string craw_url(std::string url, std::string& htmlheader, std::string encoding) {
    CURL* curl = getCurl::instance();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    CURLcode res = curl_easy_perform(curl);
    if(res==CURLE_OK) {
        fprintf(stderr, "crawl url %s success\n", url.c_str());
		htmlheader = getCurl::gethtmlheader(encoding);
        return getCurl::gethtml(encoding);
    } else {
		getCurl::clear();
    }
    return std::string("");
}

//encoding related
std::vector<std::pair<std::string, std::string> > getEncodingPatterns() {
	std::vector<std::pair<std::string, std::string> > ret;
	ret.push_back(make_pair(std::string(".douban.com/"), std::string("utf8")));
	return ret;
}

std::string getEncoding(std::string url) {
	static std::vector<std::pair<std::string, std::string> > pattern_encoding = getEncodingPatterns();
	for(size_t i = 0; i < pattern_encoding.size(); i++) {
		if(url.find(pattern_encoding[i].first) != std::string::npos) {
			return pattern_encoding[i].second;
		}
	}
	return std::string("gb18030");
}



static int convert_encoding_dup(const char *fromenc, const char *src, size_t srclen, const char *toenc, char **dst_out, size_t *dstlen_ref)
{
	iconv_t handle = iconv_open(toenc, fromenc);
	if (handle == (iconv_t)-1) {
		return -1; 
	}

	int ret = -1; 
	size_t dstlen = *dstlen_ref;
	char *s = (char *)malloc(dstlen);
	assert(s);

	char *in = (char *)src;
	size_t inleft = srclen;
	char *out = s;
	size_t outleft = dstlen;
	if (0 == iconv(handle, &in, &inleft, &out, &outleft) && inleft == 0) {
		dstlen -= outleft;
		s = (char *)realloc(s, dstlen + 1); 
		s[dstlen] = '\0';
		*dst_out = s;
		*dstlen_ref = dstlen;
		ret = 0;
	} else {
		free(s);
	}

	iconv_close(handle);
	return ret;
}

static int convertEncoding(char** s, size_t* len, const char *from, const char *to)
{
	char *in = *s;
	size_t in_len = *len;
	char *os = NULL;
	size_t oslen = (*len) * 5 + 1;
	int ret = convert_encoding_dup(from, in, in_len, to, &os, &oslen);
	if (ret == 0) {
		free(in);
		os[oslen] = '\0';
		*s = os;
		*len = oslen;
	}
	return ret;
}

static int convertEncoding(std::string& str, const std::string& from, const std::string& to) {
	char* src = strndup(str.c_str(), str.length());
	size_t len = str.length();
	if(convertEncoding(&src, &len, from.c_str(), to.c_str()) == -1) {
		free(src);
		return -1;
	}
	str = std::string(src, len);
	free(src);
	return 0;
}



