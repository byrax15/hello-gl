#pragma once

#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>


struct GLFWwrapper {
	GLFWwindow* window;

	GLFWwrapper() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwSwapInterval(1);

		window = glfwCreateWindow(800, 600, "Triangles", nullptr, nullptr);
		glfwMakeContextCurrent(window);
	}

	~GLFWwrapper() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	auto drawLoop(auto&& drawFunc) {
		while (!glfwWindowShouldClose(window)) {
			drawFunc();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	GLFWwindow* operator*() { return window; }
};
