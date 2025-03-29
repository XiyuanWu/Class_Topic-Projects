import heapq

class Node:
    def __init__(self, state, g_cost=0, h_cost=0, parent=None):
        self.state = state  # Current puzzle state (2D list)
        self.g_cost = g_cost  # g(n): Path cost
        self.h_cost = h_cost  # h(n): Heuristic cost
        self.f_cost = g_cost + h_cost  # f(n) = g(n) + h(n)
        self.parent = parent  # Parent node for path reconstruction

    def __lt__(self, other):
        # For priority queue comparison (based on f_cost)
        return self.f_cost < other.f_cost

    def get_path(self):
        """
        Reconstructs the path from the initial state to the current node.
        """
        path = []
        current = self
        while current:
            path.append(current.state)
            current = current.parent
        return path[::-1]  # Reverse to get path from start to goal


def a_star_misplaced(initial_state, goal_state):
    """
    Implements A* search using the Misplaced Tile heuristic.

    Args:
        initial_state: The starting configuration of the puzzle (2D list).
        goal_state: The target configuration of the puzzle (2D list).

    Returns:
        solution_path: List of puzzle states from start to goal.
        metrics: Dictionary containing metrics like nodes expanded and max depth.
    """
    # Priority queue (min-heap) for A*
    frontier = []
    initial_h_cost = misplaced_tile_heuristic(initial_state, goal_state)
    heapq.heappush(frontier, Node(initial_state, 0, initial_h_cost))

    # Explored set to track visited states
    explored = set()

    # Metrics
    metrics = {
        "nodes_expanded": 0,
        "max_depth": 0,
    }

    while frontier:
        # Pop the node with the lowest f(n)
        current_node = heapq.heappop(frontier)
        current_state = current_node.state

        # Check if goal is reached
        if current_state == goal_state:
            metrics["max_depth"] = current_node.g_cost
            return current_node.get_path(), metrics

        # Mark the current state as explored
        explored.add(tuple(map(tuple, current_state)))
        metrics["nodes_expanded"] += 1

        # Generate neighbors
        for next_state, move_cost in generate_neighbors(current_state):
            new_g_cost = current_node.g_cost + move_cost
            new_h_cost = misplaced_tile_heuristic(next_state, goal_state)
            if tuple(map(tuple, next_state)) not in explored:
                heapq.heappush(frontier, Node(next_state, new_g_cost, new_h_cost, current_node))

    return None, metrics  # No solution found


def misplaced_tile_heuristic(state, goal):
    """
    Calculates the Misplaced Tile heuristic (h(n)).

    Args:
        state: The current puzzle state (2D list).
        goal: The target puzzle state (2D list).

    Returns:
        h_cost: Number of tiles that are not in their correct position.
    """
    h_cost = 0
    for i in range(len(state)):
        for j in range(len(state[i])):
            if state[i][j] != goal[i][j] and state[i][j] != 0:  # Ignore blank space (0)
                h_cost += 1
    return h_cost


def generate_neighbors(state):
    """
    Generates all valid moves (neighbors) for the blank space in the 8-puzzle.

    Args:
        state: The current puzzle state (2D list).

    Returns:
        neighbors: List of tuples (next_state, move_cost).
    """
    neighbors = []
    size = len(state)
    # Find the position of the blank space (0)
    for i in range(size):
        for j in range(size):
            if state[i][j] == 0:
                blank_pos = (i, j)

    moves = [
        (0, 1),  # Move right
        (0, -1), # Move left
        (1, 0),  # Move down
        (-1, 0)  # Move up
    ]

    for dx, dy in moves:
        new_x, new_y = blank_pos[0] + dx, blank_pos[1] + dy
        if 0 <= new_x < size and 0 <= new_y < size:  # Check bounds
            # Create a new state with the blank space moved
            new_state = [row[:] for row in state]  # Deep copy
            new_state[blank_pos[0]][blank_pos[1]], new_state[new_x][new_y] = (
                new_state[new_x][new_y],
                new_state[blank_pos[0]][blank_pos[1]],
            )
            neighbors.append((new_state, 1))  # Move cost is always 1

    return neighbors
