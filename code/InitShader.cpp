#include "Angel.h"
#include <cstdio>  
#include <cstring> 
#include <iostream>
#include <string> 

// �Ƴ�������� fopen_s �궨��
// #define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL

namespace Angel
{
    // Create a NULL-terminated string by reading the provided file
    // �����汾��ʹ�ñ�׼ fopen ����Ӵ�����
    static char* readShaderSource(const char *shaderFile)
    {
        FILE *fp = nullptr;

#ifdef _WIN32
        // �� Windows �ϣ�����ѡ��ʹ�� fopen_s �Ի��Ǳ�ڵİ�ȫ����
        errno_t err = fopen_s(&fp, shaderFile, "r");
        if (err != 0 || fp == nullptr) {
             std::cerr << "����: �޷�����ɫ���ļ� (fopen_s): " << shaderFile << std::endl;
             // ����ԭʼ������Ϊ������Ӧ�÷��� NULL
             return NULL;
        }
#else
        // �ڷ� Windows ƽ̨��Linux, macOS �ȣ�ʹ�ñ�׼ fopen
        fp = fopen(shaderFile, "r");
        if (fp == NULL) {
            // ʹ�� perror �������ϸ��ϵͳ������Ϣ
            perror(("����: �޷�����ɫ���ļ� (fopen): " + std::string(shaderFile)).c_str());
            return NULL;
        }
#endif

        // ��ȡ�ļ���С
        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        if (size == -1L) { // ��Ӵ�����
            perror("����: ��ȡ��ɫ���ļ���Сʧ��");
            fclose(fp);
            return NULL;
        }
        fseek(fp, 0L, SEEK_SET); // �ص��ļ���ͷ

        // �����ڴ� (+1 ���� '\0')
        char *buf = new(std::nothrow) char[size + 1]; // ʹ�� nothrow �����쳣
        if (!buf) {
            std::cerr << "����: ������ɫ��Դ���ڴ�ʧ��" << std::endl;
            fclose(fp);
            return NULL;
        }

        // ��ջ����� (��ѡ��������ʵ��)
        memset(buf, 0, size + 1);

        // ��ȡ�ļ�����
        size_t bytesRead = fread(buf, 1, size, fp);
        // ����ȡ�Ƿ������Լ��Ƿ�������
        if (bytesRead < size && ferror(fp)) {
            perror(("����: ��ȡ��ɫ���ļ�ʧ��: " + std::string(shaderFile)).c_str());
            delete[] buf; // �ͷ��ڴ�
            fclose(fp);
            return NULL;
        }

        // ȷ���ַ����� null ��β (��ʹ fread ��ǰ����)
        buf[bytesRead] = '\0';
        fclose(fp);

        return buf;
    }

    // Create a GLSL program object from vertex and fragment shader files
    // InitShader �������������ṩ�İ汾��ȫ��ͬ�Ľṹ����Ϊ
    GLuint InitShader(const char *vShaderFile, const char *fShaderFile)
    {
        struct Shader
        {
            const char *filename;
            GLenum type;
            GLchar *source; // ����ʹ�� GLchar*
        } shaders[2] = {
            {vShaderFile, GL_VERTEX_SHADER, NULL},
            {fShaderFile, GL_FRAGMENT_SHADER, NULL}};

        GLuint program = glCreateProgram();

        for (int i = 0; i < 2; ++i)
        {
            Shader &s = shaders[i];
            // ����������� readShaderSource
            s.source = readShaderSource(s.filename);
            // ����ԭʼ�� NULL ��鷽ʽ
            if (shaders[i].source == NULL)
            {
                std::cerr << "Failed to read " << s.filename << std::endl;
                exit(EXIT_FAILURE);
            }

            GLuint shader = glCreateShader(s.type);
            glShaderSource(shader, 1, (const GLchar **)&s.source, NULL);
            glCompileShader(shader);

            GLint compiled;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled)
            {
                std::cerr << s.filename << " failed to compile:" << std::endl;
                GLint logSize;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
                char *logMsg = new char[logSize];
                glGetShaderInfoLog(shader, logSize, NULL, logMsg);
                std::cerr << logMsg << std::endl;
                delete[] logMsg;
                // �ڱ���ʧ��ʱҲ��Ҫ�ͷ� s.source
                delete[] s.source;
                exit(EXIT_FAILURE);
            }

            // �����ڱ���͸��Ӻ��ͷ�Դ���ڴ�
            delete[] s.source;

            glAttachShader(program, shader);
            // ����ԭʼ������Ϊ����������ǰɾ�� shader ����
        }

        /* link and error check */
        glLinkProgram(program);

        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            std::cerr << "Shader program failed to link" << std::endl;
            GLint logSize;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
            char *logMsg = new char[logSize];
            glGetProgramInfoLog(program, logSize, NULL, logMsg);
            std::cerr << logMsg << std::endl;
            delete[] logMsg;

            exit(EXIT_FAILURE);
        }

        /* use program object */
        // ����ԭʼ�������Ϊ���ں���ĩβ���� glUseProgram
        glUseProgram(program);

        return program;
    }

} // Close namespace Angel block