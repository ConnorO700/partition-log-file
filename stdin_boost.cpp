// Copyright 2018 Connor O'Brien
// compile with
// g++ stdin_boost.cpp -lboost_regex
// regex_match example
#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
// I asked in class and got permission from Rykalova to use
// NOLINT() on lines that are longer than 80chars error.
int main(int argc, char *argv[]) {
  std::string fn = argv[1];
  std::string s;
  std::string regdate = ("[0-9]{4}-[0-9]{2}-[0-9]{2}[ ][0-9]{2}:[0-9]{2}:[0-9]{2,}:");//NOLINT()
  std::string rs1 = ("[0-9]{4}-[0-9]{2}-[0-9]{2}[ ][0-9]{2}:[0-9]{2}:[0-9]{2,}\\.[0-9]{3}:INFO:oejs\\.AbstractConnector:Started SelectChannelConnector.*");//NOLINT()
  std::string rs = (regdate + " \\(log\\.c\\.166\\) server started.*");
// I was told even if a date is wrong aka 3018 or month 22 or day 91 that it should be found and  shouldn't make a regex that only finds correct dates.//NOLINT()
  boost::regex e;
  boost::regex e1;
  try {
    e = boost::regex(rs);
  } catch (boost::regex_error& exc) {
    std::cout << "Regex constructor failed with code " << exc.code() << std::endl;//NOLINT()
    exit(1);
  }
  try {
    e1 = boost::regex(rs1);
  } catch (boost::regex_error& exc) {
    std::cout << "Regex constructor failed with code " << exc.code() << std::endl;//NOLINT()
    exit(1);
  }
  int lineC = 0;
  bool flag = false;
  boost::posix_time::ptime t1;
  boost::posix_time::ptime t2;
  std::string temp = fn+".rpt";
  std::ofstream outfile;
  outfile.open(temp.c_str());
  std::ifstream infile;
  infile.open(fn.c_str());
  while (getline(infile, s)) {
    lineC++;
    if (flag) {
      if (regex_match(s, e1)) {
        std::cout << lineC << "(" << fn << "): " << s.substr(0, 24) << " Boot Completed" << std::endl;//NOLINT()
        outfile << lineC << "(" << fn << "): " << s.substr(0, 24) << " Boot Completed" << std::endl;//NOLINT()
        t2 = boost::posix_time::time_from_string(s.substr(0, 24));
        boost::posix_time::time_duration diff = t2 - t1;
        std::cout << "       Boot Time: "<< diff.total_milliseconds() << "ms"<< std::endl;//NOLINT()
        outfile << " Boot Time: "<< diff.total_milliseconds() << "ms"<< std::endl;//NOLINT()
        flag = false;
      }
    }
    if (regex_match(s, e)) {
      if (flag) {
        std::cout << "**** Incomplete boot ****" << std::endl;
        outfile << "**** Incomplete boot ****" << std::endl;
        break;
      }
      std::cout << lineC << "(" << fn << "): " << s.substr(0, 21) << "Boot Start" << std::endl;//NOLINT()
      outfile << lineC << "(" << fn << "): " << s.substr(0, 21) << "Boot Start" << std::endl; //NOLINT()
      t1 = boost::posix_time::time_from_string(s.substr(0, 20));
      flag = true;
    }
  }
  if (flag) {
    std::cout << "**** Incomplete boot ****" << std::endl;
    outfile << "**** Incomplete boot ****" << std::endl;
  }
  infile.close();
  outfile.close();
  return 0;
}
