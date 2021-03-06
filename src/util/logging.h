// use glog in default.
// llvm 3.3 failed to compile glog, uncommet the following line to use a
// compatible version
// #define COMPATIBLE_GLOG_

#ifndef COMPATIBLE_GLOG_

#include "glog/logging.h"

#else

// Copyright 2010-2013 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OR_TOOLS_BASE_LOGGING_H_
#define OR_TOOLS_BASE_LOGGING_H_

#include <assert.h>
#include <stdlib.h>
#include <iostream>  // NOLINT
//#include "util/commandlineflags.h"
#include "gflags/gflags.h"
#include "util/macros.h"

DECLARE_int32(log_level);
DECLARE_bool(log_prefix);

// Debug-only checking.
#define DCHECK(condition) assert(condition)
#define DCHECK_EQ(val1, val2) assert((val1) == (val2))
#define DCHECK_NE(val1, val2) assert((val1) != (val2))
#define DCHECK_LE(val1, val2) assert((val1) <= (val2))
#define DCHECK_LT(val1, val2) assert((val1) < (val2))
#define DCHECK_GE(val1, val2) assert((val1) >= (val2))
#define DCHECK_GT(val1, val2) assert((val1) > (val2))

// Always-on checking
#define CHECK(x) \
  if (!(x)) LogMessageFatal(__FILE__, __LINE__).stream() << "Check failed: " #x
#define CHECK_LT(x, y) CHECK((x) < (y))
#define CHECK_GT(x, y) CHECK((x) > (y))
#define CHECK_LE(x, y) CHECK((x) <= (y))
#define CHECK_GE(x, y) CHECK((x) >= (y))
#define CHECK_EQ(x, y) CHECK((x) == (y))
#define CHECK_NE(x, y) CHECK((x) != (y))
#define CHECK_NOTNULL(x) CHECK((x) != NULL)

#define LOG_INFO LogMessage(__FILE__, __LINE__)
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL

#define VLOG(x) if ((x) <= FLAGS_log_level) LOG_INFO.stream()

#define LOG(severity) LOG_ ## severity.stream()
#define LG LOG_INFO.stream()
#define LOG_IF(severity, condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() & LOG(severity)

#ifdef NDEBUG
#define DEBUG_MODE 0
#define LOG_DFATAL LOG_ERROR
#define DFATAL ERROR
#define DLOG(severity) \
  true ? (void) 0 : LogMessageVoidify() & LOG(severity)
#define DLOG_IF(severity, condition) \
  (true || !(condition)) ? (void) 0 : LogMessageVoidify() & LOG(severity)
#else
#define DEBUG_MODE 1
#define LOG_DFATAL LOG_FATAL
#define DFATAL FATAL
#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#endif

namespace operations_research {
class DateLogger {
 public:
  DateLogger();
  char* const HumanDate();
 private:
  char buffer_[9];
};
}  // namespace operations_research

class LogMessage {
 public:
  LogMessage(const char* file, int line) {
    if (FLAGS_log_prefix) {
      std::cerr << "[" << pretty_date_.HumanDate() << "] "
                << file << ":" << line << ": ";
    }
  }
  ~LogMessage() { std::cerr << "\n"; }
  std::ostream& stream() { return std::cerr; }

 private:
  operations_research::DateLogger pretty_date_;
  DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line)
    : LogMessage(file, line) { }
  ~LogMessageFatal() {
    std::cerr << "\n";
    abort();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(LogMessageFatal);
};

// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than "?:". See its usage.
  void operator&(std::ostream&) { }
};

#endif  // OR_TOOLS_BASE_LOGGING_H_

#endif  // COMPATIBLE_GLOG_
