#include<filesystem>
#include<fstream>
#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
using namespace std;
namespace fs = std::filesystem;

typedef vector<string> svec;
typedef pair<string, svec> s_vecpair;

s_vecpair get_top_dir_and_files_to_check(char* argv[], int argc);
int main(int argc,char* argv[])
{
	auto data = get_top_dir_and_files_to_check(argv, argc);

	int line_number = 0;
	for (auto& entry : fs::recursive_directory_iterator(data.first))
	{
		if (fs::is_regular_file(entry))
		{
			auto type = entry.path().extension().string();
			bool check = find(data.second.begin(), data.second.end(), type) != data.second.end();
			if (check)
			{
				ifstream file(entry.path().string());
				line_number+=count(istreambuf_iterator<char>(file),
								   istreambuf_iterator<char>(), '\n');
				line_number += 1;//eof
			}
		}
	}
	cout << "total line number = " << line_number << endl;
	
	return 0;
}
s_vecpair get_top_dir_and_files_to_check(char* argv[], int argc)
{
	if (argc < 3)
	{
		cout << "not enough arguments!";
		exit(-1);
	}
	string top_dir = argv[1];
	if (!fs::is_directory(fs::path{ top_dir }))
	{
		cout << top_dir << " is not a directory!";
		exit(-1);
	}

	svec file_types;
	for (int i = 2; i < argc; i++)
	{
		string type = argv[i];
		file_types.push_back(type);
	}
	return make_pair(top_dir, file_types);
}