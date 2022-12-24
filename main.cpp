// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "database.h"

using namespace std;

int main(int argc, char** argv) {
	ios_base::sync_with_stdio(false);
	cin >> std::boolalpha;
	cout << std::boolalpha;

	Database database;
	database.get_options(argc, argv);
	database.runDatabase();
	return 0;
}