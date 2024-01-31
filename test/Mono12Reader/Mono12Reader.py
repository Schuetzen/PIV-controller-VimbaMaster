import os
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.colors as mcolors
import re


def switch_images(folder_path):
    # Get all image file names in the folder and sort them
    image_files = sorted([f for f in os.listdir(folder_path) if f.endswith('.tif')],
    key=lambda x: int(re.search(r'(\d+)', x).group()))

    num_images = len(image_files)
    if num_images == 0:
        print('No image files found')
        return

    # Initialize the current image index
    current_image_idx = 0

    # Create a larger figure window and display the first image
    fig, ax = plt.subplots(figsize=(12, 8))  # Increase the figure size
    image_data = mpimg.imread(os.path.join(folder_path, image_files[current_image_idx]))
    h_image = ax.imshow(image_data, cmap='gray')  # Use the "gray" colormap
    plt.colorbar(h_image, ax=ax)  # Associate the colorbar with the image
    h_image.set_clim(0, 2**12)  # Apply caxis settings to the image



    # Add a text annotation to display the image name
    h_text = ax.text(10, 45, image_files[current_image_idx], color='white')

    # Function to handle key press events
    def keypress_callback(event):
        nonlocal current_image_idx
        if event.key == 'right':
            current_image_idx = (current_image_idx + 1) % num_images
        elif event.key == 'left':
            current_image_idx = (current_image_idx - 1) % num_images

        # Update and display the new image
        new_image_data = mpimg.imread(os.path.join(folder_path, image_files[current_image_idx]))
        h_image.set_data(new_image_data)
        h_text.set_text(image_files[current_image_idx])

        # Refresh the plot
        plt.draw()

    # Set the key press callback function
    fig.canvas.mpl_connect('key_press_event', keypress_callback)

    # Display the plot
    plt.show()

# Specify the folder path
folder_path = '/home/pi/Desktop/Vim_cam/data/2024-01-30_17-01-46' # Replace with your image folder path
switch_images(folder_path)
