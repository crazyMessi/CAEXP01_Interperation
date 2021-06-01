#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <myIncludes/stb_image.h>
#include <myIncludes/shader_s.h>




using namespace std;
unsigned int loadTexture(const char* path);
//void loadText(const char* p1, const char* p2, unsigned int& t1, unsigned int& t2);


// 数值设定
//设置单边顶点个数
#define TIMES 1000
//步长。顶点的属性有x、y、r、b、g，因此步长为5
#define STRIDE 5
//边数。开始设为两条边
#define SIDE 2
//帧率。
#define FPS 10000

//若为0 显示动画。若为1，显示嘴巴，若为2，显示口罩
#define MOUTH_MASK 0




//视口
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

//顶点数组
float mouth[TIMES * SIDE * STRIDE];
float mask[TIMES * SIDE * STRIDE];
//索引数组
int index[(TIMES * SIDE - 2)*3];

//回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//初始化函数
void mouth_init();
void mask_init();
void index_init();

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ZHUODONG LI", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	mouth_init();
	mask_init();
	index_init();

	unsigned int mouth_VBO, mouth_VAO, mask_VBO, mask_VAO, EBO;

	glGenVertexArrays(1, &mouth_VAO);
	glGenBuffers(1, &mouth_VBO);
	glGenVertexArrays(1, &mask_VAO);
	glGenBuffers(1, &mask_VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(mouth_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mouth_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth), mouth, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);  

	glBindBuffer(GL_ARRAY_BUFFER, mask_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mask), mask, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(3);





	Shader mouth_Shader("mouth.vs","mouth.fs");
	Shader tex_Shader("textureshader.vs", "textureshader.fs");


	//加载纹理
	tex_Shader.use();
	unsigned int t = loadTexture("mouth.jpg");
	tex_Shader.setInt("texture1", 0);
	unsigned int t1 = loadTexture("mask.jpg");
	tex_Shader.setInt("texture2", 1);
	int time = 0;
	int flag = 1;
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, t1);

		//mouth_Shader.use();
		//mouth_Shader.setFloat("status", float(time)/float(FPS));
	
		tex_Shader.use();
		tex_Shader.setFloat("status", float(time)/float(FPS));
		
		glBindVertexArray(mouth_VAO);
		glDrawElements(GL_TRIANGLES, (SIDE * TIMES - 2)*6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		if (time >= FPS) {
			flag = -1;
		}
		if (time <= 0){
			flag = 1;
		}

		time += flag;

		if (MOUTH_MASK){
			time = 0;
			if (MOUTH_MASK==2){
				time = FPS;
			}
		}
	}

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void mouth_init()
{
	//嘴角坐标
	float left = -0.5;
	float right = 0.5;
	//r、g、b
	float r = 1.0;
	float g = 0.0;
	float b = 0.0;

	//顶点距离
	float offset = (right - left) / (TIMES-1);
	//载入顶点
	float x1, x2,y1,y2;
	for (size_t i = 0; i <= TIMES; i++){
		//计算坐标
		x1 = left + offset*i;
		x2 = x1;
		y1 = -(x1 - left) * (x1 - right);
		y2 = -y1;
		
		//载入顶点数组。每次载入两个顶点
		mouth[SIDE * i * STRIDE + 0] = x1;
		mouth[SIDE * i * STRIDE + 1] = y1;
		mouth[SIDE * i * STRIDE + 2] = r;
		mouth[SIDE * i * STRIDE + 3] = g;
		mouth[SIDE * i * STRIDE + 4] = b;
		mouth[(SIDE * i + 1) * STRIDE + 0] = x2;
		mouth[(SIDE * i + 1) * STRIDE + 1] = y2;
		mouth[(SIDE * i + 1) * STRIDE + 2] = r;
		mouth[(SIDE * i + 1) * STRIDE + 3] = g;
		mouth[(SIDE * i + 1) * STRIDE + 4] = b;

	}



}
void mask_init()
{
	//两侧坐标
	float left = -0.8;
	float right = 0.8;
	//r、g、b
	float r = 0.0;
	float g = 0.0;
	float b = 1.0;

	//顶点距离
	float offset = (right - left) / TIMES;
	//载入顶点
	float x1, x2, y1, y2;
	for (size_t i = 0; i <= TIMES; i++) {
		x1 = left + offset * i;
		x2 = x1;
		y1 = 0.5;
		y2 = -y1;

		//载入顶点数组。每次载入两个顶点
		mask[SIDE * i * STRIDE + 0] = x1;
		mask[SIDE * i * STRIDE + 1] = y1;
		mask[SIDE * i * STRIDE + 2] = r;
		mask[SIDE * i * STRIDE + 3] = g;
		mask[SIDE * i * STRIDE + 4] = b;
		mask[(SIDE * i + 1) * STRIDE + 0] = x2;
		mask[(SIDE * i + 1) * STRIDE + 1] = y2;
		mask[(SIDE * i + 1) * STRIDE + 2] = r;
		mask[(SIDE * i + 1) * STRIDE + 3] = g;
		mask[(SIDE * i + 1) * STRIDE + 4] = b;
	}
}
void index_init(){
	for (size_t i = 0; i < TIMES*SIDE-2; i++){
		index[i * 3 + 0] = i;
		index[i * 3 + 1] = i+1;
		index[i * 3 + 2] = i+2;
	}

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

