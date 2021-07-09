
Example usage:
```cpp
#include "File.h"

#define MEASURE_TIME 1
#ifdef MEASURE_TIME
#include <chrono>
#include <string>
#endif

int main() {
	
	ESM::File file;

#ifdef MEASURE_TIME
	auto begin = std::chrono::steady_clock::now();
#endif

	file.parse("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim.esm");

#ifdef MEASURE_TIME	
	auto end = std::chrono::steady_clock::now();
	std::cout << "Loaded " << file.groups.size() << " groups in " 
			  << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0 << " s.\n";
#endif

}
```