import matplotlib.pyplot as plt

def generate_input_and_visualize(subdivisions, domain_size, output_filename, point_sources, bcs, flux_value):
    # Generate nodes
    node_coords = [(x, y) for y in range(subdivisions + 1) for x in range(subdivisions + 1)]

    # Generate elements
    elements = []
    for j in range(subdivisions):
        for i in range(subdivisions):
            n1 = j * (subdivisions + 1) + i
            n2 = n1 + 1
            n3 = n2 + subdivisions
            n4 = n3 + 1
            elements.append((n1, n2, n4, n3))

    # Write to output file
    with open(output_filename, 'w') as file:
        file.write(f"coeff: 1.0\n")

        # Write elements
        file.write(f"numelem: {len(elements)}\n")
        for idx, element in enumerate(elements):
            file.write(f"element{idx + 1} indices: {element[0] + 1} {element[1] + 1} {element[2] + 1} {element[3] + 1}\n")

        # Write nodes
        for idx, (x, y) in enumerate(node_coords):
            file.write(f"node{idx + 1} coord: {x * (domain_size / subdivisions):.6f} {y * (domain_size / subdivisions):.6f}\n")

        # Write flux boundaries (right and top edges with parameterized flux value)
        fluxes = []

        # Top boundary flux
        for j in range(subdivisions):
            node1 = subdivisions * (subdivisions + 1) + j + 1
            node2 = node1 + 1
            fluxes.append((node1, node2, flux_value))

        # Right boundary flux
        for i in range(subdivisions):
            node1 = (i + 1) * (subdivisions + 1)
            node2 = node1 + (subdivisions + 1)
            fluxes.append((node1, node2, flux_value))

        # Write flux data
        file.write(f"numflux: {len(fluxes)}\n")
        for idx, (node1, node2, value) in enumerate(fluxes, start=1):
            file.write(f"flux{idx} location: {node1} {node2} value: {value}\n")

        # Write point sources
        file.write(f"numpointsource: {len(point_sources)}\n")
        for idx, (element, value) in enumerate(point_sources, start=1):
            file.write(f"source{idx} element: {element} value: {value}\n")

        # Write boundary conditions
        file.write(f"numBC: {len(bcs)}\n")
        for idx, (node, value) in enumerate(bcs, start=1):
            file.write(f"bc{idx} location: {node} value: {value}\n")

    print(f"Input file '{output_filename}' has been generated.")

    # Visualize the mesh
    visualize_mesh(subdivisions, domain_size, node_coords, elements, bcs, fluxes)

def visualize_mesh(subdivisions, domain_size, node_coords, elements, bcs, fluxes):
    # Plot nodes
    fig, ax = plt.subplots(figsize=(10, 10))
    for idx, (x, y) in enumerate(node_coords):
        if any(node == idx + 1 for node, _ in bcs):
            ax.plot(x * (domain_size / subdivisions), y * (domain_size / subdivisions), 'ro')  # Boundary condition node as red dot
        else:
            ax.plot(x * (domain_size / subdivisions), y * (domain_size / subdivisions), 'bo')  # Plot node as a blue dot
        ax.text(x * (domain_size / subdivisions), y * (domain_size / subdivisions), f'{idx + 1}', color='red', fontsize=6)  # Label node number

    # Plot elements
    for element in elements:
        x_coords = [node_coords[n][0] * (domain_size / subdivisions) for n in element] + [node_coords[element[0]][0] * (domain_size / subdivisions)]
        y_coords = [node_coords[n][1] * (domain_size / subdivisions) for n in element] + [node_coords[element[0]][1] * (domain_size / subdivisions)]
        ax.plot(x_coords, y_coords, 'k-')  # Connect nodes with black lines

    # Plot flux boundaries
    for node1, node2, _ in fluxes:
        x_coords = [node_coords[node1 - 1][0] * (domain_size / subdivisions), node_coords[node2 - 1][0] * (domain_size / subdivisions)]
        y_coords = [node_coords[node1 - 1][1] * (domain_size / subdivisions), node_coords[node2 - 1][1] * (domain_size / subdivisions)]
        ax.plot(x_coords, y_coords, 'g-', linewidth=2, label='Flux boundary' if 'Flux boundary' not in ax.get_legend_handles_labels()[1] else '')

    # Label plot
    ax.set_title(f'Mesh Visualization with {subdivisions}x{subdivisions} Elements')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.axis('equal')
    ax.grid(True)
    ax.legend()
    plt.show()

# Generate input file and visualize
subdivisions = 5
domain_size = 1.0
output_filename = "INPUT5_NOFLUX.txt"
point_sources = []  # Example: [(element, value), ...]
bcs = [(1, 3.0)]  # Example: [(node, value), ...]
flux_value = 0.0

generate_input_and_visualize(subdivisions, domain_size, output_filename, point_sources, bcs, flux_value)
