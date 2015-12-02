#include <libxslt/xslt.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxml/HTMLparser.h>
#include <libxslt/transform.h>
#include <algorithm>

static void XMLCDECL xml_error(void *ctx, const char *msg, ...)
{
	va_list v;
	if (ctx) {
		va_start(v, msg);
		vfprintf((FILE *)ctx, msg, v);
		va_end(v);
	}
}


void string_trim(std::string& str)
{
	size_t pos = str.find_first_not_of(" \f\n\r\t\v",0);
	if(pos != std::string::npos)
	{
		str.erase(0, pos);
	}

	pos = str.find_last_not_of(" \f\n\r\t\v", str.length());
	if(pos != std::string::npos && pos < str.length()-1)
	{
		str.erase(pos+1);
	}
}	

static std::string getMIMETypeFromHttpHeader(const std::string& http_header)
{
	std::string mimetype;
	size_t pos = http_header.find("\nContent-Type: ");
	if (pos != std::string::npos) {
		pos += 15;
		size_t epos = http_header.find("\n", pos);
		if (epos != std::string::npos) {
			std::string ct = http_header.substr(pos, epos - pos);
			size_t charsetpos = ct.find("charset=");
			if (charsetpos != std::string::npos) {
				mimetype = ct.substr(0, charsetpos);
				size_t split_pos = mimetype.find(';');
				if (split_pos != std::string::npos) {
					mimetype = mimetype.substr(0, split_pos);
				}
				string_trim(mimetype);
				std::transform(mimetype.begin(), mimetype.end(), mimetype.begin(), (int(*)(int))tolower);
			}
		}
	}
	return mimetype;
}


std::string getMIMEType(const std::string& http_header, const std::string& html)
{
	std::string mimetype;
	if(!http_header.empty()) {
		mimetype = getMIMETypeFromHttpHeader(http_header);
	}
	return mimetype;
}


class applyXSLT {
public:
	applyXSLT() {
		//³õÊ¼»¯
		xmlSubstituteEntitiesDefault(1);
		xmlLoadExtDtdDefaultValue = 1;
		xmlSetGenericErrorFunc(NULL, &xml_error);
	}
	~applyXSLT() {
		xsltCleanupGlobals();
		xmlCleanupParser();
	}

	std::string parse(const std::string& xml, const std::string& name, const std::string& url, const std::string& html, const std::string& htmlheader, std::vector<std::pair<std::string, std::string> >& attaches) {
		std::string ret("");

		std::string pp = getParserPath(url);
		xsltStylesheetPtr xslt = xsltParseStylesheetFile(BAD_CAST pp.c_str());
		htmlDocPtr doc = NULL;
		static std::string encoding("gb18030");
		std::string mimetype = getMIMEType(htmlheader, html);
		if (!mimetype.empty() && mimetype == "text/xml") {
			doc = html.empty() ? NULL : xmlReadDoc(BAD_CAST html.c_str(), NULL, encoding.c_str(), XML_PARSE_RECOVER);
		} else {
			doc = html.empty() ? NULL : htmlParseDoc(BAD_CAST html.c_str(), encoding.c_str());
		}
		if (doc != NULL) {
			const char *params[7] = {0};
			size_t n_param = 0;
			params[n_param] = NULL;
			xmlDocPtr res = xsltApplyStylesheet(xslt, doc, params);
			//free_xslt_params(params, n_param);
			if (res != NULL) {
				xmlChar *s = NULL;
				int len = 0;
				if (xsltSaveResultToString(&s, &len, res, xslt) >= 0) {
					ret.assign((const char *)s, len);
					xmlFree(s);
				}
				xmlFreeDoc(res);
			}
			xmlFreeDoc(doc);
		}
		return ret;
	}
private:
	std::string getParserPath(const std::string& url) {
		return std::string("./DoubanBook_20130513.xsl");
	}

};


