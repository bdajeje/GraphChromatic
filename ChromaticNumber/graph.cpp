#include "graph.hpp"

#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <set>

Node::Node(const std::string &name, unsigned int value)
  : m_name(name)
  , m_value(value)
{}

bool Node::addNeighbour(NodePtr node)
{
  // Check not already neightbour
  if(isNeighbour(node))
    return false;

  // Add the node
  m_neighbours.push_back(node);

  return true;
}

bool Node::isNeighbour(NodePtr node) const
{
  return std::find_if(m_neighbours.begin(), m_neighbours.end(), [&node] (const NodePtr& node_comp) {
    return node->name() == node_comp->name();
  }) != m_neighbours.end();
}

void Node::applyChromatic()
{
  // Apply minimal value to this node if needed
  if( !hasValue() )
  {
    auto value = minimumValue();
    m_value = value;
  }

  // Get through neighbours
  for( auto neighbour : m_neighbours )
  {
    if( !neighbour->hasValue() )
      neighbour->applyChromatic();
  }
}

unsigned int Node::minimumValue() const
{
  std::vector<unsigned int> close_values;
  close_values.reserve(m_neighbours.size());

  for(const auto& neighbour : m_neighbours)
    close_values.push_back(neighbour->value());

  std::sort(close_values.begin(), close_values.end());

  unsigned int start = 0;
  for( auto value : close_values )
  {
    if(value > start)
    {
      if( value == start + 1 )
        start++;
      else
        break;
    }
  }

  return start + 1;
}

void Graph::applyChromatic()
{
  if(m_nodes.empty())
    return;

  // Let's start from any point
  m_nodes.front()->applyChromatic();
}

NodePtr Graph::findNode(const std::string& name)
{
  std::vector<NodePtr>::iterator node = std::find_if(m_nodes.begin(), m_nodes.end(), [&name] (const NodePtr &node) {
    return name == node->name();
  });

  return node != m_nodes.end() ? *node : nullptr;
}

NodePtr Graph::getNodeOrCreateIt(const std::string& name)
{
  // Returns already existing node if any
  auto node = findNode(name);
  if(node)
    return node;

  // Create a new node
  auto new_node = std::make_shared<Node>(name);
  m_nodes.push_back(new_node);

  return new_node;
}

Graph Graph::createGraph(const std::string& input)
{
  Graph graph;

  // Let's first split line by line (as each line represent a node with its neighbours)
  std::vector<std::string> lines;
  boost::split(lines, input, boost::is_any_of("\n") );

  // Now go through each line and create the nodes!
  for( const auto& line : lines )
  {
    // Extract node name
    auto separator = line.find(':');
    if(separator == std::string::npos)
      continue;

    // Create node with its name
    auto node = graph.getNodeOrCreateIt( line.substr(0, separator) );

    // Now add neighbour nodes to this node
    std::string neigbours_part {line.substr(separator+1)};
    std::vector<std::string> neighbour_names;
    boost::split( neighbour_names, neigbours_part, boost::is_any_of(",") );
    for( const auto& neighbour_name : neighbour_names )
      node->addNeighbour( graph.getNodeOrCreateIt( neighbour_name ) );
  }

  return graph;
}

void Graph::describe() const
{
  for( const auto& node : m_nodes )
  {
    std::cout << node->name() + ": ";
    for( const auto& neighbour : node->neighbours() )
      std::cout << neighbour->name() + ", ";
    std::cout << '\n';
  }

  std::cout << std::endl;
}

size_t Graph::nbrUniqueColor() const
{
  std::set<unsigned int> unique_values;
  for( const auto& node : m_nodes )
    unique_values.insert( node->value() );

  return unique_values.size();
}
