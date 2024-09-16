import matplotlib.pyplot as plt
import networkx as nx

# Function to load adjacency list from file
def load_adjacency_list(file):
    adjacency_list = {}
    with open(file, 'r') as f:
        for line in f:
            parts = line.split(':')
            node = int(parts[0].strip())
            neighbors = list(map(int, parts[1].strip().split()))
            adjacency_list[node] = neighbors
    return adjacency_list

# Function to plot and save the directed graph with arbitrary node placement
def plot_and_save_directed_edges(adjacency_list, save_path, title, layout_type='spring'):
    G = nx.DiGraph()  # Directed graph
    
    # Add directed edges to the graph
    for node, neighbors in adjacency_list.items():
        for neighbor in neighbors:
            G.add_edge(node, neighbor)
    
    # Choose layout for node placement
    if layout_type == 'spring':
        pos = nx.spring_layout(G, seed=42)  # Seed for reproducibility
    elif layout_type == 'circular':
        pos = nx.circular_layout(G)
    elif layout_type == 'kamada_kaway':
        pos = nx.kamada_kaway_layout(G)
    else:
        pos = nx.random_layout(G)  # Arbitrary random placement if layout is not recognized
    
    # Draw the directed graph with arrows
    plt.figure(figsize=(10, 8))
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, 
            font_size=10, font_weight='bold', edge_color='gray', arrows=True, 
            arrowstyle='->', arrowsize=20)
    
    # Add title
    plt.title(title)
    
    # Save the plot to the specified path
    plt.savefig(save_path)
    print(f"Directed graph saved as {save_path}")
    
    # Show the plot (optional)
    plt.show()

# Load the adjacency list from a file
adjacency_list_file = 'data/initial_adjacency_list.txt'
adjacency_list = load_adjacency_list(adjacency_list_file)

# Specify the save path, title, and layout type
save_path = 'figures/directed_initial_graph_visualization.png'
title = 'Initial Directed Graph Visualization'
layout_type = 'spring'  # Try 'circular', 'kamada_kaway', or 'spring'

# Plot and save the directed edges
plot_and_save_directed_edges(adjacency_list, save_path, title, layout_type)

# Load the adjacency list from a file
adjacency_list_file = 'data/final_adjacency_list.txt'
adjacency_list = load_adjacency_list(adjacency_list_file)

# Specify the save path, title, and layout type
save_path = 'figures/directed_final_graph_visualization.png'
title = 'Final Directed Graph Visualization'
layout_type = 'spring'  # Try 'circular', 'kamada_kaway', or 'spring'

# Plot and save the directed edges
plot_and_save_directed_edges(adjacency_list, save_path, title, layout_type)