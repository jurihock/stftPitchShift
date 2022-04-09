#ifndef _ANYOPTION_H
#define _ANYOPTION_H

#define _CRT_SECURE_NO_WARNINGS /* Microsoft C/C++ Compiler: Disable C4996     \
                                   warnings for security-enhanced CRT          \
                                   functions */

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

#define COMMON_OPT 1
#define COMMAND_OPT 2
#define FILE_OPT 3
#define COMMON_FLAG 4
#define COMMAND_FLAG 5
#define FILE_FLAG 6

#define COMMAND_OPTION_TYPE 1
#define COMMAND_FLAG_TYPE 2
#define FILE_OPTION_TYPE 3
#define FILE_FLAG_TYPE 4
#define UNKNOWN_TYPE 5

#define DEFAULT_MAXOPTS 10
#define MAX_LONG_PREFIX_LENGTH 2

#define DEFAULT_MAXUSAGE 3
#define DEFAULT_MAXHELP 10

#define TRUE_FLAG "true"

using namespace std;

class AnyOption {

public: /* the public interface */
  AnyOption();

  explicit AnyOption(unsigned int maxoptions);
  explicit AnyOption(unsigned int maxoptions, unsigned int maxcharoptions);
  ~AnyOption();

  /*
   * following set methods specifies the
   * special characters and delimiters
   * if not set traditional defaults will be used
   */

  void setCommandPrefixChar(char _prefix);    /* '-' in "-w" */
  void setCommandLongPrefix(const char *_prefix); /* '--' in "--width" */
  void setFileCommentChar(char _comment);     /* '#' in shell scripts */
  void setFileDelimiterChar(char _delimiter); /* ':' in "width : 100" */

  /*
   * provide the input for the options
   * like argv[] for commnd line and the
   * option file name  to use;
   */

  void useCommandArgs(int _argc, char **_argv);
  void useFiileName(const char *_filename);

  /*
   * turn off the POSIX style options
   * this means anything starting with a '-' or "--"
   * will be considered a valid option
   * which also means you cannot add a bunch of
   * POIX options chars together like "-lr"  for "-l -r"
   *
   */

  void noPOSIX();

  /*
   * prints warning verbose if you set anything wrong
   */
  void setVerbose();

  /*
   * there are two types of options
   *
   * Option - has an associated value ( -w 100 )
   * Flag  - no value, just a boolean flag  ( -nogui )
   *
   * the options can be either a string ( GNU style )
   * or a character ( traditional POSIX style )
   * or both ( --width, -w )
   *
   * the options can be common to the command line and
   * the option file, or can belong only to either of
   * command line and option file
   *
   * following set methods, handle all the above
   * cases of options.
   */

  /* options command to command line and option file */
  void setOption(const char *opt_string);
  void setOption(char opt_char);
  void setOption(const char *opt_string, char opt_char);
  void setFlag(const char *opt_string);
  void setFlag(char opt_char);
  void setFlag(const char *opt_string, char opt_char);

  /* options read from command line only */
  void setCommandOption(const char *opt_string);
  void setCommandOption(char opt_char);
  void setCommandOption(const char *opt_string, char opt_char);
  void setCommandFlag(const char *opt_string);
  void setCommandFlag(char opt_char);
  void setCommandFlag(const char *opt_string, char opt_char);

  /* options read from an option file only  */
  void setFileOption(const char *opt_string);
  void setFileOption(char opt_char);
  void setFileOption(const char *opt_string, char opt_char);
  void setFileFlag(const char *opt_string);
  void setFileFlag(char opt_char);
  void setFileFlag(const char *opt_string, char opt_char);

  /*
   * process the options, registered using
   * useCommandArgs() and useFileName();
   */
  void processOptions();
  void processCommandArgs();
  void processCommandArgs(unsigned int max_args);
  bool processFile();

  /*
   * process the specified options
   */
  void processCommandArgs(int _argc, char **_argv);
  void processCommandArgs(int _argc, char **_argv, int max_args);
  bool processFile(const char *_filename);

  /*
   * get the value of the options
   * will return NULL if no value is set
   */
  char *getValue(const char *_option);
  bool getFlag(const char *_option);
  char *getValue(char _optchar);
  bool getFlag(char _optchar);

  /*
   * Print Usage
   */
  void printUsage();
  void printAutoUsage();
  void addUsage(const char *line);
  void printHelp();
  /* print auto usage printing for unknown options or flag */
  void autoUsagePrint(bool flag);

  /*
   * get the argument count and arguments sans the options
   */
  int getArgc() const;
  char *getArgv(int index) const;
  bool hasOptions() const;

private:                /* the hidden data structure */
  int argc;             /* command line arg count  */
  char **argv;          /* commnd line args */
  const char *filename; /* the option file */
  char *appname;        /* the application name from argv[0] */

  int *new_argv;      /* arguments sans options (index to argv) */
  int new_argc;       /* argument count sans the options */
  unsigned int max_legal_args; /* ignore extra arguments */

  /* option strings storage + indexing */
  unsigned int max_options; /* maximum number of options */
  const char **options; /* storage */
  int *optiontype;      /* type - common, command, file */
  int *optionindex;     /* index into value storage */
  unsigned int option_counter;   /* counter for added options  */

  /* option chars storage + indexing */
  unsigned int max_char_options;  /* maximum number options */
  char *optionchars;    /*  storage */
  int *optchartype;     /* type - common, command, file */
  int *optcharindex;    /* index into value storage */
  int optchar_counter;  /* counter for added options  */

  /* values */
  char **values;       /* common value storage */
  int g_value_counter; /* globally updated value index LAME! */

  /* help and usage */
  const char **usage;  /* usage */
  unsigned int max_usage_lines; /* max usage lines reserved */
  unsigned int usage_lines;     /* number of usage lines */

  bool command_set;   /* if argc/argv were provided */
  bool file_set;      /* if a filename was provided */
  bool mem_allocated; /* if memory allocated in init() */
  bool posix_style;   /* enables to turn off POSIX style options */
  bool verbose;       /* silent|verbose */
  bool print_usage;   /* usage verbose */
  bool print_help;    /* help verbose */

  char opt_prefix_char;                             /*  '-' in "-w" */
  char long_opt_prefix[MAX_LONG_PREFIX_LENGTH + 1]; /* '--' in "--width" */
  char file_delimiter_char;                         /* ':' in width : 100 */
  char file_comment_char; /*  '#' in "#this is a comment" */
  char equalsign;
  char comment;
  char delimiter;
  char endofline;
  char whitespace;
  char nullterminate;

  bool set;  // was static member
  bool once; // was static member

  bool hasoptions;
  bool autousage;

private: /* the hidden utils */
  void init();
  void init(unsigned int maxopt, unsigned int maxcharopt);
  bool alloc();
  void allocValues(int index, size_t length);
  void cleanup();
  bool valueStoreOK();

  /* grow storage arrays as required */
  bool doubleOptStorage();
  bool doubleCharStorage();
  bool doubleUsageStorage();

  bool setValue(const char *option, char *value);
  bool setFlagOn(const char *option);
  bool setValue(char optchar, char *value);
  bool setFlagOn(char optchar);

  void addOption(const char *option, int type);
  void addOption(char optchar, int type);
  void addOptionError(const char *opt) const;
  void addOptionError(char opt) const;
  bool findFlag(char *value);
  void addUsageError(const char *line);
  bool CommandSet() const;
  bool FileSet() const;
  bool POSIX() const;

  char parsePOSIX(char *arg);
  int parseGNU(char *arg);
  bool matchChar(char c);
  int matchOpt(char *opt);

  /* dot file methods */
  char *readFile();
  char *readFile(const char *fname);
  bool consumeFile(char *buffer);
  void processLine(char *theline, int length);
  char *chomp(char *str);
  void valuePairs(char *type, char *value);
  void justValue(char *value);

  void printVerbose(const char *msg) const;
  void printVerbose(char *msg) const;
  void printVerbose(char ch) const;
  void printVerbose() const;
};

#endif /* ! _ANYOPTION_H */
