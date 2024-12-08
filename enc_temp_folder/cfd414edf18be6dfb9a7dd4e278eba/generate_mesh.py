
import matplotlib.pyplot as plt

def generate_input_and_visualize(subdivisions, domain_size, output_filename):
    delta = domain_size / subdivisions
    num_nodes = (subdivisions + 1)**2
    num_elements = subdivisions**2

    node_coords = []  # To store node coordinates for visualization
    elements = []     # To store element connectivity for visualization

    # Open the file to write
    with open(output_filename, "w") as file:
        # Write coefficient
        file.write("coeff: 1.0\n")
        
        # Write number of elements
        file.write(f"numelem: {num_elements}\n")

        # Write element connectivity
        element_id = 1
        for i in range(subdivisions):
            for j in range(subdivisions):
                node1 = i * (subdivisions + 1) + j + 1
                node2 = node1 + 1
                node3 = node2 + subdivisions + 1
                node4 = node1 + subdivisions + 1
                file.write(f"element{element_id} indices: {node1} {node2} {node3} {node4}\n")
                elements.append([node1 - 1, node2 - 1, node3 - 1, node4 - 1])  # For visualization
                element_id += 1

        # Write node coordinates
        file.write(f"numcoord: {num_nodes}\n")
        node_id = 1
        for i in range(subdivisions + 1):
            for j in range(subdivisions + 1):
                x = j * delta
                y = i * delta
                file.write(f"node{node_id} coord: {x:.6f} {y:.6f}\n")
                node_coords.append((x, y))  # For visualization
                node_id += 1

        # Write flux boundaries (right and top edges with flux = 1)
        flux_id = 1
        file.write("numflux: 60\n")  # Flux applied only on the top and right boundaries

        # Top boundary flux
        for j in range(subdivisions):
            node1 = subdivisions * (subdivisions + 1) + j + 1
            node2 = node1 + 1
            file.write(f"flux{flux_id} location: {node1} {node2} value: 1.0\n")
            flux_id += 1

        # Right boundary flux
        for i in range(subdivisions):
            node1 = (i + 1) * (subdivisions + 1)
            node2 = node1 + (subdivisions + 1)
            file.write(f"flux{flux_id} location: {node1} {node2} value: 1.0\n")
            flux_id += 1

    print(f"Input file '{output_filename}' has been generated.")

    # Visualize the mesh
    visualize_mesh(subdivisions, domain_size, node_coords, elements)

def visualize_mesh(subdivisions, domain_size, node_coords, elements):
    # Plot nodes
    fig, ax = plt.subplots(figsize=(10, 10))
    for idx, (x, y) in enumerate(node_coords):
        ax.plot(x, y, 'bo')  # Plot node as a blue dot
        ax.text(x, y, f'{idx + 1}', color='red', fontsize=6)  # Label node number

    # Plot elements
    for element in elements:
        x_coords = [node_coords[n][0] for n in element] + [node_coords[element[0]][0]]
        y_coords = [node_coords[n][1] for n in element] + [node_coords[element[0]][1]]
        ax.plot(x_coords, y_coords, 'k-')  # Connect nodes with black lines

    # Label plot
    ax.set_title(f'Mesh Visualization with {subdivisions}x{subdivisions} Elements')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.axis('equal')
    ax.grid(True)
    plt.show()

# Generate input file and visualize
generate_input_and_visualize(subdivisions=30, domain_size=1.0, output_filename="input900_with_flux.txt")