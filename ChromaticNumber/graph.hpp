#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <memory>
#include <string>
#include <vector>

/* Some forward declaration */
class Node;
class Graph;

/* Shorten version of node type */
typedef std::shared_ptr<Node> NodePtr;

/* Represents a node in a graph */
class Node : public std::enable_shared_from_this<Node>
{
  public:

    /* Constructor */
    Node(const std::string& name, unsigned int value = 0);

    /* Getter for name
     * \returns name of this node
     */
    const std::string& name() const { return m_name; }

    /* Getter for value
     * \returns value of this node
     */
    unsigned int value() const { return m_value; }

    /* Does this node have a value
     * \return true if this node has en internal value different from 0
     */
    bool hasValue() const { return m_value != 0; }

    /* Is a given node a neighbour of this one ?
     * \arg node - to verify
     * \returns true if given node is a direct neighbour of this one, false otherwise
     */
    bool isNeighbour(NodePtr node) const;

    /* Get neighbours of this node
     * \retuns neighbours of this node
     */
    const std::vector<NodePtr>& neighbours() const { return m_neighbours; }

  private:

    // In order to access private methods to construct/edit a node
    friend class Graph;

    /* Apply chromatic algotirhm on this node and recusivly on neighbours */
    void applyChromatic();

    /* Add a neighbour node to this one
     * \arg node - to add
     * \returns true if the node has been added, false otherwise (if already added)
     */
    bool addNeighbour(NodePtr node);

    /* Find the minimum available value for this node depending on values of neighbours */
    unsigned int minimumValue() const;

  private:

    /* Name of this node */
    std::string m_name;

    /* Could represents anything */
    /* In this case, value actually represents a color */
    unsigned int m_value {0};

    /* Adjacent nodes to this one */
    std::vector<NodePtr> m_neighbours;
};

/* Represent a graph */
class Graph
{
  public:

    /* Apply chromatic algorithm */
    void applyChromatic();

    /* Get number of unique color in graph
     * \returns number of unique color in graph
     */
    size_t nbrUniqueColor() const;

    /* Ouput a representation of the graph nodes and neighbours */
    void describe() const;

    /* Create a graph from a string
     * \arg input - to read from
     * \returns a Graph object
     */
    static Graph createGraph(const std::string& input);

  private:

    /* Find a node by its name
     * \arg name - of the node to find
     * \returns the found node or a invalid shared ptr
     */
    NodePtr findNode(const std::string& name);

    /* Get a node from existing ones in that graph
     * If no node matching the inut name can be found create a new one
     * \arg name - of the name to find/create
     * \returns the found or newly created node
     */
    NodePtr getNodeOrCreateIt(const std::string& name);

  private:

    /* Nodes in this graphs */
    std::vector<NodePtr> m_nodes;
};

#endif // GRAPH_HPP
