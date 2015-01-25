#include <fstream>
#include <iostream>

#include "graph.hpp"

/* Display program help message */
void showHelp()
{
  std::cout << "Chromatic Number is a program which finds the smallest number of colors needed to color the vertices of a graph so that no two adjacent vertices share the same color.\n";
  std::cout << "Usage: ./ChromaticNumber target_filepath\n";
  std::cout << "Args:\n";
  std::cout << "target_filepath: Path to a file representing a graph (example files could be found under the graph/ directory." << std::endl;
}

/* Read a file
 * \arg filepath - where to find the file to read
 * \arg result   - [ouput] result from the read
 * \returns true if the file has been read correctly, false otherwise
 */
bool readFile( const std::string& filepath, std::string& result )
{
  std::ifstream file(filepath.c_str());
  if( !file.is_open() )
    return false;

  // Get length of file
  file.seekg(0, file.end);
  const int length = file.tellg();
  file.seekg(0, file.beg);

  // If file content is empty, exit
  if( length < 1 )
    return true;

  // Read the file
  result.resize( length );
  file.read(&result[0], length);

  // Close the file and return good read
  file.close();
  return true;
}

int main(int argc, char **argv)
{
  // Validate arguments
  if( argc < 2 )
  {
    showHelp();
    return EXIT_FAILURE;
  }

  // Read graph files
  std::string graph_representation;
  if( !readFile( argv[1], graph_representation ) )
  {
    std::cerr << "Can't read file: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // Create graph
  Graph graph = Graph::createGraph(graph_representation);

  // Display graph
  graph.describe();

  // Resolve graph
  graph.applyChromatic();

  // Display number of different colors
  std::cout << "This graph has " << graph.nbrUniqueColor() << " unique color(s)" << std::endl;

  return EXIT_SUCCESS;
}

