////////////////////////////////////////////////////////////////////////////////
//
//  Loading meshes from external files
//
// Copyright (c) 2023 by Carlos Martinho
//
// INTRODUCES:
// MODEL DATA, ASSIMP, mglMesh.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "../mgl/mgl.hpp"
#include <iomanip> 
#include <direct.h>
#include "../utils/sceneNode.h"
#define GetCurrentDir _getcwd
#include "../utils/CameraManager.h"
#include "../utils/InputManager.h"
#include <unordered_map>

////////////////////////////////////////////////////////////////////////// MYAPP



class MyApp : public mgl::App {
public:
	void initCallback(GLFWwindow* win) override;
	void displayCallback(GLFWwindow* win, double elapsed) override;
	void windowSizeCallback(GLFWwindow* win, int width, int height) override;
	void cursorCallback(GLFWwindow* window, double xpos, double ypos) override;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

private:
	const GLuint UBO_BP = 0;

	mgl::ShaderProgram* Shaders = nullptr;
	mgl::Camera* Camera = nullptr;
	mgl::Camera* CameraPerspective = nullptr;
	CameraManager* cameraManager = nullptr;
	GLint ModelMatrixId, ColorId;
	mgl::Mesh* Mesh = nullptr;
	InputManager inputManager = InputManager();
	std::unordered_map<std::string, SceneNode> sceneGraph;

	//void createMesh(std::string name, std::string mesh_file);
	mgl::Mesh* createMesh(std::string mesh_file);
	void createMeshes();
	void createShaderPrograms(std::string mesh);
	void createAllShaderPrograms();
	void createCamera();
	void drawScene();
};


void print_mat4(const glm::mat4& matrix, std::string matrixName) {
	const float* matData = glm::value_ptr(matrix);

	std::cout << matrixName << ":" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << std::setw(12) << std::setprecision(6) << std::fixed << matData[i + j * 4] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////// MESHES

mgl::Mesh* MyApp::createMesh(std::string mesh_file) {
	std::string mesh_dir = "assets/models/";
	std::string mesh_fullname = mesh_dir + mesh_file;

	Mesh = new mgl::Mesh();
	Mesh->joinIdenticalVertices();
	Mesh->create(mesh_fullname);

	return Mesh;

	//sceneGraph.insert({ std::string(name), SceneNode(glm::mat4(1.0f), Shaders, Mesh) });
}

void MyApp::createMeshes() {

	mgl::Mesh* mesh = createMesh("cube-bevel.obj");
	sceneGraph.insert({ "cube", SceneNode(glm::mat4(1.0f), Shaders, mesh)});
	sceneGraph.at("cube").modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) /** glm::rotate(glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f))*/;

	//auto modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));
	//mgl::Mesh* mesh2 = createMesh("cube-bevel.obj");
	//sceneGraph.at("cube").addChild(new SceneNode(modelMatrix, Shaders, mesh2));
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms(std::string mesh) {
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf("Error getting current directory\n");
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	printf("The current working directory is %s\n", cCurrentPath);

	Shaders = new mgl::ShaderProgram();
	std::string shader_dir = "assets/shaders/";
	Shaders->addShader(GL_VERTEX_SHADER, shader_dir + "cube-vs.glsl");
	Shaders->addShader(GL_FRAGMENT_SHADER, shader_dir + "cube-fs.glsl");

	Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	if (Mesh->hasNormals()) {
		Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
	}
	if (Mesh->hasTexcoords()) {
		Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
	}
	if (Mesh->hasTangentsAndBitangents()) {
		Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
	}

	Shaders->addUniform(mgl::MODEL_MATRIX);
	Shaders->addUniform(mgl::COLOR_ATTRIBUTE);
	Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	Shaders->create();

	ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
	ColorId = Shaders->Uniforms[mgl::COLOR_ATTRIBUTE].index;

	std::cout << "Current Piece: " << mesh << std::endl;
	sceneGraph.at(mesh).shader = Shaders;
}

void MyApp::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager.updateMousePosition(xpos, ypos);

	if (inputManager.isMouseButtonHeldOrPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		float delta[2];
		inputManager.getDeltaMousePos(delta);
		cameraManager->getCurrentCamera()->updateOrientation(delta[0], -delta[1]);
		// deltaY is negative if the mouse goes up so we need to
		// add a - sign to it to rotate by a positive angle.
	}
}

void MyApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	inputManager.updateMouseButtonState(button, action);
}

void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputManager.updateKeyState(key, action);

	if (inputManager.isKeyPressed(GLFW_KEY_C)) {
		cameraManager->switchCamera();
	}

	if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
		exit(0);
	}
}

void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// Update the camera radius based on the y offset of the scroll
	float scrollDelta = static_cast<float>(yoffset);
	cameraManager->updateCameraZoom(scrollDelta);
}

void MyApp::createAllShaderPrograms() {
	for (auto const& node : sceneGraph) {
		createShaderPrograms(node.first);
	}
}
///////////////////////////////////////////////////////////////////////// CAMERA

glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
float radius = 5.0f;
glm::mat4 translateRadius = glm::translate(glm::vec3(0.0f, 0.0f, -radius));

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), center,
	up);

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), center,
	up);

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 orthogonalProjection =
glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const glm::mat4 perspectiveProjection =
glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 1.0f, 15.0f);

void MyApp::createCamera() {
	cameraManager = new CameraManager(UBO_BP);

	Camera = new mgl::Camera(UBO_BP, center, up, 5.0f);
	Camera->setViewMatrix(Camera->calculateViewMatrix());
	Camera->setProjectionMatrix(orthogonalProjection);

	CameraPerspective = new mgl::Camera(UBO_BP, center, up, 5.0f);
	CameraPerspective->setViewMatrix(Camera->calculateViewMatrix());
	CameraPerspective->setProjectionMatrix(perspectiveProjection);

	cameraManager->addCamera(Camera);
	cameraManager->addCamera(CameraPerspective);
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix(1.0f);

void MyApp::drawScene() {
	cameraManager->sendMatrices();

	for (auto& node : sceneGraph) {
		node.second.draw();
	}

}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
	createMeshes();
	createAllShaderPrograms();  // after mesh;
	createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
	glViewport(0, 0, winx, winy);
	// change projection matrices to maintain aspect ratio
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
	mgl::Engine& engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp());
	engine.setOpenGL(4, 6);
	engine.setWindow(800, 600, "Tangram 3D", 0, 1);
	engine.init();
	engine.run();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
