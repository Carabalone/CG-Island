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
#define GetCurrentDir _getcwd
#include <unordered_map>

#include "../utils/utils.h"

void printCurrentDir();
////////////////////////////////////////////////////////////////////////// MYAPP


class MyApp : public mgl::App {
public:
	void initCallback(GLFWwindow* win) override;
	void displayCallback(GLFWwindow* win, double elapsed) override;
	void windowSizeCallback(GLFWwindow* win, int width, int height) override;
	void addMeshAttributes(std::string mesh, std::vector<std::string>& attributes);
	mgl::ShaderProgram* createShaderProgram(std::string vertexShader, std::string fragmentShader, std::vector<std::string> uniforms);
	void cursorCallback(GLFWwindow* window, double xpos, double ypos) override;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

private:
	const GLuint UBO_BP = 0;

	mgl::Camera* Camera = nullptr;
	mgl::Camera* CameraPerspective = nullptr;
	CameraManager* cameraManager = nullptr;
	GLint ModelMatrixId, ColorId;
	mgl::Mesh* Mesh = nullptr;
	InputManager inputManager = InputManager();
	std::unordered_map<std::string, SceneNode> sceneGraph;
	mgl::ShaderManager shaderManager = mgl::ShaderManager();
	float fov = 30.0f;
	float time = 0.0f;

	//void createMesh(std::string name, std::string mesh_file);
	mgl::Mesh* createMesh(std::string mesh_file, bool generateNormals = false);
	SceneNode* createSilhouette(SceneNode node);
	SceneNode* createLighthousePiece(const std::string& name, const std::string& fileName, const glm::vec3& color);
	void createMeshes();
	void createAllShaderPrograms();
	void setNewProjectionMatrix(float fov);
	void createCamera();
	void drawScene();
	void setupTextures();
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

mgl::Mesh* MyApp::createMesh(std::string mesh_file, bool generateNormals) {
	std::string mesh_dir = "assets/models/";
	std::string mesh_fullname = mesh_dir + mesh_file;

	Mesh = new mgl::Mesh();
	if (generateNormals)
		Mesh->generateSmoothNormals();
	Mesh->joinIdenticalVertices();
	Mesh->create(mesh_fullname);

	return Mesh;

	//sceneGraph.insert({ std::string(name), SceneNode(glm::mat4(1.0f), Shaders, Mesh) });
}

SceneNode* MyApp::createSilhouette(SceneNode node) {
	mgl::Mesh* mesh = node.mesh;

	auto silhouetteNode = new SceneNode("silhouette-" + node.name, glm::mat4(1.0f), nullptr, mesh);
	silhouetteNode->modelMatrix = glm::scale(glm::vec3(1.013f));
	silhouetteNode->shader = shaderManager.getShader("silhouette");
	silhouetteNode->callback = new SilhouetteCallback();

	RenderConfig rc = RenderConfig();
	rc.sendUniforms = [](mgl::ShaderProgram* shader) {

	};

	silhouetteNode->renderConfig = rc;

	return silhouetteNode;
}

SceneNode* MyApp::createLighthousePiece(const std::string& name, const std::string& fileName, const glm::vec3& color) {
	mgl::Mesh* lighthouseMesh = createMesh(fileName, true);

	auto lighthouseNode = new SceneNode(name, glm::mat4(1.0f), nullptr, lighthouseMesh);
	lighthouseNode->shader = shaderManager.getShader("cel-shading");
	lighthouseNode->modelMatrix = glm::mat4(1.0f);

	RenderConfig rc;
	rc.sendUniforms = [color](mgl::ShaderProgram* shader) {
		glUniform1i(shader->Uniforms["useTexture"].index, false);
		glUniform1f(shader->Uniforms["glossiness"].index, 32.0f);
		glUniform3f(shader->Uniforms["colorUniform"].index, color.x, color.y, color.z);
	};

	lighthouseNode->renderConfig = rc;

	lighthouseNode->addChild(createSilhouette(*lighthouseNode));

	return lighthouseNode;
}

void MyApp::createMeshes() {

	mgl::Mesh* mesh = createMesh("testsphere.obj");

	{
		auto torus = SceneNode("mainSphere", glm::mat4(1.0f), nullptr, mesh);
		torus.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 7.0f));
		torus.shader = shaderManager.getShader("cel-shading");

		RenderConfig torusConfig = RenderConfig();

		torusConfig.sendUniforms = [](mgl::ShaderProgram* shader) {
			glUniform1i(shader->Uniforms["useTexture"].index, false);
			glUniform1f(shader->Uniforms["glossiness"].index, 16.0f);
			glUniform3f(shader->Uniforms["colorUniform"].index, 1.0f, 0.0f, 0.0f);
		};

		torus.renderConfig = torusConfig;
		sceneGraph.insert({ "torus", torus });
	}

	{
		mgl::ShaderProgram* silhouetteShader = shaderManager.getShader("silhouette");

		SilhouetteCallback* silhouetteCallback = new SilhouetteCallback();
		sceneGraph.at("torus").addChild(new SceneNode(
			"silhouette", glm::mat4(1.0f) * glm::scale(glm::vec3(1.013f)), silhouetteShader, mesh,
			silhouetteCallback
		));
	}

	{
		mgl::Mesh* grid = createMesh("grid.obj");

		auto gridNode = SceneNode("grid", glm::mat4(1.0f), nullptr, grid);
		gridNode.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(4.0f, 1.0f, 4.0f));
		gridNode.shader = shaderManager.getShader("water-toon");
		//gridNode.addTexture("saul_goodman_tex");
		gridNode.addTexture("waveFoamNoise");
		gridNode.addTexture("depthMap");
		gridNode.addTexture("surfaceDepth");
		gridNode.addTexture("dudvMap");
		gridNode.addTexture("refractedTexture");

		RenderConfig rc = RenderConfig();

		rc.sendUniforms = [](mgl::ShaderProgram* shader) {

		};

		gridNode.renderConfig = rc;
		gridNode.transparent = true;
		gridNode.callback = new DepthTestCallback();

		sceneGraph.insert({ "grid", gridNode });
	}

	mgl::Mesh* terrain = createMesh("island-smooth-big.obj");
	{

		auto terrainNode = SceneNode("terrain", glm::mat4(1.0f), nullptr, terrain);
		terrainNode.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f)) * glm::scale(glm::vec3(1.0f, 0.7f, 1.0f));
		terrainNode.shader = shaderManager.getShader("cel-shading");
		terrainNode.addTexture("sand_tex");
		terrainNode.addTexture("normalMap");

		RenderConfig rc = RenderConfig();
		rc.sendUniforms = [](mgl::ShaderProgram* shader) {
			glUniform1i(shader->Uniforms["useTexture"].index, true);
			glUniform1f(shader->Uniforms["glossiness"].index, 1000.0f);
		};

		terrainNode.renderConfig = rc;

		sceneGraph.insert({ "terrain", terrainNode });

		sceneGraph.at("terrain").addChild(createSilhouette(terrainNode));
	}

	{
		mgl::Mesh* lighthouseRed = createMesh("lighthouse-red.obj", true);
		SceneNode lighthouseRedNode = SceneNode("lighthouse", glm::mat4(1.0f), nullptr, lighthouseRed);
		lighthouseRedNode.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(9.5f, -1.1f, 0.0f)) *
			glm::rotate(glm::radians(1.5f), glm::vec3(0.0f, 0.0f, -1.0f)) *
			glm::scale(glm::vec3(0.3f));
		lighthouseRedNode.shader = shaderManager.getShader("cel-shading");

		RenderConfig rc = RenderConfig();
		rc.sendUniforms = [](mgl::ShaderProgram* shader) {
			glUniform1i(shader->Uniforms["useTexture"].index, false);
			glUniform1f(shader->Uniforms["glossiness"].index, 32.0f);
			glUniform3f(shader->Uniforms["colorUniform"].index, 0.8f, 0.1f, 0.1f);
		};

		lighthouseRedNode.renderConfig = rc;

		sceneGraph.insert({ "lighthouse", lighthouseRedNode });
		sceneGraph.at("lighthouse").addChild(createSilhouette(lighthouseRedNode));

		SceneNode* lighthouseWhiteNode = createLighthousePiece("lighthouseWhite", "lighthouse-white.obj", glm::vec3(0.8f, 0.8f, 0.8f));
		SceneNode* lighthouseGrayNode = createLighthousePiece("lighthouseGray", "lighthouse-gray.obj", glm::vec3(0.5f, 0.5f, 0.5f));
		SceneNode* lighthouseDarkGrayNode = createLighthousePiece("lighthouseDarkGray", "lighthouse-dark-gray.obj", glm::vec3(0.3f, 0.3f, 0.3f));
		SceneNode* lighthouseYellowNode = createLighthousePiece("lighthouseYellow", "lighthouse-yellow.obj", glm::vec3(0.8f, 0.8f, 0.1f));

		////// Add child nodes to the parent
		sceneGraph.at("lighthouse").addChild(lighthouseWhiteNode);
		sceneGraph.at("lighthouse").addChild(lighthouseGrayNode);
		sceneGraph.at("lighthouse").addChild(lighthouseDarkGrayNode);
		sceneGraph.at("lighthouse").addChild(lighthouseYellowNode);

	}

	{
		mgl::Mesh* rock1 = createMesh("rock_1.obj", true);

		SceneNode rock1Node = SceneNode("rock1", glm::mat4(1.0f), nullptr, rock1);

		rock1Node.modelMatrix = glm::translate(glm::vec3(-4.0f, -2.0f, -4.0f)) *
			glm::rotate(glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::vec3(0.02f));

		rock1Node.shader = shaderManager.getShader("cel-shading");
		rock1Node.addTexture("rock_1");

		RenderConfig rc = RenderConfig();

		rc.sendUniforms = [](mgl::ShaderProgram* shader) {
			glUniform1i(shader->Uniforms["useTexture"].index, true);
			glUniform1f(shader->Uniforms["glossiness"].index, 1000.0f);
		};

		rock1Node.renderConfig = rc;

		sceneGraph.insert({ "rock1", rock1Node });

		sceneGraph.at("rock1").addChild(createSilhouette(rock1Node));
	}

	{
		mgl::Mesh* rock2 = createMesh("rock_2.obj", true);

		SceneNode rock2Node = SceneNode("rock2", glm::mat4(1.0f), nullptr, rock2);

		rock2Node.modelMatrix = glm::translate(glm::vec3(-6.0f, -1.0f, 5.0f)) *
			glm::rotate(glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::vec3(20.0f));
		rock2Node.shader = shaderManager.getShader("cel-shading");
		rock2Node.addTexture("rock_2");

		RenderConfig rc = RenderConfig();

		rc.sendUniforms = [](mgl::ShaderProgram* shader) {
			glUniform1i(shader->Uniforms["useTexture"].index, true);
			glUniform1f(shader->Uniforms["glossiness"].index, 1000.0f);
		};
		rock2Node.renderConfig = rc;

		sceneGraph.insert({ "rock2", rock2Node });

		sceneGraph.at("rock2").addChild(createSilhouette(rock2Node));
	}

}

void MyApp::setupTextures() {

	mgl::TextureManager& textureManager = mgl::TextureManager::getInstance();

	// Load the texture using the TextureManager
	mgl::Texture2D* saulGoodmanTexture = new mgl::Texture2D();
	saulGoodmanTexture->load("assets/textures/saul_goodman.png");

	// Create a sampler (you can customize this based on your needs)
	mgl::LinearSampler* saulGoodmanSampler = new mgl::LinearSampler();
	saulGoodmanSampler->create();

	// Add the texture and sampler to the TextureManager
	textureManager.addTexture("saul_goodman_tex", GL_TEXTURE0, 0, "tex1", saulGoodmanTexture, saulGoodmanSampler);



	// Load the texture using the TextureManager
	mgl::Texture2D* sandTexture = new mgl::Texture2D();
	sandTexture->load("assets/textures/sand.png");

	// Create a sampler (you can customize this based on your needs)
	mgl::LinearSampler* sandSampler = new mgl::LinearSampler();
	sandSampler->create();

	// Add the texture and sampler to the TextureManager
	textureManager.addTexture("sand_tex", GL_TEXTURE1, 1, "tex1", sandTexture, sandSampler);



	mgl::Texture2D* waveFoamNoise = new mgl::Texture2D();

	//waveFoamNoise->generatePerlinNoise(512, 512, 50);
	waveFoamNoise->load("assets/textures/perlin_noise.png");

	mgl::LinearSampler* waveFoamNoiseSampler = new mgl::LinearSampler();
	waveFoamNoiseSampler->create();

	textureManager.addTexture("waveFoamNoise", GL_TEXTURE2, 2, "waveFoamNoise", waveFoamNoise, waveFoamNoiseSampler);



	mgl::DepthTexture* depthTexture = new mgl::DepthTexture();

	depthTexture->create(800, 600);

	textureManager.addTexture("depthMap", GL_TEXTURE4, 4, "depthMap", reinterpret_cast<mgl::Texture*>(depthTexture), nullptr);

	if (!reinterpret_cast<mgl::Texture*>(depthTexture)) {
		std::cout << "[DEBUG] reinterpret_cast from DepthTexture to Texture failed" << std::endl;
		exit(1);
	}


	mgl::RenderTargetTexture* refractedTexture = new mgl::RenderTargetTexture();

	refractedTexture->create(800, 600);

	textureManager.addTexture("refractedTexture", GL_TEXTURE5, 5, "refractedTexture", reinterpret_cast<mgl::Texture*>(refractedTexture), nullptr);

	if (!reinterpret_cast<mgl::Texture*>(refractedTexture)) {
		std::cout << "[DEBUG] reinterpret_cast from RenderTargetTexture to Texture failed" << std::endl;
		exit(1);
	}

	mgl::Texture2D* dudvMap = new mgl::Texture2D();

	dudvMap->load("assets/textures/waterDUDV.png");

	mgl::LinearSampler* dudvMapSampler = new mgl::LinearSampler();

	dudvMapSampler->create();

	textureManager.addTexture("dudvMap", GL_TEXTURE6, 6, "dudvMap", dudvMap, dudvMapSampler);

	mgl::Texture2D* normalMap = new mgl::Texture2D();

	normalMap->load("assets/textures/sand_normal_map.png");

	mgl::LinearSampler* normalMapSampler = new mgl::LinearSampler();

	normalMapSampler->create();

	textureManager.addTexture("normalMap", GL_TEXTURE7, 7, "normalMap", normalMap, normalMapSampler);


	mgl::Texture2D* rock_1 = new mgl::Texture2D();

	rock_1->load("assets/textures/rock_1.png");

	mgl::LinearSampler* rock_1Sampler = new mgl::LinearSampler();

	rock_1Sampler->create();

	textureManager.addTexture("rock_1", GL_TEXTURE8, 8, "tex1", rock_1, rock_1Sampler);

	mgl::Texture2D* rock_2 = new mgl::Texture2D();

	rock_2->load("assets/textures/rock_2.png");

	mgl::LinearSampler* rock_2Sampler = new mgl::LinearSampler();

	rock_2Sampler->create();

	textureManager.addTexture("rock_2", GL_TEXTURE9, 9, "tex1", rock_2, rock_2Sampler);

}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::addMeshAttributes(std::string mesh_name, std::vector<std::string>& attributes) {
	auto mesh = sceneGraph.at(mesh_name).mesh;

	if (mesh->hasNormals())
		attributes.push_back(mgl::NORMAL_ATTRIBUTE);
	if (mesh->hasTexcoords())
		attributes.push_back(mgl::TEXCOORD_ATTRIBUTE);
	if (mesh->hasTangentsAndBitangents())
		attributes.push_back(mgl::TANGENT_ATTRIBUTE);
}

mgl::ShaderProgram* MyApp::createShaderProgram(std::string vertexShader, std::string fragmentShader,
	std::vector<std::string> uniforms) {

	auto Shaders = new mgl::ShaderProgram();
	std::string shader_dir = "assets/shaders/";

	Shaders->addShader(GL_VERTEX_SHADER, shader_dir + vertexShader);
	Shaders->addShader(GL_FRAGMENT_SHADER, shader_dir + fragmentShader);

	Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

	Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
	Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
	Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);

	Shaders->addUniform(mgl::MODEL_MATRIX);

	for (auto uniform : uniforms) {
		Shaders->addUniform(uniform);
	}

	Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	Shaders->create();

	ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;

	return Shaders;
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

	if (inputManager.isKeyPressed(GLFW_KEY_W)) {
	}

	if (inputManager.isKeyHeld(GLFW_KEY_Y)) {
	}
}

void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// Update the camera radius based on the y offset of the scroll
	//float scrollDelta = static_cast<float>(yoffset);
	//cameraManager->updateCameraZoom(scrollDelta);

	fov -= static_cast<float>(yoffset);
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;

	setNewProjectionMatrix(fov);

}

void MyApp::createAllShaderPrograms() {
	shaderManager.addShader("cel-shading", createShaderProgram("cel-shading.vert",
		"cel-shading.frag",
		std::vector<std::string>{"lightDir", "lineColor", "tex1", "useTexture", "colorUniform", "glossiness", "normalMap", mgl::CLIP_PLANE, "clip"}
	));

	shaderManager.addShader("silhouette", createShaderProgram("silhouette.vert", "silhouette.frag",
		std::vector<std::string>{mgl::CLIP_PLANE, "clip"}
	));

	shaderManager.addShader("water-toon", createShaderProgram("water-toon.vert", "water-toon.frag",
		std::vector<std::string>{ "tex1", mgl::TIME, "depthMap", "waveFoamNoise", "dudvMap", "refractedTexture" }
	));

	shaderManager.addShader("simple", createShaderProgram("simple-vs.glsl", "simple-fs.glsl",
		std::vector<std::string>{ }
	));

	shaderManager.addShader("depth", createShaderProgram("depth-vs.glsl", "depth-fs.glsl",
		std::vector<std::string>{ "clip" }
	));

	shaderManager.addShader("screen", createShaderProgram("screen-vs.glsl", "screen-fs.glsl",
		std::vector<std::string>{ "refractedTexture" }
	));

	/*shaderManager.addShader("refracted", createShaderProgram("refracted-vs.glsl", "refracted-fs.glsl",
				std::vector<std::string>{ }
	));*/
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

void MyApp::setNewProjectionMatrix(float fov) {
	const glm::mat4 perspectiveProjection =
		glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.5f, 55.0f);

	CameraPerspective->setProjectionMatrix(perspectiveProjection);
}


void MyApp::createCamera() {

	// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
	const glm::mat4 perspectiveProjection =
		glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.5f, 55.0f);

	cameraManager = new CameraManager(UBO_BP);

	Camera = new mgl::Camera(UBO_BP, center, up, 5.0f);
	Camera->setViewMatrix(Camera->calculateViewMatrix());
	Camera->setProjectionMatrix(orthogonalProjection);

	CameraPerspective = new mgl::Camera(UBO_BP, center, up, 30.0f);
	CameraPerspective->setViewMatrix(Camera->calculateViewMatrix());
	CameraPerspective->setProjectionMatrix(perspectiveProjection);

	cameraManager->addCamera(Camera);
	cameraManager->addCamera(CameraPerspective);
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix(1.0f);

void MyApp::drawScene() {
	cameraManager->sendMatrices();

	mgl::TextureManager& textureManager = mgl::TextureManager::getInstance();

	mgl::DepthTexture* depthTexture = reinterpret_cast<mgl::DepthTexture*>(textureManager.getTexture("depthMap"));
	mgl::RenderTargetTexture* refractedTexture = reinterpret_cast<mgl::RenderTargetTexture*>(textureManager.getTexture("refractedTexture"));
	if (!(depthTexture && refractedTexture)) {
		std::cout << "[DEBUG] reinterpret_cast from Texture to RTT in DrawScene() failed" << std::endl;
		exit(1);
	}

	mgl::ShaderProgram* depthShader = shaderManager.getShader("depth");
	mgl::ShaderProgram* screenShader = shaderManager.getShader("screen");

	// first pass to render the depth map
	depthTexture->bindFramebuffer();

	for (auto& node : sceneGraph) {
		if (node.second.name == "grid") {
			continue;
		}

		node.second.renderConfig.clip = true;
		node.second.draw(depthShader);
		node.second.renderConfig.clip = false;
	}

	depthTexture->unbindFramebuffer();

	// second pass to render the refracted texture
	refractedTexture->bindFramebuffer();

	glEnable(GL_CLIP_DISTANCE0);
	for (auto& node : sceneGraph) {
		node.second.renderConfig.clip = true;
		node.second.draw();
		node.second.renderConfig.clip = false;
	}
	glDisable(GL_CLIP_DISTANCE0);
	refractedTexture->unbindFramebuffer();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// third pass to render the scene
	for (auto& node : sceneGraph) {
		if (node.second.transparent) { // draw transparent objects last
			continue;
		}
		node.second.draw();
	}

	for (auto& node : sceneGraph) {
		if (!node.second.transparent) {
			continue;
		}
		node.second.draw();
	}


}


////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
	createAllShaderPrograms(); // create all shader programs before meshes
	setupTextures();
	createMeshes();
	createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
	glViewport(0, 0, winx, winy);
	// change projection matrices to maintain aspect ratio
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) {
	time += elapsed;
	RenderConfig::time = time;

	drawScene();
}

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
