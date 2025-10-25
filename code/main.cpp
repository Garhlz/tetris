// main.cpp
/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - 本代码仅仅是参考代码，具体要求请参考作业说明，按照顺序逐步完成。
 * - 关于配置OpenGL开发环境、编译运行，请参考第一周实验课程相关文档。
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格和‘L’型方块
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 *
 * - 未实现功能如下：
 * - 1) 绘制‘J’、‘Z’等形状的方块
 * - 2) 随机生成方块并赋上不同的颜色
 * - 3) 方块的自动向下移动
 * - 4) 方块之间、方块与边界之间的碰撞检测
 * - 5) 棋盘格中每一行填充满之后自动消除
 * - 6) 其他
 *
 */

#include "Angel.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include "loadAndSave.h"
using glm::vec2;
using glm::vec4;

int rotation = 0; // 控制当前窗口中的方块旋转
vec2 tile[4];	  // 表示当前窗口中的方块
int xsize = 400;
int ysize = 720;

double startTime;
double endTime;

double lastTime = glfwGetTime();
double fallInterval = 0.5;

// 单个网格大小
int tile_width = 50;

// 网格布大小
const int board_width = 10;
const int board_height = 20;

// 网格三角面片的顶点数量
const int points_num = board_height * board_width * 6;

// 我们用画直线的方法绘制网格
// 包含竖线 board_width+1 条
// 包含横线 board_height+1 条
// 一条线2个顶点坐标
// 网格线的数量
const int board_line_num = (board_width + 1) + (board_height + 1);

int score = 0;
// 先直接用已经消除的行数表示得分

enum GameState
{
	STATE_START_SCREEN, // 开始界面
	STATE_PLAYING,		// 游戏中
	STATE_PAUSE,		// 暂停中
	STATE_GAME_OVER		// 游戏结束
};
// 添加全局变量来存储当前状态，初始为开始界面
GameState currentState = STATE_START_SCREEN;

std::string currentUsername;

// 直接用全局变量来管理信息
GameRecord gameRecord;

// 用一个二维数组表示所有可能出现的方块和方向
// 注意如果只有两个方向，就得交错存储
vec2 OShape[4][4] =
	{{vec2(0, 0), vec2(0, -1), vec2(-1, 0), vec2(-1, -1)},
	 {vec2(0, 0), vec2(0, -1), vec2(-1, 0), vec2(-1, -1)},
	 {vec2(0, 0), vec2(0, -1), vec2(-1, 0), vec2(-1, -1)},
	 {vec2(0, 0), vec2(0, -1), vec2(-1, 0), vec2(-1, -1)}};

vec2 IShape[4][4] = {
	{vec2(1, 0), vec2(0, 0), vec2(-1, 0), vec2(-2, 0)},
	{vec2(0, 1), vec2(0, 0), vec2(0, -1), vec2(0, -2)},
	{vec2(1, 0), vec2(0, 0), vec2(-1, 0), vec2(-2, 0)},
	{vec2(0, 1), vec2(0, 0), vec2(0, -1), vec2(0, -2)}};

vec2 SShape[4][4] = {
	{vec2(1, 0), vec2(0, 0), vec2(0, -1), vec2(-1, -1)},
	{vec2(0, 1), vec2(0, 0), vec2(1, 0), vec2(1, -1)},
	{vec2(1, 0), vec2(0, 0), vec2(0, -1), vec2(-1, -1)},
	{vec2(0, 1), vec2(0, 0), vec2(1, 0), vec2(1, -1)}};

vec2 ZShape[4][4] = {
	{vec2(-1, 0), vec2(0, 0), vec2(0, -1), vec2(1, -1)},
	{vec2(0, -1), vec2(0, 0), vec2(1, 0), vec2(1, 1)},
	{vec2(-1, 0), vec2(0, 0), vec2(0, -1), vec2(1, -1)},
	{vec2(0, -1), vec2(0, 0), vec2(1, 0), vec2(1, 1)}};

vec2 LShape[4][4] = {
	{vec2(0, 0), vec2(-1, 0), vec2(1, 0), vec2(-1, -1)},
	{vec2(0, 1), vec2(0, 0), vec2(0, -1), vec2(1, -1)},
	{vec2(1, 1), vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
	{vec2(-1, 1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};

vec2 JShape[4][4] = {
	{vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(1, -1)},
	{vec2(0, -1), vec2(0, 0), vec2(0, 1), vec2(1, 1)},
	{vec2(-1, 1), vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
	{vec2(-1, -1), vec2(0, -1), vec2(0, 0), vec2(0, 1)}};

vec2 TShape[4][4] = {
	{vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(0, -1)},
	{vec2(0, -1), vec2(0, 0), vec2(1, 0), vec2(0, 1)},
	{vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(0, 1)},
	{vec2(0, -1), vec2(0, 0), vec2(-1, 0), vec2(0, 1)},
};

vec2 (*allShapes[7])[4][4] = {
	&OShape,
	&IShape,
	&SShape,
	&ZShape,
	&LShape,
	&JShape,
	&TShape};

vec4 shapeColors[7] = {
	vec4(1.0, 0.5, 0.0, 1.0), // O (橙色)
	vec4(0.0, 1.0, 1.0, 1.0), // I (青色)
	vec4(1.0, 1.0, 0.0, 1.0), // S (黄色)
	vec4(0.0, 1.0, 0.0, 1.0), // Z (绿色)
	vec4(1.0, 0.0, 0.0, 1.0), // L (红色)
	vec4(0.0, 0.0, 1.0, 1.0), // J (蓝色)
	vec4(0.5, 0.0, 0.5, 1.0)  // T (紫色)
};

// 添加一个全局变量来存储当前方块的类型和颜色
int currentShapeType;
vec4 currentTileColor;

// 绘制窗口的颜色变量
vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 gray = vec4(0.5, 0.5, 0.5, 1.0);
// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilePos = vec2(5, 19);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[board_width][board_height];

// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardColors[points_num];

GLuint locxsize;
GLuint locysize;

GLuint vao[3];
GLuint vbo[6];

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool checkValid(vec2 pos)
{
	// 检查是否在边界内
	if ((pos.x >= 0) && (pos.x < board_width) && (pos.y >= 0) && (pos.y < board_height))
	{
		// 如果在边界内，再检查该位置是否已被占据
		if (board[(int)pos.x][(int)pos.y])
		{
			return false; // 位置被占据，无效
		}
		return true; // 位置在界内且未被占据，有效
	}
	else
	{
		return false; // 超出边界，无效
	}
}

// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO
void changeCellColor(vec2 pos, vec4 colour)
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardColors[(int)(6 * (board_width * pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(board_width * pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 当前方块移动或者旋转时，更新VBO
void updateTile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilePos.x + tile[i].x;
		GLfloat y = tilePos.y + tile[i].y;
		// 修改深度
		vec4 p1 = vec4(tile_width + (x * tile_width), tile_width + (y * tile_width), -0.1, 1);
		vec4 p2 = vec4(tile_width + (x * tile_width), tile_width * 2 + (y * tile_width), -0.1, 1);
		vec4 p3 = vec4(tile_width * 2 + (x * tile_width), tile_width + (y * tile_width), -0.1, 1);
		vec4 p4 = vec4(tile_width * 2 + (x * tile_width), tile_width * 2 + (y * tile_width), -0.1, 1);

		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

void gameover()
{
	currentState = STATE_GAME_OVER;
	// 更新记录信息
	endTime = glfwGetTime();
	double duration = endTime - startTime;
	gameRecord.setDuration(duration);
	gameRecord.setScore(score);
	saveGameRecord(gameRecord);

	score = 0;
	gameRecord.clear();
	gameRecord.setUsername(currentUsername);
}

// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断（没有足够的空间来生成新的方块）
void newTile()
{
	// 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
	tilePos = vec2(board_width / 2, board_height - 2);
	rotation = rand() % 4; // 随机初始旋转状态 (0-3)

	currentShapeType = rand() % 7;					  // 随机选择一个形状 (0-6)
	currentTileColor = shapeColors[currentShapeType]; // 获取对应颜色

	// 从选中的形状和旋转状态中获取方块数据
	for (int i = 0; i < 4; i++)
	{
		// 使用 allShapes 数组来获取正确的形状和旋转
		tile[i] = (*allShapes[currentShapeType])[rotation][i];
		if (!checkValid(tilePos + tile[i]))
		{ // 只要有一个格子无效,游戏结束
			gameover();
			return;
		}
	}

	updateTile(); // 更新当前方块的VBO

	// 给新方块赋上对应的颜色
	vec4 newcolours[24]; // 4个格子 * 6个顶点
	for (int i = 0; i < 24; i++)
		newcolours[i] = currentTileColor; // 使用随机选取的颜色

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

// 游戏和OpenGL初始化
void init()
{
	// 初始化棋盘格，这里用画直线的方法绘制网格
	// 包含竖线 board_width+1 条
	// 包含横线 board_height+1 条
	// 一条线2个顶点坐标，并且每个顶点一个颜色值

	vec4 gridpoints[board_line_num * 2];
	vec4 gridcolours[board_line_num * 2];

	// 绘制网格线， 修改深度
	// 纵向线
	for (int i = 0; i < (board_width + 1); i++)
	{
		gridpoints[2 * i] = vec4((tile_width + (tile_width * i)), tile_width, -0.3, 1);
		gridpoints[2 * i + 1] = vec4((tile_width + (tile_width * i)), (board_height + 1) * tile_width, -0.3, 1);
	}

	// 水平线
	for (int i = 0; i < (board_height + 1); i++)
	{
		gridpoints[2 * (board_width + 1) + 2 * i] = vec4(tile_width, (tile_width + (tile_width * i)), -0.3, 1);
		gridpoints[2 * (board_width + 1) + 2 * i + 1] = vec4((board_width + 1) * tile_width, (tile_width + (tile_width * i)), -0.3, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < (board_line_num * 2); i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	vec4 boardpoints[points_num];
	for (int i = 0; i < points_num; i++)
		boardColors[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
	for (int i = 0; i < board_height; i++)
		for (int j = 0; j < board_width; j++)
		{
			// 修改深度
			vec4 p1 = vec4(tile_width + (j * tile_width), tile_width + (i * tile_width), -0.2, 1);
			vec4 p2 = vec4(tile_width + (j * tile_width), tile_width * 2 + (i * tile_width), -0.2, 1);
			vec4 p3 = vec4(tile_width * 2 + (j * tile_width), tile_width + (i * tile_width), -0.2, 1);
			vec4 p4 = vec4(tile_width * 2 + (j * tile_width), tile_width * 2 + (i * tile_width), -0.2, 1);
			boardpoints[6 * (board_width * i + j) + 0] = p1;
			boardpoints[6 * (board_width * i + j) + 1] = p2;
			boardpoints[6 * (board_width * i + j) + 2] = p3;
			boardpoints[6 * (board_width * i + j) + 3] = p2;
			boardpoints[6 * (board_width * i + j) + 4] = p3;
			boardpoints[6 * (board_width * i + j) + 5] = p4;
		}

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < board_width; i++)
		for (int j = 0; j < board_height; j++)
			board[i][j] = false;

	// 载入着色器
	std::string vshader, fshader;
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";
	GLuint program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vao[0]);
	glBindVertexArray(vao[0]); // 棋盘格顶点

	glGenBuffers(2, vbo);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, (board_line_num * 2) * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, (board_line_num * 2) * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(vao[1]); // 棋盘格每个格子

	glGenBuffers(2, &vbo[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, points_num * sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, points_num * sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(vao[2]); // 当前方块

	glGenBuffers(2, &vbo[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);

	glClearColor(0, 0, 0, 0);

	// 启用深度测试
	// glEnable(GL_DEPTH_TEST);
	// 游戏初始化
	newTile();
}

// 在棋盘上有足够空间的情况下旋转当前方块
void rotate()
{
	int nextRotation = (rotation + 1) % 4;
	vec2 nextTileCoords[4]; // 下一个状态的坐标
	for (int i = 0; i < 4; ++i)
	{
		// 使用 nextRotation 获取坐标
		nextTileCoords[i] = (*allShapes[currentShapeType])[nextRotation][i];
	}

	// 检查旋转后的位置是否有效
	bool canRotate = true;
	for (int i = 0; i < 4; ++i)
	{
		if (!checkValid(nextTileCoords[i] + tilePos))
		{
			canRotate = false;
			break;
		}
	}

	if (canRotate)
	{
		rotation = nextRotation; // 更新旋转状态
		for (int i = 0; i < 4; i++)
		{
			tile[i] = nextTileCoords[i];
		}
		updateTile(); // 更新 VBO
	}
}

bool checkFull(int row)
{
	for (int i = 0; i < board_width; i++)
	{
		if (!board[i][row])
		{
			return false;
		}
	}
	return true;
}

void eliminateRow(int row)
{
	// 如果该行已满，则进行消除和下落操作,从当前行开始，到最高行-1
	for (int j = row; j < board_height - 1; j++)
	{
		// 将上一行(j+1)的状态和颜色复制到当前行(j)
		for (int i = 0; i < board_width; i++)
		{
			board[i][j] = board[i][j + 1];
			// 更新VBO中的颜色信息
			vec4 colorAbove = black; // 默认为黑色
			// 计算 (i, j+1) 在 board_colours 中的起始索引
			int indexAbove = 6 * (board_width * (j + 1) + i);
			if (indexAbove < points_num)
			{
				colorAbove = boardColors[indexAbove];
			}
			changeCellColor(vec2(i, j), colorAbove);
		}
	}
	// 清空最顶行
	for (int i = 0; i < board_width; i++)
	{
		board[i][board_height - 1] = false;
		changeCellColor(vec2(i, board_height - 1), black);
	}
	// 增加得分。因为在窗口中显示比较复杂，这里暂时只在命令行显示
	score++;
	std::cout << "分数: " << score << std::endl;
}

// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO
void setTile()
{
	// 每个格子
	int minY = board_height - 1;
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilePos).x;
		int y = (tile[i] + tilePos).y;
		minY = std::min(minY, y);
		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		changeCellColor(vec2(x, y), gray);
	}

	// 从影响的底部开始检查所有行，直到顶部
	for (int y_check = minY; y_check < board_height; ++y_check)
	{
		bool row_was_full = checkFull(y_check);
		if (row_was_full)
		{
			eliminateRow(y_check);
			y_check--;
		}
	}
}

// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)
// 分别对应于向左，向下和向右移动。如果移动成功，返回值为true，反之为false
bool moveTile(vec2 direction)
{
	// 计算移动之后的方块的位置坐标
	vec2 newPos[4];
	for (int i = 0; i < 4; i++)
		newPos[i] = tile[i] + tilePos + direction;

	// 检查移动之后的有效性
	if (checkValid(newPos[0]) && checkValid(newPos[1]) && checkValid(newPos[2]) && checkValid(newPos[3]))
	{
		// 有效：移动该方块
		tilePos.x = tilePos.x + direction.x;
		tilePos.y = tilePos.y + direction.y;

		updateTile();

		return true;
	}

	return false;
}

// 重新启动游戏
void restart()
{
	// 清空棋盘格状态
	for (int i = 0; i < board_width; i++)
	{
		for (int j = 0; j < board_height; j++)
		{
			if (board[i][j])
			{
				board[i][j] = false;
				changeCellColor(vec2(i, j), black); // 将颜色改回黑色
			}
		}
	}
	score = 0;
	// 设置开始时间
	gameRecord.setDateTime();
	startTime = glfwGetTime();
	lastTime = glfwGetTime();

	newTile();

	std::cout << std::endl
			  << "游戏开始!" << std::endl;
	std::cout << "分数: " << score << std::endl;
}

// 游戏渲染部分
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	// 绘制棋盘格背景和网格线 (在所有状态下都可能需要显示)
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, points_num); // 棋盘格背景 (已固定的方块)
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, board_line_num * 2); // 棋盘格线

	// 只在 PLAYING 或 PAUSE 状态下绘制当前活动方块
	if (currentState == STATE_PLAYING || currentState == STATE_PAUSE)
	{
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, 24); // 当前方块
	}

	if (currentState == STATE_PAUSE)
	{
		std::cout << "暂停中... 按 P 继续" << '\r';
	}
	else if (currentState == STATE_GAME_OVER)
	{
		std::cout << "游戏结束! 最终得分: " << score << "  按 R 重新开始" << '\r';
	}
	else if (currentState == STATE_START_SCREEN)
	{
		std::cout << "按 Enter 开始游戏..." << '\r';
	}

	glFlush(); // 确保绘制命令被发送
}

// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// 通用按键退出，可以在任何状态下触发
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
	{
		gameover();
		if (action == GLFW_PRESS)
		{
			exit(EXIT_SUCCESS);
		}

		return; // 处理完退出后直接返回
	}

	// 根据当前游戏状态处理按键
	switch (currentState)
	{
	case STATE_START_SCREEN:
		// 在开始界面，只响应回车键开始游戏
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			restart(); // 初始化或重置游戏状态
			currentState = STATE_PLAYING;
		}
		break;

	case STATE_PLAYING:
		// 在游戏进行中
		// 处理方块移动和旋转 (只有在PLAYING状态下有效)
		switch (key)
		{
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				rotate();
			break;
		case GLFW_KEY_SPACE: // 直接fall_through
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				if (!moveTile(vec2(0, -1)))
				{
					setTile();
					newTile(); // newTile 内部会检查是否游戏结束并切换状态
				}
			}
			break;
		case GLFW_KEY_LEFT:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				moveTile(vec2(-1, 0));
			break;
		case GLFW_KEY_RIGHT:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				moveTile(vec2(1, 0));
			break;
		// 处理暂停键
		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
			{
				currentState = STATE_PAUSE; // 切换到暂停状态
				std::cout << "游戏已暂停" << std::endl;
				// 暂停时不需要重置lastTime，因为恢复时main循环会自动处理
			}
			break;
		}
		break;

	case STATE_PAUSE:
		// 在暂停状态下，只响应 'P' 键取消暂停
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			currentState = STATE_PLAYING; // 切换回游戏状态
			// 重置下落计时器，避免恢复时立即下落
			lastTime = glfwGetTime();

			std::cout << std::endl
					  << "游戏已恢复" << std::endl;
		}
		break; // STATE_PAUSE 结束

	case STATE_GAME_OVER:
		// 在游戏结束状态下，只响应 'R' 键重新开始
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			currentState = STATE_PLAYING; // 切换回游戏状态
			restart();					  // 重置游戏
		}
		break; // STATE_GAME_OVER 结束
	}
}

void inputUsername()
{

	char recordChoice;
	bool validInput = false;

	while (!validInput)
	{
		std::cout << "是否要记录本次游戏用户名？(y/n): ";
		std::cin >> recordChoice;

		// 清理输入缓冲区，防止上次输入的回车影响后续 getline
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		recordChoice = std::tolower(recordChoice); // 转换为小写

		if (recordChoice == 'y')
		{
			std::cout << "请输入您的用户名: ";
			// 使用 std::getline 读取可能包含空格的用户名
			std::getline(std::cin, currentUsername);
			if (currentUsername.empty())
			{ // 防止用户直接回车
				std::cout << "用户名不能为空，将使用默认名: " << defaultUsername << std::endl;
			}
			else
			{
				gameRecord.setUsername(currentUsername);
			}
			validInput = true;
		}
		else if (recordChoice == 'n')
		{

			std::cout << "将使用默认名: " << defaultUsername << std::endl;
			validInput = true;
		}
		else
		{
			std::cout << "输入无效，请输入 'y' 或 'n'" << std::endl;
			// 清理 cin 的错误状态（如果输入了非字符）
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}
}

int main(int argc, char **argv)
{
	// 使用命令行交互获取username
	inputUsername();

	// 打印游戏记录
	printGameRecords();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 创建窗口。
	// 计算棋盘区域宽度 + 网格线宽度 + 右侧留白宽度
	int boardPixelWidth = (board_width + 2) * tile_width; // +2 是为了左右边框
	int scoreBoardWidth = 150;							  // 假设给计分板留 150 像素
	int windowWidth = boardPixelWidth + scoreBoardWidth;

	// 计算棋盘区域高度 + 网格线高度
	int windowHeight = (board_height + 2) * tile_width; // +2 是为了上下边框

	// 使用新尺寸创建窗口
	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Tetris - 2023271073华浩名", NULL, NULL);

	// 更新全局变量 (如果其他地方用到了 xsize, ysize)
	xsize = windowWidth;
	ysize = windowHeight;
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window))
	{
		// 只在 STATE_PLAYING 状态下处理自动下落
		if (currentState == STATE_PLAYING)
		{
			double currentTime = glfwGetTime();
			if (currentTime - lastTime >= fallInterval)
			{
				// 尝试向下移动
				if (!moveTile(vec2(0, -1)))
				{
					setTile(); // 固定方块
					newTile(); // 生成新方块 (newTile 内部检查是否 Game Over 并可能切换状态)
				}
				lastTime = currentTime; // 更新上次下落时间
			}
		}

		display(); // 渲染当前帧

		// 缓冲区交换与事件处理
		glfwSwapBuffers(window); // 显示画面
		glfwPollEvents();		 // 处理输入和其他窗口事件
	}
	glfwTerminate();
	return 0;
}
