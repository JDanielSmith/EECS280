/* p1_library.cpp
 *
 * Libraries needed for EECS 280 project 1
 * Project UID 5366c7e2b77742d5b2142097e51561a5
 * 
 * by Andrew DeOrio <awdeorio@umich.edu>
 * 2015-04-28
 */

////////////////////////////// BEGIN csvstream.h //////////////////////////////
//////////// GitHub hash 29c19d29854d6566d5452a10d12128737a61f327 /////////////
/* -*- mode: c++ -*- */
#ifndef CSVSTREAM_H
#define CSVSTREAM_H
/* csvstream.h
 *
 * Andrew DeOrio <awdeorio@umich.edu>
 *
 * An easy-to-use CSV file parser for C++
 * https://github.com/awdeorio/csvstream
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <exception>
#include <filesystem>


// A custom exception type
struct csvstream_exception final : public std::exception {
  const char * what () const noexcept override {
    return msg.c_str();
  }
  const std::string msg;
  explicit csvstream_exception(const std::string &msg) : msg(msg) {};
};


// csvstream interface
struct csvstream final {
  // Constructor from filename. Throws csvstream_exception if open fails.
  explicit csvstream(const std::filesystem::path &filename, char delimiter=',', bool strict=true);

  // Constructor from stream
  explicit csvstream(std::istream &is, char delimiter=',', bool strict=true);

  csvstream() = delete;
  // Disable copying because copying streams is bad!
  csvstream(const csvstream&) = delete;
  csvstream& operator=(const csvstream&) = delete;
  csvstream(csvstream&&) = default;
  csvstream& operator=(csvstream&&) = default;

  // Destructor
  ~csvstream() noexcept(false);

  // Return false if an error flag on underlying stream is set
  explicit operator bool() const;

  // Return header processed by constructor
  std::vector<std::string> getheader() const;

  // Stream extraction operator reads one row. Throws csvstream_exception if
  // the number of items in a row does not match the header.
  csvstream & operator>> (std::map<std::string, std::string>& row);

  // Stream extraction operator reads one row, keeping column order. Throws
  // csvstream_exception if the number of items in a row does not match the
  // header.
  csvstream & operator>> (std::vector<std::pair<std::string, std::string> >& row);

private:
  // Filename.  Used for error messages.
  std::filesystem::path filename;

  // File stream in CSV format, used when library is called with filename ctor
  std::ifstream fin;

  // Stream in CSV format
  std::istream &is;

  // Delimiter between columns
  char delimiter;

  // Strictly enforce the number of values in each row.  Raise an exception if
  // a row contains too many values or too few compared to the header.  When
  // strict=false, ignore extra values and set missing values to empty string.
  bool strict;

  // Line no in file.  Used for error messages
  size_t line_no;

  // Store header column names
  std::vector<std::string> header;

  // Process header, the first line of the file
  void read_header();
};


///////////////////////////////////////////////////////////////////////////////
// Implementation

// Read and tokenize one line from a stream
static bool read_csv_line(std::istream &is,
                          std::vector<std::string> &data,
                          char delimiter
                          ) {

  // Add entry for first token, start with empty string
  data.clear();
  data.push_back(std::string());

  // Process one character at a time
  char c = '\0';
  enum class State {BEGIN, QUOTED, QUOTED_ESCAPED, UNQUOTED, UNQUOTED_ESCAPED, END};
  State state = State::BEGIN;
  while(is.get(c)) {
    switch (state) {
    case State::BEGIN:
      // We need this state transition to properly handle cases where nothing
      // is extracted.
      state = State::UNQUOTED;

      // Intended switch fallthrough.  Beginning with GCC7, this triggers an
      // error by default.  Disable the error for this specific line.
      #if __GNUG__ && __GNUC__ >= 7
      [[fallthrough]];
      #endif

    case State::UNQUOTED:
      if (c == '"') {
        // Change states when we see a double quote
        state = State::QUOTED;
      } else if (c == '\\') { //note this checks for a single backslash char
        state = State::UNQUOTED_ESCAPED;
        data.back() += c;
      } else if (c == delimiter) {
        // If you see a delimiter, then start a new field with an empty string
        data.push_back("");
      } else if (c == '\n' || c == '\r') {
        // If you see a line ending *and it's not within a quoted token*, stop
        // parsing the line.  Works for UNIX (\n) and OSX (\r) line endings.
        // Consumes the line ending character.
        state = State::END;
      } else {
        // Append character to current token
        data.back() += c;
      }
      break;

    case State::UNQUOTED_ESCAPED:
      // If a character is escaped, add it no matter what.
      data.back() += c;
      state = State::UNQUOTED;
      break;

    case State::QUOTED:
      if (c == '"') {
        // Change states when we see a double quote
        state = State::UNQUOTED;
      } else if (c == '\\') {
        state = State::QUOTED_ESCAPED;
        data.back() += c;
      } else {
        // Append character to current token
        data.back() += c;
      }
      break;

    case State::QUOTED_ESCAPED:
      // If a character is escaped, add it no matter what.
      data.back() += c;
      state = State::QUOTED;
      break;

    case State::END:
      if (c == '\n') {
        // Handle second character of a Windows line ending (\r\n).  Do
        // nothing, only consume the character.
      } else {
        // If this wasn't a Windows line ending, then put character back for
        // the next call to read_csv_line()
        is.unget();
      }

     #if _MSC_VER
     #pragma warning(push)
     #pragma warning(disable: 26438) //  Avoid 'goto' (es.76).
     #endif // _MSC_VER

      // We're done with this line, so break out of both the switch and loop.
      goto multilevel_break; //This is a rare example where goto is OK
      break;

      #if _MSC_VER
      #pragma warning(pop)
      #endif // _MSC_VER

    default:
      assert(0);
      throw state;

    }//switch
  }//while

 multilevel_break:
  // Clear the failbit if we extracted anything.  This is to mimic the behavior
  // of getline(), which will set the eofbit, but *not* the failbit if a partial
  // line is read.
  if (state != State::BEGIN) is.clear();

  // Return status is the underlying stream's status
  return static_cast<bool>(is);
}


csvstream::csvstream(const std::filesystem::path &filename, char delimiter, bool strict)
  : filename(filename),
    is(fin),
    delimiter(delimiter),
    strict(strict),
    line_no(0) {

  // Open file
  fin.open(filename.c_str());
  if (!fin.is_open()) {
    throw csvstream_exception("Error opening file: " + filename.string());
  }

  // Process header
  read_header();
}


csvstream::csvstream(std::istream &is, char delimiter, bool strict)
  : filename("[no filename]"),
    is(is),
    delimiter(delimiter),
    strict(strict),
    line_no(0) {
  read_header();
}


csvstream::~csvstream() noexcept(false) {
  if (fin.is_open()) fin.close();
}


csvstream::operator bool() const {
  return static_cast<bool>(is);
}


std::vector<std::string> csvstream::getheader() const {
  return header;
}


csvstream & csvstream::operator>> (std::map<std::string, std::string>& row) {
  // Clear input row
  row.clear();

  // Read one line from stream, bail out if we're at the end
  std::vector<std::string> data;
  if (!read_csv_line(is, data, delimiter)) return *this;
  line_no += 1;

  // When strict mode is disabled, coerce the length of the data.  If data is
  // larger than header, discard extra values.  If data is smaller than header,
  // pad data with empty strings.
  if (!strict) {
    data.resize(header.size());
  }

  // Check length of data
  if (data.size() != header.size()) {
    auto msg = "Number of items in row does not match header. " +
      filename.string() + ":L" + std::to_string(line_no) + " " +
      "header.size() = " + std::to_string(header.size()) + " " +
      "row.size() = " + std::to_string(data.size()) + " "
      ;
    throw csvstream_exception(msg);
  }

  // combine data and header into a row object
  for (size_t i=0; i<data.size(); ++i) {
    #if _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 26446) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #pragma warning(disable: 26458) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #endif // _MSC_VER

    row[header[i]] = data[i];

    #if _MSC_VER
    #pragma warning(pop)
    #endif // _MSC_VER
  }

  return *this;
}


csvstream & csvstream::operator>> (std::vector<std::pair<std::string, std::string> >& row) {
  // Clear input row
  row.clear();
  row.resize(header.size());

  // Read one line from stream, bail out if we're at the end
  std::vector<std::string> data;
  if (!read_csv_line(is, data, delimiter)) return *this;
  line_no += 1;

  // When strict mode is disabled, coerce the length of the data.  If data is
  // larger than header, discard extra values.  If data is smaller than header,
  // pad data with empty strings.
  if (!strict) {
    data.resize(header.size());
  }

  // Check length of data
  if (row.size() != header.size()) {
    auto msg = "Number of items in row does not match header. " +
      filename.string() + ":L" + std::to_string(line_no) + " " +
      "header.size() = " + std::to_string(header.size()) + " " +
      "row.size() = " + std::to_string(row.size()) + " "
      ;
    throw csvstream_exception(msg);
  }

  // combine data and header into a row object
  for (size_t i=0; i<data.size(); ++i) {
    #if _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 26446) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #pragma warning(disable: 26458) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #endif // _MSC_VER

    row[i] = make_pair(header[i], data[i]);

    #if _MSC_VER
    #pragma warning(pop)
    #endif // _MSC_VER
  }

  return *this;
}


void csvstream::read_header() {
  // read first line, which is the header
  if (!read_csv_line(is, header, delimiter)) {
    throw csvstream_exception("error reading header");
  }
}

#endif
/////////////////////////////// END csvstream.h ///////////////////////////////


////////////////////////////// P1 library functions /////////////////////////////

#include "p1_library.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>


void sort(std::vector<double> &v) {
  #if _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 26823) //  Dereferencing a possibly null pointer 'allotemp.1' (lifetime.1).
  #endif // _MSC_VER

  std::sort(v.begin(), v.end());

  #if _MSC_VER
  #pragma warning(pop)
  #endif
}


std::vector<double> extract_column(std::string filename,
                                   std::string column_name) {

  // open file
  std::ifstream fin;
  fin.open(filename.c_str());
  if (!fin.is_open()) {
    std::cout << "Error opening " << filename << "\n";
    exit(1);
  }

  // use csvstream to read file
  csvstream csvin(fin, '\t');

  // check for column name not found
  std::vector<std::string> header = csvin.getheader();
  size_t column = header.size();
  for (size_t i = 0; i < header.size(); ++i) {
    #if _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 26446) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #endif // _MSC_VER

    if (header[i] == column_name) {
      column = i;
      break;
    }

    #if _MSC_VER
    #pragma warning(pop)
    #endif // _MSC_VER
  }
  if (column == header.size()) {
    std::cout << "Error: column name " << column_name << " not found in "
         << filename << "\n";
    fin.close();
    exit(1);
  }

  // extract column of data
  std::vector<double> column_data;
  std::vector<std::pair<std::string, std::string>> row;
  while (csvin >> row) {

    #if _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 26446) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #pragma warning(disable: 26458) //  Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
    #endif // _MSC_VER

    column_data.push_back(stod(row[column].second));

    #if _MSC_VER
    #pragma warning(pop)
    #endif // _MSC_VER
  }

  return column_data;
}
