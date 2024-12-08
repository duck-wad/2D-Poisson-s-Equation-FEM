
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
import pandas as pd

def plot_3d_contour(csv_file):
    # Load CSV file
    data = pd.read_csv(csv_file, header=None, names=["x", "y", "potential"])

    # Extract unique x and y coordinates
    x_coords = np.sort(data["x"].unique())
    y_coords = np.sort(data["y"].unique())

    # Create a meshgrid for x and y
    X, Y = np.meshgrid(x_coords, y_coords)

    # Reshape potential values into a 2D grid
    Z = data.pivot(index="y", columns="x", values="potential").values

    # Create the 3D plot
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')

    # Plot the surface
    contour = ax.plot_surface(X, Y, Z, cmap=cm.viridis, edgecolor='k', alpha=0.9)

    # Add color bar
    cbar = fig.colorbar(contour, ax=ax, shrink=0.5, aspect=10)
    cbar.set_label("Potential")

    # Set labels
    ax.set_xlabel("X Coordinate")
    ax.set_ylabel("Y Coordinate")
    ax.set_zlabel("Potential")
    ax.set_title("3D Contour Plot of Potential")

    plt.show()

# Example usage
csv_file = "NODE_OUTPUT.csv"  # Replace with your CSV file
plot_3d_contour(csv_file)
