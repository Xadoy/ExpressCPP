#include "response.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace CExpress;

const std::string CONTENT_LENGTH_ATTRI = "Content-Length";

const char SPACE = ' ';

const char ATTRI_PLACE_HOLDER = ':';

const std::string CRLF = "\r\n";

Response::Response() {

}

bool Response::setBodyWithFile(const std::string& file_name) {
  std::fstream file(file_name);
  if (!file.good()) {
    return false;
  }
  std::stringstream ss;
  std::string str = "";
  while (std::getline(file, str)) {
    ss << str << std::endl;
  }
  file.close();
  this->body = ss.str();

  // set content type
  auto periodIt = file_name.find(".");
  if (periodIt == file_name.npos) {
    headers["Content-type"] = "text/plain; charset=utf8";
  }
  else {
    auto mimeIt = MIME_TYPES.find(file_name.substr(periodIt));
    if (mimeIt == MIME_TYPES.end()) {
      headers["Content-type"] = "text/plain; charset=utf8";
    }
    else
    {
      headers["Content-type"] = mimeIt->second;
      headers["Content-type"].append("; charset=utf8");
    }
  }
  return true;
}

std::string CExpress::Response::generateResponseString()
{
  // make sure the content-length is set.
  try
  {
    headers.at(CONTENT_LENGTH_ATTRI);
  }
  catch (const std::exception&)
  {
    headers[CONTENT_LENGTH_ATTRI] = std::to_string(body.length());
  }

  std::stringstream result;
  // generate the status line.
  result << HTTP_VERSION << SPACE << status << SPACE;
  if (status == "200") {
    result << REASON_PHRASE_200;
  }
  else if (status == "404") {
    result << REASON_PHRASE_404;
  }
  else if (status == "500") {
    result << REASON_PHRASE_500;
  }
  else {
    //throw "Stauts not supported";
    // dont throw here, there is no outter catch!
  }
  result << CRLF;
  for (const auto& header_attribute : headers) {
    result << header_attribute.first << ATTRI_PLACE_HOLDER << SPACE << header_attribute.second << CRLF;
  }

  result << CRLF << body;

  return result.str();
}


const std::string CExpress::Response::HTTP_VERSION = "HTTP/1.1";

const std::string CExpress::Response::REASON_PHRASE_200 = "OK";

const std::string CExpress::Response::REASON_PHRASE_404 = "Not Found";

const std::string CExpress::Response::REASON_PHRASE_500 = "Internal Server Error";

const std::unordered_map<std::string, std::string> CExpress::Response::MIME_TYPES = {
  { ".aac", "audio/aac" },
  { ".abw", "application/x-abiword" },
  { ".arc", "application/octet-stream" },
  { ".avi", "video/x-msvideo" },
  { ".azw", "application/vnd.amazon.ebook" },
  { ".bin", "application/octet-stream" },
  { ".bz", "application/x-bzip" },
  { ".bz2", "application/x-bzip2" },
  { ".csh", "application/x-csh" },
  { ".css", "text/css" },
  { ".csv", "text/csv" },
  { ".doc", "application/msword" },
  { ".eot", "application/vnd.ms-fontobject" },
  { ".epub", "application/epub+zip" },
  { ".gif", "image/gif" },
  { ".htm", "text/html" },
  { ".html", "text/html" },
  { ".ico", "image/x-icon" },
  { ".ics", "text/calendar" },
  { ".jar", "application/java-archive" },
  { ".jpeg", "image/jpeg" },
  { ".jpg", "image/jpeg" },
  { ".js", "application/javascript" },
  { ".json", "application/json" },
  { ".mid", "audio/midi" },
  { ".midi", "audio/midi" },
  { ".mpeg", "video/mpeg" },
  { ".mpkg", "application/vnd.apple.installer+xml" },
  { ".odp", "application/vnd.oasis.opendocument.presentation" },
  { ".ods", "application/vnd.oasis.opendocument.spreadsheet" },
  { ".odt", "application/vnd.oasis.opendocument.text" },
  { ".oga", "audio/ogg" },
  { ".ogv", "video/ogg" },
  { ".ogx", "application/ogg" },
  { ".otf", "font/otf" },
  { ".png", "image/png" },
  { ".pdf", "application/pdf" },
  { ".ppt", "application/vnd.ms-powerpoint" },
  { ".rar", "application/x-rar-compressed" },
  { ".rtf", "application/rtf" },
  { ".sh", "application/x-sh" },
  { ".svg", "image/svg+xml" },
  { ".swf", "application/x-shockwave-flash" },
  { ".tar", "application/x-tar" },
  { ".tif", "image/tiff" },
  { ".tiff", "image/tiff" },
  { ".ts", "video/vnd.dlna.mpeg-tts" },
  { ".ttf", "font/ttf" },
  { ".vsd", "application/vnd.visio" },
  { ".wav", "audio/x-wav" },
  { ".weba", "audio/webm" },
  { ".webm", "video/webm" },
  { ".webp", "image/webp" },
  { ".woff", "font/woff" },
  { ".woff2", "font/woff2" },
  { ".xhtml", "application/xhtml+xml" },
  { ".xls", "application/vnd.ms-excel" },
  { ".xml", "application/xml" },
  { ".xul", "application/vnd.mozilla.xul+xml" },
  { ".zip", "application/zip" },
  { ".3gp", "video/3gpp" },
  { ".3g2", "video/3gpp2" },
  { ".7z", "application/x-7z-compressed" }
};