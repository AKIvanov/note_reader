#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <tuple>

void checkIfSuccessfulLoad(std::ifstream& stream)
{
    if (stream.fail())
    {
        throw std::runtime_error("Failed to read expected data from stream.");
    }
    else
    {
        std::cout << "File successfully loaded!" << std::endl;
    }
}

std::string toLower(const std::string& str)
{   
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    return lowerStr;
}

std::vector<int> commonLines(std::vector<int>& vec1, std::vector<int>& vec2)
{
    // Common indices
    std::vector<int> commonInd;

    // Sort both of the vectotrs
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    // Get iterators for both the vectors
    auto it1 = vec1.begin();
    auto it2 = vec2.begin();

    while( it1 != vec1.end() && it2 != vec2.end() )
    {
        if ( *it1 == *it2 )
        {
            commonInd.push_back( *it1 );
            ++it1;
            ++it2;
        }
        else if ( *it1 < *it2 ){
            ++it1;
        } else {
            ++it2;
        }
    }

    return commonInd;
}

std::vector<int> uniqueLines(const std::vector<int>& vec1, const std::vector<int>& vec2)
{
    // Concatenated vector
    std::vector<int> concat_vec = vec1;

    // Concatenate the two vectors together 
    concat_vec.insert(concat_vec.end(), vec2.begin(), vec2.end());

    // Sort the indices 
    std::sort(concat_vec.begin(), concat_vec.end());

    // Remove consecutive (adjacent) duplicates
    auto last = std::unique(concat_vec.begin(), concat_vec.end());

    // Shrink the vec1 to contain only the unique (adjacent) elements
    concat_vec.erase(last, concat_vec.end());

    return concat_vec;
}

void printLineNumberAndLine(const std::vector<std::string>& lines, const std::vector<int>& line_numbers)
{
    int count = 0;
    for(const auto line : line_numbers)
    {
        // Line starts at 1 not at 0
        int ind = line - 1;

        // print out the line number followed by the line
        std::cout << "Line Number: " << line_numbers[count] << ", Line: " << lines[ind] << std::endl;

        count++;
    }
}

std::tuple<std::string, int> findTag(const std::string& line, const int start_ind)
{
    // Find the first instance of # starting at tag_ind 
    auto tag_ind1   = line.find('#', start_ind);

    // Use that tag_ind1 to find the next # after tag_ind1
    auto tag_ind2 = line.find('#', tag_ind1 + 1); 

    // Determine count from tag_ind1 
    auto count = tag_ind2 - (tag_ind1 + 1);

    // Retrieve the first tag after the #
    auto key = line.substr(tag_ind1 + 1, count);

    // Make the key lowercase
    auto keyLower = toLower(key);

    // Remove any characters from the of the std::string
    keyLower.erase(std::remove_if( keyLower.begin(), keyLower.end(), [](char c) 
    { return std::ispunct(c); }), keyLower.end() );

    // Remove whitespace from the key if exists
    keyLower.erase( remove_if(keyLower.begin(), keyLower.end(), isspace), keyLower.end() );

    // Create tuple of key and ind (character before next #) 
    return std::make_tuple(keyLower, tag_ind2 - 1);
}

void populateUnorderedMap(const int line_num, const std::string& line, std::unordered_map<std::string, std::vector<int>>& tag_map)
{
    std::size_t numCharactersInLine = line.length();
    int tag_ind = 0;

    // Loop through the string looking for # until npos is returned
    while(line.find('#', tag_ind) != std::string::npos)
    {   
        // Find the next tag and corresponding index (last character of key)
        auto [key, ind] = findTag(line, tag_ind);
        
        // Create a key in the unordered map
        tag_map[key].push_back(line_num);

        // Update the tag_ind 
        tag_ind = ind; 
    }
        
}

std::vector<std::string> processCliArguments(std::unordered_map<std::string, std::vector<int>>& tag_map, 
                                             std::vector<std::string>& vector_of_lines, 
                                             std::string& cliArgument)
{

    std::string lowerCliArg = toLower(cliArgument);
    std::vector<std::string> cliStrings = { "and", "or" };
    std::vector<std::string> parsedCliArgs; 

    // Determine whether AND or OR or NONE was specified
    for (auto const& str : cliStrings)
    {
        // Find the AND or OR 
        if (lowerCliArg.find( str ) != std::string::npos)
        {
            auto cli_ind = lowerCliArg.find( str );

            // Find the space after the AND or OR 
            auto space_ind = lowerCliArg.find(' ', cli_ind);

            // Determine count from start_ind 
            auto count = space_ind - (cli_ind);

            // Return AND or OR substr
            auto cli_arg = lowerCliArg.substr(cli_ind, count);

            // Check whether AND or OR or NONE was found, compare returns 0 if true
            if (cli_arg.compare(str) == 0)
            {
                parsedCliArgs.push_back( str );
            }

        } else {

            continue;
        }
        
    }

    // Check for the case where no AND or OR was given 
    if( parsedCliArgs.empty() )
    {
        throw std::invalid_argument("AND or OR must be specified in Command Line Argument");
    }

    // Loop over the tags in unordered_map
    for (auto const& [key, val_vec] : tag_map)
    {
        if (lowerCliArg.find( key ) != std::string::npos)
        {
            auto cli_ind = lowerCliArg.find( key );

            // Find the space after the AND or OR 
            auto space_ind = lowerCliArg.find(' ', cli_ind);

            // Determine count from start_ind 
            auto count = space_ind - (cli_ind);

            // Get valid tag
            auto cli_arg = lowerCliArg.substr(cli_ind, count);

            // Parse the lowerCliArg to determine which tags are being requested
            parsedCliArgs.push_back( cli_arg );

        } else {
            
            continue;

        }

    }

    return parsedCliArgs;

}

void printLinesFromText(std::vector<std::string>& parsedCliLines, 
                        std::vector<std::string>& vector_of_lines,
                        std::unordered_map<std::string, std::vector<int>>& tag_map)
{
    std::vector<std::string> cliStrings = { "and", "or" };

    // Unpack the keyword
    std::string keyWord = parsedCliLines[0];

    // Unpack the tags 
    std::string tag1 = parsedCliLines[1];
    std::string tag2 = parsedCliLines[2];

    // Get the indices that correspond to the different tags
    std::vector<int> vecInd1 = tag_map[tag1];
    std::vector<int> vecInd2 = tag_map[tag2];

    // Create a vector of the Results 
    std::vector<int> vecResults;

    // Check if the condition is an "and", 0 means true
    if (cliStrings[0].compare( keyWord ) == 0)
    {
        // Find the common indices
        vecResults = commonLines(vecInd1, vecInd2);
    
    // Check if the solution is "or"
    } else if (cliStrings[1].compare( keyWord ) == 0) 
    {
        // Return the unique lines where both results appear
        vecResults = uniqueLines(vecInd1, vecInd2);

    }

    // print out the lines where the tags appear and the lines themselves
    printLineNumberAndLine(vector_of_lines, vecResults);

}

int main(){

    // Read in the text file 
    std::ifstream file("notes.txt");

    // Check if file was successfully loaded
    checkIfSuccessfulLoad(file);
    
    // Create std::string for line that needs to be parsed
    std::string line;

    // Store all of the lines in a vector so that it can be print out later
    std::vector<std::string> vector_of_lines;

    // Create unordered_map 
    std::unordered_map<std::string, std::vector<int>> tag_map;

    // Print C++ verison being used 
    std::cout << __cplusplus << std::endl;

    // Go through the lines in the text file and assign to string
    while( std::getline(file , line))
    {
        // Stores std::string in std::vector for later use 
        vector_of_lines.push_back(line);

        // Call function that populates the unordered_map
        populateUnorderedMap(vector_of_lines.size(), line, tag_map);

    }

    // Process user queries, can be single tag, two tags using AND, or One tag or the Other using OR 
    std::string cliString;

    // Print out statement prompting user 
    std::cout << "Provide tags to look for in notes.txt ie cpp AND stl" << std::endl;

    // Receive full string from the CLI
    std::getline(std::cin, cliString);

    // Return parsed cliArgs
    std::vector<std::string> parseCliArgs = processCliArguments(tag_map, vector_of_lines, cliString);

    // Depending on the AND or OR return the specified lines 
    printLinesFromText(parseCliArgs, vector_of_lines, tag_map);
}