#pragma once
/*
there are two different ways to inform an error 
1.new the errirMessenger nested class and inform it to show disered message
2.call the Log function directly
The difference is that any error catched by errorMgr will make an additional windows message box 
*/

//---------------------------------------------------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------------------------------------------------

// display flags
const unsigned char LOGFLAG_WRITE_TO_LOG_FILE =		1 << 0;
const unsigned char LOGFLAG_WRITE_TO_DEBUGGER =		1 << 1;

namespace EngLogger
{
	// This class is used by the debug macros and shouldn't be accessed externally.
	class ErrorMessenger
	{
		bool m_enabled;

	public:
		ErrorMessenger(void);
		void Show(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);
	};

	// construction; must be called at the beginning and end of the program
	void Init(const char* loggingConfigFilename);
	void Destroy(void);
	
	// logging functions
	void Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);
}


//---------------------------------------------------------------------------------------------------------------------
// Debug macros
//---------------------------------------------------------------------------------------------------------------------

// Fatal Errors are fatal and are always presented to the user.
#define ENG_FATAL(str) \
	do \
	{ \
		static EngLogger::ErrorMessenger* pErrorMessenger = ENG_NEW EngLogger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\

#if 1 //ndef NDEBUG

// Errors are bad and potentially fatal.  They are presented as a dialog with Abort, Retry, and Ignore.  Abort will
// break into the debugger, retry will continue the game, and ignore will continue the game and ignore every subsequent 
// call to this specific error.  They are ignored completely in release mode.
#define ENG_ERROR(str) \
	do \
	{ \
		static EngLogger::ErrorMessenger* pErrorMessenger = ENG_NEW EngLogger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\

// Warnings are recoverable.  They are just logs with the "WARNING" tag that displays calling information.  The flags
// are initially set to WARNINGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden normally.
#define ENG_WARNING(str) \
	do \
	{ \
		std::string s((str)); \
		EngLogger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__); \
	}\
	while (0)\

// This is just a convenient macro for logging if you don't feel like dealing with tags.  It calls Log() with a tag
// of "INFO".  The flags are initially set to LOGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden 
// normally.
#define ENG_INFO(str) \
	do \
	{ \
		std::string s((str)); \
		EngLogger::Log("INFO", s, NULL, NULL, 0); \
	} \
	while (0) \

// This macro is used for logging and should be the preferred method of "printf debugging".  You can use any tag 
// string you want, just make sure to enabled the ones you want somewhere in your initialization.
#define ENG_LOG(tag, str) \
	do \
	{ \
		std::string s((str)); \
		EngLogger::Log(tag, s, NULL, NULL, 0); \
	} \
	while (0) \

// This macro replaces ENG_ASSERT().
#define ENG_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
			static EngLogger::ErrorMessenger* pErrorMessenger = ENG_NEW EngLogger::ErrorMessenger; \
			pErrorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	} \
	while (0) \

#else  // NDEBUG is defined

// These are the release mode definitions for the macros above.  They are all defined in such a way as to be 
// ignored completely by the compiler.
#define ENG_ERROR(str) do { (void)sizeof(str); } while(0) 
#define ENG_WARNING(str) do { (void)sizeof(str); } while(0) 
#define ENG_INFO(str) do { (void)sizeof(str); } while(0) 
#define ENG_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define ENG_ASSERT(expr) do { (void)sizeof(expr); } while(0) 

#endif  // !defined NDEBUG
