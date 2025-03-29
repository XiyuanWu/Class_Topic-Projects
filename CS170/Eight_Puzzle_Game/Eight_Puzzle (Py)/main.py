import time
from ucs import uniform_cost_search
from heuristic import a_star_misplaced
from manhattan import a_star_manhattan

def get_user_input():
    """
    Prompts the user to input the initial state for the 8-puzzle.
    Returns the input as a 2D list.
    """
    print("Enter the initial state of the puzzle row by row, separated by spaces:")
    initial_state = []
    for i in range(3):  # 3x3 puzzle
        row = list(map(int, input(f"Row {i + 1}: ").strip().split()))
        initial_state.append(row)
    return initial_state

def print_solution(algorithm_name, solution, metrics):
    """
    Prints the solution path and metrics for a given algorithm.
    """
    print(f"\n{algorithm_name} Results:")
    print("Solution Path:")
    for step in solution:
        for row in step:
            print(row)
        print("----")
    print(f"Total Steps: {len(solution) - 1}")
    print(f"Nodes Expanded: {metrics['nodes_expanded']}")
    print(f"Maximum Depth: {metrics['max_depth']}")
    print(f"Runtime: {metrics['runtime']:.4f} seconds")

def main():
    print("Welcome to the 8-Puzzle Solver!")
    print("Do you want to use the default puzzle or enter a custom puzzle?")
    print("1. Default puzzle \n2. Custom puzzle")
    option = int(input("Enter your choice (1/2): ").strip())
    
    if option == 2:
        # Get initial state from user
        initial_state = get_user_input()
    else:
        # initial_state = [[1, 2, 3],
        #                  [5, 0, 6],
        #                  [4, 7, 8]]  # Default initial state
        initial_state = [[0, 7, 2],
                         [4, 6, 1],
                         [3, 5, 8]]  # Default initial state


    # Define goal state
    goal_state = [[1, 2, 3],
                  [4, 5, 6],
                  [7, 8, 0]]  # Standard 8-puzzle goal

    # Test case option
    # test_cases = [
    #     [[1, 2, 3], [4, 5, 6], [7, 8, 0]],  # Already solved
    #     [[1, 2, 3], [4, 5, 6], [7, 0, 8]],  # Simple move
    #     [[1, 2, 3], [5, 0, 6], [4, 7, 8]]   # More complex
    # ]

    # print("\nTest Cases:")
    # for i, test in enumerate(test_cases, 1):
    #     print(f"Test Case {i}:")
    #     for row in test:
    #         print(row)
    #     print("----")

    # Select algorithm
    print("\nChoose an algorithm to solve the puzzle:")
    print("1. Uniform Cost Search (UCS)")
    print("2. A* with Misplaced Tile Heuristic")
    print("3. A* with Manhattan Distance Heuristic")
    choice = int(input("Enter your choice (1/2/3): ").strip())

    # Run selected algorithm
    algorithm_name = ""
    solution = []
    metrics = {}
    start_time = time.time()

    if choice == 1:
        algorithm_name = "Uniform Cost Search"
        solution, metrics = uniform_cost_search(initial_state, goal_state)
    elif choice == 2:
        algorithm_name = "A* with Misplaced Tile Heuristic"
        solution, metrics = a_star_misplaced(initial_state, goal_state)
    elif choice == 3:
        algorithm_name = "A* with Manhattan Distance Heuristic"
        solution, metrics = a_star_manhattan(initial_state, goal_state)
    else:
        print("Invalid choice. Exiting...")
        return

    end_time = time.time()
    metrics["runtime"] = end_time - start_time

    # Print results
    print_solution(algorithm_name, solution, metrics)

if __name__ == "__main__":
    main()
