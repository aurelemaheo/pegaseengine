#ifndef CONST_HPP
#define CONST_HPP

#define COLOR_BLUE "1;34"
#define COLOR_GREEN "1;32"
#define COLOR_CYAN "1;36"
#define COLOR_YELLOW "1;33"
#define COLOR_RED "1;31"

#define COLOR(clevel,msg) "\033[" clevel "m" msg "\033[0m"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#endif //CONST_HPP
