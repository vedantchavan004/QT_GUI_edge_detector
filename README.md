# Edge Detector GUI

A desktop application built using **Qt 6** and **OpenCV 4** that allows users to interactively apply **Sobel** and **Canny** edge detection on images with real-time threshold adjustment via sliders.

---

## 🚀 Features

- Load images from your file system
- Apply **Sobel** or **Canny** edge detection
- Adjust thresholds dynamically using sliders
- Clean and responsive GUI using Qt Designer

---

## 📦 Dependencies

- [Qt 6.x](https://www.qt.io/download)
- [OpenCV 4.x](https://opencv.org/)
- [CMake ≥ 3.10](https://cmake.org/)

---

## 🧱 Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/edge-detector-gui.git
cd edge-detector-gui

# Create and enter the build directory
mkdir build && cd build

# Run CMake
cmake ..

# Build the app
make

# Run
./EdgeDetectorGUI
```

> 💡 If OpenCV is not found, specify its path:
> ```bash
> cmake -DOpenCV_DIR=/your/path/to/opencv/build ..
> ```

---

## 🗂 Project Structure

```
├── CMakeLists.txt
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── README.md
└── .gitignore
```

---

## 📷 Preview

![result](https://github.com/user-attachments/assets/5b336585-9704-49bc-9bce-d1e44cd1bb04)


---

## 📄 License

This project is licensed under the MIT License.  
See [LICENSE](LICENSE) for details.

---

## 👤 Author

**Vedant Sanjay Chavan**  
Feel free to connect with me at [vedantsanjaychavan.de](https://vedantsanjaychavan.de)
