import os
import re
import tkinter as tk
from tkinter import filedialog
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class Mono12FrameViewer:
    def __init__(self, master):
        self.master = master
        self.master.title("Mono12 Frame Viewer")
        
        # Button to choose folder
        self.folder_button = tk.Button(master, text="Choose Folder", command=self.load_folder)
        self.folder_button.pack()

        # Placeholder for the figure canvas
        self.canvas = None

    def load_folder(self):
        folder_path = filedialog.askdirectory()
        if folder_path:
            self.switch_images(folder_path)

    def switch_images(self, folder_path):
        image_files = sorted([f for f in os.listdir(folder_path) if f.endswith('.tif')],
                             key=lambda x: int(re.search(r'(\d+)', x).group()))

        num_images = len(image_files)
        if num_images == 0:
            print('No image files found')
            return

        current_image_idx = 0
        fig, ax = plt.subplots(figsize=(12, 8))
        image_data = mpimg.imread(os.path.join(folder_path, image_files[current_image_idx]))
        h_image = ax.imshow(image_data, cmap='gray')
        plt.colorbar(h_image, ax=ax)
        h_image.set_clim(0, 2**12)

        h_text = ax.text(10, -10, image_files[current_image_idx], color='black')

        def keypress_callback(event, fig=fig, ax=ax):
            nonlocal current_image_idx
            if event.key == 'right':
                current_image_idx = (current_image_idx + 1) % num_images
            elif event.key == 'left':
                current_image_idx = (current_image_idx - 1) % num_images

            new_image_data = mpimg.imread(os.path.join(folder_path, image_files[current_image_idx]))
            h_image.set_data(new_image_data)
            h_text.set_text(image_files[current_image_idx])

            fig.canvas.draw()

        fig.canvas.mpl_connect('key_press_event', keypress_callback)

        # Embed the matplotlib figure in the Tkinter window
        if self.canvas:
            self.canvas.get_tk_widget().destroy()
        self.canvas = FigureCanvasTkAgg(fig, master=self.master)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack()

if __name__ == "__main__":
    root = tk.Tk()
    app = Mono12FrameViewer(root)
    root.mainloop()
