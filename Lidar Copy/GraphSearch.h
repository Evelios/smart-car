#ifndef GraphSearch_h
#define GraphSearch_h

#include "List.h"
#include "PriorityList.h"

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * *
 * Helper Functions
 * * * * * * * * * * * * * * * * * * * * * */

/*
 * Helper function for deleting lists containing pointers
 */
template<typename T>
void deleter(List<T> list)
{
  while (list.begin() != list.end()) {
    delete list.back();
    list.pop_back();
  }
}

/*
 * Helper function to find if a list contains an element
 */
template<typename T>
bool listContains(List<T> list, T ele) {
  auto itr = list.begin();
  while (itr != list.end()) {
    if ((*itr) == ele) {
      return true;
    }
    itr++;
  }
  return false;
}

template<typename T>
bool pointerListContains(List<T> list, T ele) {
  auto itr = list.begin();
  while (itr != list.end()) {
    if (*(*itr) == *ele) {
      return true;
    }
    itr++;
  }
  return false;
}

/* * * * * * * * * * * * * * * * * * * * * *
 * Graph Search Structures
 * * * * * * * * * * * * * * * * * * * * * */


/* The abstract class for a formal problem.  You should subclass
 * this class and implement the methods actions and result, and possibly
 * __init__, goal_test, and path_cost. Then you will create instances
 * of your subclass and solve them with the various search functions.
 */

template<typename S, typename A>
class Problem {

public:
  /*
   * The constructor specifies the initial state, and possibly a goal
   * state, if there is a unique goal.  Your subclass's constructor can add
   * other arguments.
   */
  Problem(S initial, S goal) {
    this->initial = initial;
    this->goal = goal;
  }

  /*
   * Return the actions that can be executed in the given
   * state. The result would typically be a list, but if there are
   * many actions, consider yielding them one at a time in an
   * iterator, rather than building them all at once.
   */
  virtual List<S> actions(const S& state) const =0;

  /* 
   * Return the state that results from executing the given
   * action in the given state. The action must be one of
   * self.actions(state).
   */
  virtual S result(const S& state, const A& action) const =0;

  /*
   * Return the heuristic cost of reaching the goal state from the
   * current state with action. The heuristic could be a relaxed version
   * of the problem to give the algorithm a guiding direction towards
   * the goal node through the search space.
   */
  float heuristic(const S& state, const A& action) const{
    return 0;
  }

  /*
   * Return True if the state is a goal. The default method compares the
   * state to self.goal, as specified in the constructor. Override this
   * method if checking against a single self.goal is not enough.
   */
  bool goalTest(const S& state) const {
    return this->goal == state;
  }

  /*
   * Return the cost of a solution path that arrives at state2 from
   * state1 via action action, assuming cost c to get up to state1. If the problem
   * is such that the path doesn't matter, this function will only look at
   * state2.  If the path does matter, it will consider c and maybe state1
   * and action. The default method costs 1 for every step in the path.
   */
  float pathCost(float c, const S& state1, const A& action, const S& state2) const {
    return c + 1;
  }

  S initial;
  S goal;
};

template<typename S, typename A>
class Node {
  typedef Problem<S, A> P;
  typedef Node<S, A> N;
/* A node in a search tree. Contains a pointer to the parent (the node
 * that this is a successor of) and to the actual state for this node. Note
 * that if a state is arrived at by two paths, then there are two nodes with
 * the same state.  Also includes the action that got us to this state, and
 * the total path_cost (also known as g) to reach the node.  Other functions
 * may add an f and h value; see best_first_graph_search and astar_search for
 * an explanation of how the f and h values are handled. You will not need to
 * subclass this class.
 */

public:
	Node(const S& state) {
		this->state = state;
    this->depth = 0;
    this->parent = NULL;
	}
	Node(const S& state, N* parent, const A& action, float pathCost) {
		this->state = state;
		this->parent = parent;
		this->action = action;
		this->pathCost = pathCost;
		this->depth = this->parent->depth + 1;
	}
  Node(const Node& other) {
    this->state = other.state;
    this->parent = other.parent;
    this->action = other.action;
    this->pathCost = other.pathCost;
    this->depth = other.depth;
  }
  /*
   * Returns all the children nodes that are reachable from the current node.
   * All nodes are reachable by actions in accordance to the input problem.
   */
	List<N*> expand(const P& problem) {
		List<A> actions = problem.actions(this->state);  
		List<N*> children = List<N*>();

    auto act_itr = actions.begin();
    while(act_itr != actions.end()) {
      children.push_back(this->childNode(problem, *act_itr));
      act_itr++;
    }

    return children;
	}

  /*
   * Returns the child node reachable from the current node when the action
   * is taken.
   */
	Node* childNode(const P& problem, const A action) {
		S next = problem.result(this->state, action);
		float cost = problem.pathCost(this->pathCost, this->state, action, next);

		return new N(next, this, action, cost);
	}

  /*
   * Returns the list of nodes which is the path back from the current
   * node to the initial start node.
   */
	List<A> solution() {
    N* node = this;
    List<A> actions = List<A>();
    
    while (node) {
      actions.push_back(node->action);
      node = node->parent;
    }

    // Reverse list and remove the last element
    actions.pop_back();
    List<A> solution = List<A>();
    
    auto act_itr = actions.rend();
    while(act_itr != actions.rbegin()) {
      solution.push_back(*act_itr);
      act_itr--;
    }

    return solution;
	}

  String repr() const {
    String b = "<Node State";
    String e = ">";
    return b + state.repr() + e;
  }

	bool operator==(const N& other) const {
    return this->state == other.state;
  }
	
	S state;
	A action;
  N* parent;
	float pathCost;
	int depth;
  bool root;
	
	
};

/* * * * * * * * * * * * * * * * * * * * * *
 * Graph Search Algorithms
 * * * * * * * * * * * * * * * * * * * * * */

template<typename S, typename A>
List<A> breadthFirstSearch(const Problem<S, A>& problem) {
  Node<S, A>* node = new Node<S, A>(problem.initial);
  if (problem.goalTest(node->state)) {
    // Cleanup
    delete node;
    node = NULL;
    
    return List<A>();
  }
  
  List<Node<S, A>*> frontier = List<Node<S, A>*>();
  frontier.push_back(node);
  List<Node<S, A>*> explored = List<Node<S, A>*>();

  while (frontier.begin() != frontier.end()) {
    node = frontier.front();
    frontier.pop_front();
    explored.push_back(node);
    List<Node<S, A>*> expandedNodes = node->expand(problem);

    while (expandedNodes.size() > 0) {
      Node<S, A>* child = expandedNodes.front();
      expandedNodes.pop_front();
      
      if (problem.goalTest(child->state)) {
        List<A> solution = child->solution();
        
        // Cleanup
        delete child;
        deleter(expandedNodes);
        deleter(frontier);
        deleter(explored);

        child = NULL;
        node = NULL;
        
        return solution;
      }

      if (!pointerListContains(frontier, child) && !pointerListContains(explored, child)) {        
        frontier.push_back(child);
      } else {
        delete child;
        child = NULL;
      }
    }
  }

  // Cleanup
  deleter(frontier);
  deleter(explored);
  return List<A>();
}

template<typename S, typename A>
List<A> astar_search(const Problem<S, A>& problem) {
  Node<S, A>* node = new Node<S, A>(problem.initial);
  if (problem.goalTest(node->state)) {
    // Cleanup
    delete node;
    return List<A>();
  }
  
  List<Node<S, A>*> frontier = PriorityList<Node<S, A>*>();
  frontier.push_back(node);
  List<Node<S, A>*> explored = List<Node<S, A>*>();

  while (frontier.begin() != frontier.end()) {
    node = frontier.front();
    frontier.pop_front();
    explored.push_back(node);
    List<Node<S, A>*> expandedNodes = node->expand(problem);

    auto expan_itr = expandedNodes.begin();
    while (expan_itr != expandedNodes.end()) {
      Node<S, A>* child = *expan_itr;
      expan_itr++;
      
      if (problem.goalTest(child->state)) {
        List<A> solution = child->solution();
        // Cleanup        
        deleter(frontier);
        deleter(explored);
        
        return solution;
      }

      if (!pointerListContains(frontier, child) && !pointerListContains(explored, child)) {        
        frontier.push_back(child);
      }
    }
  }

  // Cleanup
  deleter(frontier);
  deleter(explored);
  return List<A>();
}

template <typename T>
void printNodes(List<T> list) {
  Serial.print("[ ");
  auto itr = list.begin();
  while (itr != list.end()) {
    Serial.print((*itr)->repr());
    Serial.print(", ");
    itr++;
  }
  Serial.println(" ]");
}

template <typename T>
void printState(List<T> list) {
  Serial.print("[ ");
  auto itr = list.begin();
  while (itr != list.end()) {
    Serial.print((*itr).repr());
    Serial.print(", ");
    itr++;
  }
  Serial.println(" ]");
}

#endif
