#ifndef AWLOGGER
#define AWLOGGER

#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

namespace AWUtil
{
    class Logger
    {
    public:
        Logger(const std::string &logFileName) : mLogFileName(logFileName) {}


        void reportStringStream(std::basic_ostream<char>& ss)
        {
            std::ofstream log_file(mLogFileName,
                                   std::ios_base::out | std::ios_base::app );

            log_file << std::endl << getTimeStamp().rdbuf() << ": " << ss.rdbuf();
        }

        void operator<<(const char *str)
        {
            std::ofstream log_file(mLogFileName,
                                   std::ios_base::out | std::ios_base::app );

            log_file << std::endl << getTimeStamp().rdbuf() << ": " << str;
        }
    protected:
        static std::stringstream getTimeStamp()
        {
            std::time_t currentTime;
            struct std::tm *localTime;

            std::time( &currentTime );                   // Get the current time
            localTime = std::localtime( &currentTime );  // Convert the current time to the local time

            //int Day    = localTime->tm_mday;
            //int Month  = localTime->tm_mon + 1;
            //int Year   = localTime->tm_year + 1900;
            int Hour   = localTime->tm_hour;
            int Min    = localTime->tm_min;
            int Sec    = localTime->tm_sec;

            std::stringstream stringout;
            stringout << ((Hour<10) ? "0" : "") << Hour << ":" <<
                         ((Min<10) ? "0" : "") << Min << ":" <<
                         ((Sec<10) ? "0" : "") << Sec;


            return stringout;

        }

    private:
        std::string mLogFileName;
    };
}

#endif // AWLOGGER

