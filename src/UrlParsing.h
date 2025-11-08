#pragma once

#include "Arduino.h"
// #include "String.h"

struct ParsedUrl {
    String host;
    uint16_t port;
    String path;
    bool tls;
    bool failed;
};

char HTTPS_SCHEMA_LCASE[] = "https://";
char SEP[] = "://";
#define CHAR_NUM_OFFSET 48
#define HTTPS_SCHEMA_LENGHT 8
#define LCASE_LETTER_OFFSET 32

class UrlParsing {
    public:

        /// @brief Parses the given URL
        /// @param url The URL to parse
        /// @return A struct containing host, port and path encoded in the URL
        static ParsedUrl parseUrl(const char* url) {
            return UrlParsing::parseUrl(String(url));
        }

        /// @brief Parses the given URL
        /// @param url The URL to parse
        /// @return A struct containing host, port and path encoded in the URL
        static ParsedUrl parseUrl(const String& url) {          
            size_t urlLength = url.length();
            ParsedUrl result = ParsedUrl();

            String urlLcase = String(url);
            urlLcase.toLowerCase();

            result.tls = urlLcase.startsWith("https://");
            result.port = result.tls ? 443 : 80;

            uint8_t sepPos = 0;
            uint16_t pos = 0;

            // advance until after the "://"
            while (sepPos < 3 && pos < urlLength) {
                if (url[pos] == SEP[sepPos]) {
                    ++sepPos;
                }
                ++pos;
            }

            if (pos == urlLength) {
                result.failed = true;
                return result;
            }

            result.failed = false;

            // read host and port if available
            uint16_t hostStart = pos;
            uint16_t hostEnd = 0;
            uint16_t portStart = 0;
            uint16_t portEnd = 0;

            while (pos++ < urlLength) {
                if (url[pos] == ':') {
                    hostEnd = pos;

                    if (++pos < urlLength) {
                        portStart = pos;
                    }
                    else {
                        break;
                    }
                }
                else if (url[pos] == '/') {
                    if (portStart > 0) {
                        portEnd = pos;
                    }
                    else {
                        hostEnd = pos;
                    }
                    break;
                }
            }

            result.host = url.substring(hostStart, hostEnd);
            
            if (portEnd > 0) {
                result.port = url.substring(portStart, portEnd).toInt();
            }

            // test rest is the resource path
            result.path = url.substring(pos, urlLength);
            return result;
        }
};