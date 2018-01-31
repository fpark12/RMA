#include <Windows.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <chrono>

std::vector<std::experimental::filesystem::path> video_files;
std::vector<std::experimental::filesystem::path> clean_video_files;

void generatesamples()
{
	std::wregex video_filter(LR"(.+\.mkv)");

	auto working_directory = std::experimental::filesystem::current_path();
	for (auto entry : std::experimental::filesystem::directory_iterator(working_directory))
	{
		if (std::regex_match(entry.path().filename().native(), video_filter, std::regex_constants::match_default))
		{
			video_files.push_back(std::move(entry));
		}
	}

	std::experimental::filesystem::create_directory("./samples");

	for (auto entry : video_files)
	{
		auto old_file_name = entry.filename().native();
		std::wstring sample_path = L"./samples/";
		sample_path += old_file_name;
		std::ofstream(sample_path.c_str());
	}
}

std::experimental::filesystem::path get_folder_name(const std::experimental::filesystem::path& name)
{
	auto old_file_name = name.filename();
	auto old_file_name_no_ext = old_file_name;
	old_file_name_no_ext.replace_extension("");
	auto desired_folder_name = old_file_name;

	// ([^\.]+) [0-9]+\.)
	std::wregex remove_ep_number_filter(LR"(([\s-]+(E)*(SP)*(NC)*(OP)*(ED)*([0-9]+)*\s*(v[0-9])*(END)*)$)");
	desired_folder_name = std::regex_replace(old_file_name_no_ext.native(), remove_ep_number_filter, L"");
	std::wcout << "Folder name:         " << desired_folder_name.native() << std::endl;

	std::wregex remove_ova_filter(LR"( [oO][vV][aA])");
	desired_folder_name = std::regex_replace(desired_folder_name.native(), remove_ova_filter, L"");
	std::wcout << "Folder name:         " << desired_folder_name.native() << std::endl;


	std::wregex trailing_space_filter(LR"(\s+\.)");
	desired_folder_name = std::regex_replace(desired_folder_name.native(), trailing_space_filter, L".");
	std::wcout << "Folder name:         " << desired_folder_name.native() << std::endl;

	return desired_folder_name;
}

void renamefiles(bool recursive = false)
{
	auto result = true;// test_unicode();
	if (!result)
	{
		std::wcout << "Couldn't enable unicode." << std::endl;
	}
	auto working_directory = std::experimental::filesystem::current_path();

	std::wcout << "Renaming anime files in : " << working_directory << std::endl << std::endl;

	/*
	{
	std::vector<std::experimental::filesystem::path> video_files;
	std::wregex video_filter(LR"(.+\.txt)");
	for (auto entry : std::experimental::filesystem::directory_iterator(working_directory))
	{
	if (std::regex_match(entry.path().filename().native(), video_filter, std::regex_constants::match_default))
	{
	auto old_file_name = entry.path();
	auto new_file_name = old_file_name;
	new_file_name.replace_extension(".mkv");

	std::wcout << "Replacing .txt in " << old_file_name.filename() << " to .mkv" << std::endl;

	std::experimental::filesystem::rename(old_file_name, new_file_name);
	}
	}
	}
	//*/

	std::wregex video_filter(LR"(.+\.mkv|.+\.mp4)");


	/*
	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(TEXT("./*.*"), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
	do
	{
	std::experimental::filesystem::path name = data.cFileName;
	if (std::regex_match(name.native(), video_filter, std::regex_constants::match_default))
	{
	video_files.push_back(std::move(name));
	}
	printf("%s\n", data.cFileName);
	}
	while (FindNextFile(hFind, &data));
	FindClose(hFind);
	}
	*/
	//*
	if (recursive)
	{
		for (auto entry : std::experimental::filesystem::recursive_directory_iterator(working_directory))
		{
			if (std::regex_match(entry.path().filename().native(), video_filter, std::regex_constants::match_default))
			{
				video_files.push_back(std::move(entry));
			}
		}
	}
	else
	{
		for (auto entry : std::experimental::filesystem::directory_iterator(working_directory))
		{
			if (std::regex_match(entry.path().filename().native(), video_filter, std::regex_constants::match_default))
			{
				video_files.push_back(std::move(entry));
			}
		}
	}
	//*/

	for (auto entry : video_files)
	{
		auto old_file_name = entry.filename();

		std::wcout << std::hex;
		for (wchar_t ch : old_file_name.native())
		{
			std::wcout << "0x" << static_cast<uint32_t>(ch) << ' ';
		}

		auto new_file_name = old_file_name;

		try
		{
			std::wcout << "Renaming: " << old_file_name.native() << std::endl;

			std::wregex underscore_filter(LR"(_)");
			new_file_name = std::regex_replace(new_file_name.native(), underscore_filter, L" ");
			std::wcout << "new_file_name  1:       " << new_file_name.native() << std::endl;

			std::wregex prefix_junk_filter(LR"(^(\s?[0-9]*\s?-?\s?))");
			new_file_name = std::regex_replace(new_file_name.native(), prefix_junk_filter, L"");
			std::wcout << "new_file_name  2:       " << new_file_name.native() << std::endl;

			std::wregex ep_filter(LR"([Ee][Pp])");
			new_file_name = std::regex_replace(new_file_name.native(), ep_filter, L"-");
			std::wcout << "new_file_name  3:       " << new_file_name.native() << std::endl;

			std::wregex sqaure_bracket_filter(LR"(\[[^\]]+\]\s?)");
			new_file_name = std::regex_replace(new_file_name.native(), sqaure_bracket_filter, L"");
			std::wcout << "new_file_name  4:       " << new_file_name.native() << std::endl;

			std::wregex bracket_filter(LR"(\([^\)]+\))");
			new_file_name = std::regex_replace(new_file_name.native(), bracket_filter, L"");
			std::wcout << "new_file_name  5:       " << new_file_name.native() << std::endl;

			std::wregex parentheses_filter(LR"(\{[^\}]+\})");
			new_file_name = std::regex_replace(new_file_name.native(), parentheses_filter, L"");
			std::wcout << "new_file_name  55:       " << new_file_name.native() << std::endl;

			std::wregex thora_filter(LR"(\s?-?\s?THORA\s?)");
			new_file_name = std::regex_replace(new_file_name.native(), thora_filter, L"");
			std::wcout << "new_file_name  6:       " << new_file_name.native() << std::endl;

			std::wregex resolution_filter(LR"(\s?720[pP]|\s?1080[pP])");
			new_file_name = std::regex_replace(new_file_name.native(), resolution_filter, L"");
			std::wcout << "new_file_name  7:       " << new_file_name.native() << std::endl;

			// post processing
			std::wregex add_ep_dash_filter(LR"(([^-])[\s]([0-9]+\.))");
			new_file_name = std::regex_replace(new_file_name.native(), add_ep_dash_filter, L"$1 - $2");
			std::wcout << "new_file_name  8:       " << new_file_name.native() << std::endl;

			std::wregex add_ep_space_filter(LR"(-([0-9]+\.))");
			new_file_name = std::regex_replace(new_file_name.native(), add_ep_space_filter, L"- $1");
			std::wcout << "new_file_name  9:       " << new_file_name.native() << std::endl;

			std::wregex add_ep_zero_filter(LR"(-\s([0-9]\.))");
			new_file_name = std::regex_replace(new_file_name.native(), add_ep_zero_filter, L"- 0$1");
			std::wcout << "new_file_name 10:       " << new_file_name.native() << std::endl;

			std::wregex double_name_filter(LR"(\s\s[^\.]+\.)");
			new_file_name = std::regex_replace(new_file_name.native(), double_name_filter, L".");
			std::wcout << "new_file_name  4:       " << new_file_name.native() << std::endl;

			std::wregex trailing_space_filter(LR"(\s+\.)");
			new_file_name = std::regex_replace(new_file_name.native(), trailing_space_filter, L".");
			std::wcout << "new_file_name 11:       " << new_file_name.native() << std::endl;

			std::wregex leading_space_filter(LR"(^\s+)");
			new_file_name = std::regex_replace(new_file_name.native(), leading_space_filter, L"");
			std::wcout << "new_file_name 12:       " << new_file_name.native() << std::endl;


			std::wcout << "To:       " << new_file_name.native() << std::endl;


			auto new_path = entry.parent_path();
			new_path /= new_file_name;
			bool r = MoveFile(entry.c_str(), new_path.native().c_str());
			clean_video_files.push_back(new_path);
			//std::experimental::filesystem::rename(old_file_name.native(), new_file_name.native());
		}
		catch (std::experimental::filesystem::filesystem_error e)
		{
			std::wcout << "Exception: " << e.what() << std::endl;
			std::wcout << "Code: " << e.code() << std::endl;
			std::wcout << "Arg1: " << e.path1().native() << std::endl;
			std::wcout << "Arg2: " << e.path2().native() << std::endl;

		}

	}
}

void reorganize()
{
	for (auto entry : clean_video_files)
	{
		auto desired_folder_name = get_folder_name(entry);
		std::experimental::filesystem::create_directory(desired_folder_name);

		auto new_file_name = desired_folder_name;
		new_file_name += "/";
		new_file_name += entry.filename();
		bool r = MoveFile(entry.native().c_str(), new_file_name.native().c_str());

		auto& parent_path = entry.parent_path();
		if (std::experimental::filesystem::is_empty(parent_path))
		{
			std::experimental::filesystem::remove(parent_path);
		}
	}
}

bool test_unicode()
{
	std::locale old;
	std::locale::global(std::locale("en_US.UTF-8"));

	std::regex pattern("[[:alpha:]]+", std::regex_constants::extended);
	bool result = std::regex_match(std::string("abcd\xC3\xA9""fg"), pattern);

	std::locale::global(old);

	return result;
}
int main(int argc, char* argv[])
{
	bool recursive = false;
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (!strcmp(argv[i], "-r"))
			{
				recursive = true;
			}
		}
	}
	//generatesamples();
	renamefiles(recursive);
	reorganize();
	std::wcout << "All done, press any key to exit..." << std::endl;
	getchar();
	return 0;
}