#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "shader.h"
#include "camera.h"
//#include "waves.h"
#include "model.h"

using namespace std;

const string modelBasePath = "E:\\资料\\大三下\\计算机图形学\\CGFinalProject\\CGFinalProject\\model\\";

std::vector<float>* init_height_map(unsigned char* data, float width, float height, float chan, float rec_width) {
	(void)height;
	auto vertices = new std::vector<float>();

	float start = -(width / 2) * rec_width;

	for (int x = 0; x < width; x++)
	{
		float pos_x = start + x * rec_width;
		for (int z = 0; z < width; z++)
		{
			float pos_z = -(start + z * rec_width);

			int pos_img = (x + z * width) * chan;
			float pixel = data[pos_img]; // Height map is already grayscaled

			pixel = (pixel / 10) - 10; // Hardcoded scaling
			vertices->push_back(pos_x);
			vertices->push_back(pixel);
			vertices->push_back(pos_z);

			// Texture coordinates
			vertices->push_back(pos_x);
			vertices->push_back(pos_z);
		}
	}

	return vertices;
}

std::vector<float>* init_plane(int size, float width, float height) {
	auto vertices = new std::vector<float>();

	float start = -(size / 2) * width;

	for (int x = 0; x < size; x++)
	{
		float pos_x = start + x * width;
		for (int z = 0; z < size; z++)
		{
			float pos_z = -(start + z * width);
			vertices->push_back(pos_x);
			vertices->push_back(height);
			vertices->push_back(pos_z);

			// Texture coordinates
			vertices->push_back(pos_x);
			vertices->push_back(pos_z);
		}
	}
	return vertices;
}

std::vector<unsigned int>* init_indices(int size) {
	auto indices = new std::vector<unsigned int>();

	for (int z = 0; z < size - 1; ++z)
	{
		for (int x = 0; x < size - 1; ++x)
		{
			int start = x + z * size;
			indices->push_back(start);
			indices->push_back(start + 1);
			indices->push_back(start + size);
			indices->push_back(start + 1);
			indices->push_back(start + 1 + size);
			indices->push_back(start + size);
		}
	}

	return indices;
}

unsigned int load_object(std::vector<float>* vertices, std::vector<unsigned int>* indices) {
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), &vertices->at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &indices->at(0), GL_STATIC_DRAW);

	// Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

unsigned int load_cubemap(void) {
	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	return VAO;
}

void top_left(std::vector<float>& vertices, float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	vertices.push_back(x);
	vertices.push_back(z);
}

void top_right(std::vector<float>& vertices, float width, float x, float y, float z)
{
	vertices.push_back(x + width);
	vertices.push_back(y);
	vertices.push_back(z);

	vertices.push_back(x + width);
	vertices.push_back(z);
}

void bottom_left(std::vector<float>& vertices, float width, float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + width);

	vertices.push_back(x);
	vertices.push_back(z + width);
}

void bottom_right(std::vector<float>& vertices, float width, float x, float y, float z)
{
	vertices.push_back(x + width);
	vertices.push_back(y);
	vertices.push_back(z + width);

	vertices.push_back(x + width);
	vertices.push_back(z + width);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void init_opengl(void);
unsigned int load_texture(const char* path);
unsigned int load_cubemap_texture(std::vector<std::string> faces);

// settings
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

float mouse_x = SRC_WIDTH / 2;
float mouse_y = SRC_HEIGHT / 2;
float fov = 45.0f;
unsigned char mouse_action = 0;

float opt_speed = 0.8f;
float opt_amount = 0.01f;
float opt_height = 0.5f;
unsigned char opt_mesh = 0;


Camera* camera = new Camera();
GLFWwindow* window = NULL;

int main(void)
{
	init_opengl();

	float rec_width = 0.5f;

	/* Initialize the cubemap */
	Shader shader_cubemap("vertex_cubemap.glsl", "frag_cubemap.glsl");
	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"up.jpg",
		"down.jpg",
		"front.jpg",
		"back.jpg"
	};
	unsigned int texture_cubemap = load_cubemap_texture(faces);
	unsigned int VAO_cubemap = load_cubemap();

	/* Initialize the sand */
	Shader shader_sand("vertex_sand.glsl", "frag_sand.glsl");
	unsigned int texture_sand = load_texture("ground.jpg");

	int width, height, chan;
	unsigned char* data = stbi_load("height_map.jpg", &width, &height, &chan, 0);
	auto vertices_sand = init_height_map(data, width, height, chan, rec_width);
	stbi_image_free(data);
	auto indices_sand = init_indices(width);
	unsigned int VAO_sand = load_object(vertices_sand, indices_sand);

	/* Initialize the water */
	Shader shader_water("vertex_water.glsl", "frag_water.glsl");
	unsigned int texture_water = load_texture("water.png");
	auto vertices_water = init_plane(width, rec_width, 4.0f);
	auto indices_water = init_indices(width);
	unsigned int VAO_water = load_object(vertices_water, indices_water);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);


	shader_cubemap.use();
	shader_cubemap.setInt("skybox", 2);

	shader_water.use();
	shader_water.setInt("TexWater", 0);
	shader_water.setInt("skybox", 2);

	shader_sand.use();
	shader_sand.setInt("TexSand", 1);

	Shader ourShader("vertex_model.glsl", "frag_model.glsl");
	//Model ourModel(modelBasePath + "tree\\file.obj");
	Model ourModel(modelBasePath + "palm/Palm.obj");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_water);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_sand);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		/* Sand */
		shader_sand.use();
		shader_sand.updateView(fov, SRC_WIDTH, SRC_HEIGHT, camera->GetViewMatrix(), false);

		glBindVertexArray(VAO_sand);
		glDrawElements(GL_TRIANGLES, indices_sand->size(), GL_UNSIGNED_INT, 0);

		/* Water */
		shader_water.use();
		shader_water.updateView(fov, SRC_WIDTH, SRC_HEIGHT, camera->GetViewMatrix(), false);

		shader_water.setFloat("time", glfwGetTime());
		shader_water.setVec3("cameraPos", camera->Position);

		shader_water.setFloat("speed", opt_speed);
		shader_water.setFloat("amount", opt_amount);
		shader_water.setFloat("height", opt_height);

		glBindVertexArray(VAO_water);
		glDrawElements(GL_TRIANGLES, indices_water->size(), GL_UNSIGNED_INT, 0);

		/* Cubemap */
		glDepthFunc(GL_LEQUAL);
		shader_cubemap.use();
		shader_cubemap.updateView(fov, SRC_WIDTH, SRC_HEIGHT, camera->GetViewMatrix(), true);

		glBindVertexArray(VAO_cubemap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		//ourShader.updateView(fov, SRC_WIDTH, SRC_HEIGHT, camera->GetViewMatrix(), false);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 3.0f, 20.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	(void)window;

	static float last_time = glfwGetTime();

	float curr_time = glfwGetTime();
	float delta_time = curr_time - last_time;
	last_time = curr_time;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// -- Camera
		if (key == GLFW_KEY_RIGHT)
			camera->ProcessKeyboard(RIGHT, delta_time);
		else if (key == GLFW_KEY_LEFT)
			camera->ProcessKeyboard(LEFT, delta_time);
		else if (key == GLFW_KEY_UP)
			camera->ProcessKeyboard(FORWARD, delta_time);
		else if (key == GLFW_KEY_DOWN)
			camera->ProcessKeyboard(BACKWARD, delta_time);
		// --- Water
		// Speed
		else if (key == GLFW_KEY_Q && opt_speed < 2.0f)
			opt_speed += 0.1f;
		else if (key == GLFW_KEY_W && opt_speed > 0.0f)
			opt_speed -= 0.1f;
		// Amount
		else if (key == GLFW_KEY_A && opt_amount < 0.1f)
			opt_amount += 0.01f;
		else if (key == GLFW_KEY_S && opt_amount > 0.0f)
			opt_amount -= 0.1f;
		// Height
		else if (key == GLFW_KEY_Z && opt_height < 0.5f)
			opt_height += 0.1f;
		else if (key == GLFW_KEY_X && opt_height > 0.0f)
			opt_height -= 0.1f;
		// -- Misc
		// Mesh grid
		else if (key == GLFW_KEY_M)
		{
			if (opt_mesh)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			opt_mesh = opt_mesh ? 0 : 1;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	(void)window;

	if (!mouse_action)
		return;

	float xoffset = mouse_x - xpos;
	float yoffset = mouse_y - ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
	mouse_x = xpos;
	mouse_y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	(void)mods;
	(void)window;

	if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			mouse_action = 1;
	}
	else
		mouse_action = 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void init_opengl(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Water Simulation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set callbacks
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
}

unsigned int load_texture(const char* path)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

unsigned int load_cubemap_texture(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
			//exit(1);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

