#pragma once
#include<string>

#define FILE_READ
#define FILE_FIND
#define FILE_WRITE

enum class ErroRType
{
	FileFinding,
	FileLoading,
	FileWriting
};

struct Error
{
	std::string _error_file;
	int _error_line;
	std::string _error_message;
};