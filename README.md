# **sdl\_gui\_tutorials**

This repository contains the source code for a series of tutorials on building a UI framework using SDL2. The tutorials are a companion to the blog posts on [SDLGamesDev101](https://sdlgamesdev101.com).

## **About the Project**

**sdl\_gui\_tutorials** provides a step-by-step guide for creating fundamental UI controls from scratch, including buttons, labels, and input fields. We'll explore the core principles of game development, such as rendering, event handling, and state management, all within a clean and extensible framework.

The goal is to demystify how UI systems work and provide a solid foundation for creating your own user interfaces in SDL2.

## **Features**

* **Modular Design**: Learn to build a Scene-based architecture for organizing game content and UI elements.  
* **Core UI Controls**: Implement basic controls like buttons and labels from the ground up.  
* **Event Handling**: Discover how to process user input (mouse clicks, keyboard events) to make your UI interactive.  
* **State Management**: Understand how to manage the state of your UI components (e.g., hover, pressed).  
* **Clear Code**: The code is well-commented and easy to follow, making it perfect for beginners.

## **How to Use**

Each directory in this repository corresponds to a specific tutorial on the blog. To run the code for a particular chapter, simply navigate to the respective directory and compile the C++ files.

### **Prerequisites**

* [SDL2](https://www.libsdl.org/)  
* [SDL2\_ttf](https://github.com/libsdl-org/SDL_ttf)

For detailed instructions on setting up your development environment, please refer to the first tutorial post on my website.

### **Building and Running**

You can use a simple `cmake` or your favorite IDE (like Visual Studio or CMake) to build the projects.
eg. in powershell

```cmake
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=path/to/your/sdl/library path/to/your/simple_sdl_gui_tutorials/tutorials01/
cmake --build .
```


## **Support and Contribution**

This project is primarily a learning resource. If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## **Visit My Blog**

For the full, detailed tutorials and explanations of the code, please visit my website:

➡️ [**sdlgamesdev101.com**](https://sdlgamesdev101.com)

Happy coding\!

