#ifndef PROTOLOG_UTILITY_HPP
#define PROTOLOG_UTILITY_HPP

#include <string>
#include <fstream>
#include <filesystem>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
	#define POSIX_COMPLIANT
	#include <unistd.h>
	#include <limits.h>
	#include <sys/param.h>
#endif

#ifdef WIN32
	#include <windows.h>
	#include <processthreadsapi.h>
#endif

namespace Protolog
{
	const std::string RESET = "\033[0m";
	const std::string BLACK = "\033[30m";
	const std::string RED = "\033[31m";
	const std::string GREEN = "\033[32m";
	const std::string YELLOW = "\033[33m";
	const std::string BLUE = "\033[34m";
	const std::string MAGENTA = "\033[35m";
	const std::string CYAN = "\033[36m";
	const std::string WHITE = "\033[37m";
	const std::string CHARTREUSE = "\033[38;5;154m";
	const std::string PURPLE = "\033[38;5;201m";
	const std::string VELVET = "\033[38;5;126m";
	const std::string BOLDBLACK = "\033[1m\033[30m";
	const std::string BOLDRED = "\033[1m\033[31m";
	const std::string BOLDGREEN = "\033[1m\033[32m";
	const std::string BOLDYELLOW = "\033[1m\033[33m";
	const std::string BOLDBLUE = "\033[1m\033[34m";
	const std::string BOLDMAGENTA = "\033[1m\033[35m";
	const std::string BOLDCYAN = "\033[1m\033[36m";
	const std::string BOLDWHITE = "\033[1m\033[37m";

	pid_t get_process_id()
	{
		#ifdef POSIX_COMPLIANT
			return getpid();
		#endif
		#ifdef WIN32
			return GetCurrentProcessId();
		#endif
	}

	pid_t get_thread_id()
	{
		#ifdef POSIX_COMPLIANT
			return gettid();
		#endif
		#ifdef WIN32
			return GetCurrentThreadId();
		#endif
	}

	pid_t get_effective_user_id()
	{
		#ifdef POSIX_COMPLIANT
			return geteuid();
		#endif
		#ifdef WIN32
			return 1;
		#endif
	}

	std::string get_host_name()
	{
		#ifdef POSIX_COMPLIANT
  			char host[MAXHOSTNAMELEN];
  			gethostname(host, sizeof(host));
		#endif
		#ifdef WIN32
  			char host[MAX_COMPUTERNAME_LENGTH+1];
  			int ilen = sizeof (host);
  			GetComputerName (host, &ilen);
		#endif
		return host;
	}

	uint file_size(std::ofstream& fout)
    {
        return fout.tellp();
    }

	std::string path_cat(const std::filesystem::path& dir_path, const std::string& filename)
    {
		return dir_path.string() + filename;
    }

	template<typename ... Types>
	std::string formatter_string(const std::string& format, Types... args)
	{
	    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
	    size_t size = static_cast<size_t>(size_s);
	    char* buf = new char[size];
	    std::snprintf(buf, size, format.c_str(), args...);
	    return buf;
	}

	inline void reset_color(std::string& str)
	{
		str += RESET;
	}

	inline void set_color(std::string& str, const std::string& clr)
	{
		str += clr;
	}

	auto curr_time_ms()
	{
	    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	template<typename T, typename... Args>
	int64_t measure(const T& foo, const std::string& f_name, Args... args)
	{
	    auto before = curr_time_ms();
	    foo(args...);
	    auto after = curr_time_ms();
	    std::cout<<f_name<<": "<<after - before<<" ms";
		return after - before;
	}
}
#endif