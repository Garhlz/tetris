// main.cpp
/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
 * - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
 *
 * - ��ʵ�ֹ������£�
 * - 1) �������̸�͡�L���ͷ���
 * - 2) ������/��/�¼����Ʒ�����ƶ����ϼ���ת����
 *
 * - δʵ�ֹ������£�
 * - 1) ���ơ�J������Z������״�ķ���
 * - 2) ������ɷ��鲢���ϲ�ͬ����ɫ
 * - 3) ������Զ������ƶ�
 * - 4) ����֮�䡢������߽�֮�����ײ���
 * - 5) ���̸���ÿһ�������֮���Զ�����
 * - 6) ����
 *
 */

#include "Angel.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include "loadAndSave.h"
using glm::vec2;
using glm::vec4;

int rotation = 0; // ���Ƶ�ǰ�����еķ�����ת
vec2 tile[4];	  // ��ʾ��ǰ�����еķ���
int xsize = 400;
int ysize = 720;

double startTime;
double endTime;

double lastTime = glfwGetTime();
double fallInterval = 0.5;

// ���������С
int tile_width = 50;

// ���񲼴�С
const int board_width = 10;
const int board_height = 20;

// ����������Ƭ�Ķ�������
const int points_num = board_height * board_width * 6;

// �����û�ֱ�ߵķ�����������
// �������� board_width+1 ��
// �������� board_height+1 ��
// һ����2����������
// �����ߵ�����
const int board_line_num = (board_width + 1) + (board_height + 1);

int score = 0;
// ��ֱ�����Ѿ�������������ʾ�÷�

enum GameState
{
	STATE_START_SCREEN, // ��ʼ����
	STATE_PLAYING,		// ��Ϸ��
	STATE_PAUSE,		// ��ͣ��
	STATE_GAME_OVER		// ��Ϸ����
};
// ���ȫ�ֱ������洢��ǰ״̬����ʼΪ��ʼ����
GameState currentState = STATE_START_SCREEN;

std::string currentUsername;

// ֱ����ȫ�ֱ�����������Ϣ
GameRecord gameRecord;

// ��һ����ά�����ʾ���п��ܳ��ֵķ���ͷ���
// ע�����ֻ���������򣬾͵ý���洢
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
	vec4(1.0, 0.5, 0.0, 1.0), // O (��ɫ)
	vec4(0.0, 1.0, 1.0, 1.0), // I (��ɫ)
	vec4(1.0, 1.0, 0.0, 1.0), // S (��ɫ)
	vec4(0.0, 1.0, 0.0, 1.0), // Z (��ɫ)
	vec4(1.0, 0.0, 0.0, 1.0), // L (��ɫ)
	vec4(0.0, 0.0, 1.0, 1.0), // J (��ɫ)
	vec4(0.5, 0.0, 0.5, 1.0)  // T (��ɫ)
};

// ���һ��ȫ�ֱ������洢��ǰ��������ͺ���ɫ
int currentShapeType;
vec4 currentTileColor;

// ���ƴ��ڵ���ɫ����
vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 gray = vec4(0.5, 0.5, 0.5, 1.0);
// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��
vec2 tilePos = vec2(5, 19);

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[board_width][board_height];

// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
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
	// ����Ƿ��ڱ߽���
	if ((pos.x >= 0) && (pos.x < board_width) && (pos.y >= 0) && (pos.y < board_height))
	{
		// ����ڱ߽��ڣ��ټ���λ���Ƿ��ѱ�ռ��
		if (board[(int)pos.x][(int)pos.y])
		{
			return false; // λ�ñ�ռ�ݣ���Ч
		}
		return true; // λ���ڽ�����δ��ռ�ݣ���Ч
	}
	else
	{
		return false; // �����߽磬��Ч
	}
}

// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO
void changeCellColor(vec2 pos, vec4 colour)
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardColors[(int)(6 * (board_width * pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(board_width * pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ��ǰ�����ƶ�������תʱ������VBO
void updateTile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilePos.x + tile[i].x;
		GLfloat y = tilePos.y + tile[i].y;
		// �޸����
		vec4 p1 = vec4(tile_width + (x * tile_width), tile_width + (y * tile_width), -0.1, 1);
		vec4 p2 = vec4(tile_width + (x * tile_width), tile_width * 2 + (y * tile_width), -0.1, 1);
		vec4 p3 = vec4(tile_width * 2 + (x * tile_width), tile_width + (y * tile_width), -0.1, 1);
		vec4 p4 = vec4(tile_width * 2 + (x * tile_width), tile_width * 2 + (y * tile_width), -0.1, 1);

		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

void gameover()
{
	currentState = STATE_GAME_OVER;
	// ���¼�¼��Ϣ
	endTime = glfwGetTime();
	double duration = endTime - startTime;
	gameRecord.setDuration(duration);
	gameRecord.setScore(score);
	saveGameRecord(gameRecord);
	std::cout<<"-------------------------------------------------------------"<<std::endl;
	std::cout<<"��Ϸ����! "<<std::endl;
	std::cout<<"�û���: "<<gameRecord.getUsername()<<std::endl;
	std::cout<<"ʱ��: "<<gameRecord.getDateTime()<<std::endl;
	std::cout<<"��ʱ: "<<gameRecord.getDuration()<<std::endl;
	std::cout<<"���յ÷�: "<<gameRecord.getScore()<<std::endl;
	std::cout<<"-------------------------------------------------------------"<<std::endl;
	score = 0;
	gameRecord.clear();
	gameRecord.setUsername(currentUsername);
}

// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��飬
// ����Ϸ������ʱ���жϣ�û���㹻�Ŀռ��������µķ��飩
void newTile()
{
	// ���·���������̸���������м�λ�ò�����Ĭ�ϵ���ת����
	tilePos = vec2(board_width / 2, board_height - 2);
	rotation = rand() % 4; // �����ʼ��ת״̬ (0-3)

	currentShapeType = rand() % 7;					  // ���ѡ��һ����״ (0-6)
	currentTileColor = shapeColors[currentShapeType]; // ��ȡ��Ӧ��ɫ

	// ��ѡ�е���״����ת״̬�л�ȡ��������
	for (int i = 0; i < 4; i++)
	{
		// ʹ�� allShapes ��������ȡ��ȷ����״����ת
		tile[i] = (*allShapes[currentShapeType])[rotation][i];
		if (!checkValid(tilePos + tile[i]))
		{ // ֻҪ��һ��������Ч,��Ϸ����
			gameover();
			return;
		}
	}

	updateTile(); // ���µ�ǰ�����VBO

	// ���·��鸳�϶�Ӧ����ɫ
	vec4 newcolours[24]; // 4������ * 6������
	for (int i = 0; i < 24; i++)
		newcolours[i] = currentTileColor; // ʹ�����ѡȡ����ɫ

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

// ��Ϸ��OpenGL��ʼ��
void init()
{
	// ��ʼ�����̸������û�ֱ�ߵķ�����������
	// �������� board_width+1 ��
	// �������� board_height+1 ��
	// һ����2���������꣬����ÿ������һ����ɫֵ

	vec4 gridpoints[board_line_num * 2];
	vec4 gridcolours[board_line_num * 2];

	// ���������ߣ� �޸����
	// ������
	for (int i = 0; i < (board_width + 1); i++)
	{
		gridpoints[2 * i] = vec4((tile_width + (tile_width * i)), tile_width, -0.3, 1);
		gridpoints[2 * i + 1] = vec4((tile_width + (tile_width * i)), (board_height + 1) * tile_width, -0.3, 1);
	}

	// ˮƽ��
	for (int i = 0; i < (board_height + 1); i++)
	{
		gridpoints[2 * (board_width + 1) + 2 * i] = vec4(tile_width, (tile_width + (tile_width * i)), -0.3, 1);
		gridpoints[2 * (board_width + 1) + 2 * i + 1] = vec4((board_width + 1) * tile_width, (tile_width + (tile_width * i)), -0.3, 1);
	}

	// �������߸��ɰ�ɫ
	for (int i = 0; i < (board_line_num * 2); i++)
		gridcolours[i] = white;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	vec4 boardpoints[points_num];
	for (int i = 0; i < points_num; i++)
		boardColors[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
	for (int i = 0; i < board_height; i++)
		for (int j = 0; j < board_width; j++)
		{
			// �޸����
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

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < board_width; i++)
		for (int j = 0; j < board_height; j++)
			board[i][j] = false;

	// ������ɫ��
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
	glBindVertexArray(vao[0]); // ���̸񶥵�

	glGenBuffers(2, vbo);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, (board_line_num * 2) * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, (board_line_num * 2) * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(vao[1]); // ���̸�ÿ������

	glGenBuffers(2, &vbo[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, points_num * sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, points_num * sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(vao[2]); // ��ǰ����

	glGenBuffers(2, &vbo[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);

	glClearColor(0, 0, 0, 0);

	// ������Ȳ���
	// glEnable(GL_DEPTH_TEST);
	// ��Ϸ��ʼ��
	newTile();
}

// �����������㹻�ռ���������ת��ǰ����
void rotate()
{
	int nextRotation = (rotation + 1) % 4;
	vec2 nextTileCoords[4]; // ��һ��״̬������
	for (int i = 0; i < 4; ++i)
	{
		// ʹ�� nextRotation ��ȡ����
		nextTileCoords[i] = (*allShapes[currentShapeType])[nextRotation][i];
	}

	// �����ת���λ���Ƿ���Ч
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
		rotation = nextRotation; // ������ת״̬
		for (int i = 0; i < 4; i++)
		{
			tile[i] = nextTileCoords[i];
		}
		updateTile(); // ���� VBO
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
	// �������������������������������,�ӵ�ǰ�п�ʼ���������-1
	for (int j = row; j < board_height - 1; j++)
	{
		// ����һ��(j+1)��״̬����ɫ���Ƶ���ǰ��(j)
		for (int i = 0; i < board_width; i++)
		{
			board[i][j] = board[i][j + 1];
			// ����VBO�е���ɫ��Ϣ
			vec4 colorAbove = black; // Ĭ��Ϊ��ɫ
			// ���� (i, j+1) �� board_colours �е���ʼ����
			int indexAbove = 6 * (board_width * (j + 1) + i);
			if (indexAbove < points_num)
			{
				colorAbove = boardColors[indexAbove];
			}
			changeCellColor(vec2(i, j), colorAbove);
		}
	}
	// ������
	for (int i = 0; i < board_width; i++)
	{
		board[i][board_height - 1] = false;
		changeCellColor(vec2(i, board_height - 1), black);
	}
	// ���ӵ÷֡���Ϊ�ڴ�������ʾ�Ƚϸ��ӣ�������ʱֻ����������ʾ
	score++;
	std::cout << "����: " << score << std::endl;
}

// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO
void setTile()
{
	// ÿ������
	int minY = board_height - 1;
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilePos).x;
		int y = (tile[i] + tilePos).y;
		minY = std::min(minY, y);
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
		changeCellColor(vec2(x, y), gray);
	}

	// ��Ӱ��ĵײ���ʼ��������У�ֱ������
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

// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)
// �ֱ��Ӧ���������º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse
bool moveTile(vec2 direction)
{
	// �����ƶ�֮��ķ����λ������
	vec2 newPos[4];
	for (int i = 0; i < 4; i++)
		newPos[i] = tile[i] + tilePos + direction;

	// ����ƶ�֮�����Ч��
	if (checkValid(newPos[0]) && checkValid(newPos[1]) && checkValid(newPos[2]) && checkValid(newPos[3]))
	{
		// ��Ч���ƶ��÷���
		tilePos.x = tilePos.x + direction.x;
		tilePos.y = tilePos.y + direction.y;

		updateTile();

		return true;
	}

	return false;
}

// ����������Ϸ
void restart()
{
	// ������̸�״̬
	for (int i = 0; i < board_width; i++)
	{
		for (int j = 0; j < board_height; j++)
		{
			if (board[i][j])
			{
				board[i][j] = false;
				changeCellColor(vec2(i, j), black); // ����ɫ�Ļغ�ɫ
			}
		}
	}
	score = 0;
	// ���ÿ�ʼʱ��
	gameRecord.setDateTime();
	startTime = glfwGetTime();
	lastTime = glfwGetTime();

	newTile();

	std::cout << std::endl
			  << "��Ϸ��ʼ!" << std::endl;
	std::cout << "����: " << score << std::endl;
}

// ��Ϸ��Ⱦ����
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	// �������̸񱳾��������� (������״̬�¶�������Ҫ��ʾ)
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, points_num); // ���̸񱳾� (�ѹ̶��ķ���)
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, board_line_num * 2); // ���̸���

	// ֻ�� PLAYING �� PAUSE ״̬�»��Ƶ�ǰ�����
	if (currentState == STATE_PLAYING || currentState == STATE_PAUSE)
	{
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, 24); // ��ǰ����
	}

	if (currentState == STATE_PAUSE)
	{
		std::cout << "��ͣ��... �� P ����" << '\r';
	}
	else if (currentState == STATE_GAME_OVER)
	{
		std::cout << "�� R ���¿�ʼ" << '\r';
	}
	else if (currentState == STATE_START_SCREEN)
	{
		std::cout << "�� Enter ��ʼ��Ϸ..." << '\r';
	}

	glFlush(); // ȷ�������������
}

// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// ͨ�ð����˳����������κ�״̬�´���
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
	{
		if(currentState == STATE_PAUSE || currentState == STATE_PLAYING){
			// �����Ϸ�ǽ���״̬����ֹͣ״̬, ���б�Ҫ�л���gameoverҲ���ǽ���״̬
			gameover();
		}
		if (action == GLFW_PRESS)
		{
			exit(EXIT_SUCCESS);
		}

		return; // �������˳���ֱ�ӷ���
	}

	// ���ݵ�ǰ��Ϸ״̬������
	switch (currentState)
	{
	case STATE_START_SCREEN:
		// �ڿ�ʼ���棬ֻ��Ӧ�س�����ʼ��Ϸ
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			restart(); // ��ʼ����������Ϸ״̬
			currentState = STATE_PLAYING;
		}
		break;

	case STATE_PLAYING:
		// ����Ϸ������
		// �������ƶ�����ת (ֻ����PLAYING״̬����Ч)
		switch (key)
		{
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				rotate();
			break;
		case GLFW_KEY_SPACE: // ֱ��fall_through
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				if (!moveTile(vec2(0, -1)))
				{
					setTile();
					newTile(); // newTile �ڲ������Ƿ���Ϸ�������л�״̬
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
		// ������ͣ��
		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
			{
				currentState = STATE_PAUSE; // �л�����ͣ״̬
				std::cout << "��Ϸ����ͣ" << std::endl;
				// ��ͣʱ����Ҫ����lastTime����Ϊ�ָ�ʱmainѭ�����Զ�����
			}
			break;
		}
		break;

	case STATE_PAUSE:
		// ����ͣ״̬�£�ֻ��Ӧ 'P' ��ȡ����ͣ
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			currentState = STATE_PLAYING; // �л�����Ϸ״̬
			// ���������ʱ��������ָ�ʱ��������
			lastTime = glfwGetTime();

			std::cout << std::endl
					  << "��Ϸ�ѻָ�" << std::endl;
		}
		break; // STATE_PAUSE ����

	case STATE_GAME_OVER:
		// ����Ϸ����״̬�£�ֻ��Ӧ 'R' �����¿�ʼ
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			currentState = STATE_PLAYING; // �л�����Ϸ״̬
			restart();					  // ������Ϸ
		}
		break; // STATE_GAME_OVER ����
	}
}

void inputUsername()
{
	std::cout<<"Elaine's Tetris"<<std::endl;
	std::cout<<"-------------------------------------------------------------"<<std::endl;
	char recordChoice;
	bool validInput = false;

	while (!validInput)
	{
		std::cout << "�Ƿ�Ҫ��¼������Ϸ�û�����(y/n): ";
		std::cin >> recordChoice;

		// �������뻺��������ֹ�ϴ�����Ļس�Ӱ����� getline
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		recordChoice = std::tolower(recordChoice); // ת��ΪСд

		if (recordChoice == 'y')
		{
			std::cout << "�����������û���: ";
			// ʹ�� std::getline ��ȡ���ܰ����ո���û���
			std::getline(std::cin, currentUsername);
			if (currentUsername.empty())
			{ // ��ֹ�û�ֱ�ӻس�
				currentUsername = defaultUsername;
				std::cout << "�û�������Ϊ�գ���ʹ��Ĭ����: " << defaultUsername << std::endl;
			}
			// std::cout<<"currentUsername: "<<currentUsername<<std::endl;
			gameRecord.setUsername(currentUsername);
			
			validInput = true;
		}
		else if (recordChoice == 'n')
		{

			std::cout << "��ʹ��Ĭ����: " << defaultUsername << std::endl;
			currentUsername = defaultUsername;
			gameRecord.setUsername(currentUsername);
			validInput = true;
		}
		else
		{
			std::cout << "������Ч�������� 'y' �� 'n'" << std::endl;
			// ���� cin �Ĵ���״̬����������˷��ַ���
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
	// ʹ�������н�����ȡusername
	inputUsername();

	// ��ӡ��Ϸ��¼
	printGameRecords();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// �������ڡ�
	// �������������� + �����߿�� + �Ҳ����׿��
	int boardPixelWidth = (board_width + 2) * tile_width; // +2 ��Ϊ�����ұ߿�
	int scoreBoardWidth = 0;
	// scoreBoardWidth = 150;							  // ������Ʒְ��� 150 ����
	int windowWidth = boardPixelWidth + scoreBoardWidth;

	// ������������߶� + �����߸߶�
	int windowHeight = (board_height + 2) * tile_width; // +2 ��Ϊ�����±߿�

	// ʹ���³ߴ紴������
	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Tetris - 2023271073 - Elaine", NULL, NULL);

	// ����ȫ�ֱ��� (��������ط��õ��� xsize, ysize)
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
		// ֻ�� STATE_PLAYING ״̬�´����Զ�����
		if (currentState == STATE_PLAYING)
		{
			double currentTime = glfwGetTime();
			if (currentTime - lastTime >= fallInterval)
			{
				// ���������ƶ�
				if (!moveTile(vec2(0, -1)))
				{
					setTile(); // �̶�����
					newTile(); // �����·��� (newTile �ڲ�����Ƿ� Game Over �������л�״̬)
				}
				lastTime = currentTime; // �����ϴ�����ʱ��
			}
		}

		display(); // ��Ⱦ��ǰ֡

		// �������������¼�����
		glfwSwapBuffers(window); // ��ʾ����
		glfwPollEvents();		 // ������������������¼�
	}
	glfwTerminate();
	return 0;
}