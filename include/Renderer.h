#pragma once

class Renderer
{
public:
    void Initialize(float width, float height)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        std::string vertexSourceStr = ReadFile("shaders/vs_sprite.glsl");
        const char* vertexSource = vertexSourceStr.c_str();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

//        GLint compile_ok = GL_FALSE;
//        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile_ok);
//        if (compile_ok == GL_FALSE)
//        {
//            char buffer[512];
//            glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
//            throw Exception("Couldn't compile vs:\n" + std::string(buffer));
//        }

        std::string fragmentSourceStr = ReadFile("shaders/fs_sprite.glsl");
        const char* fragmentSource = fragmentSourceStr.c_str();
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        
        m_sprProgram = glCreateProgram();
        glAttachShader(m_sprProgram, vertexShader);
        glAttachShader(m_sprProgram, fragmentShader);
        glLinkProgram(m_sprProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glBindAttribLocation(m_sprProgram, 0, "vPosition");
        glBindAttribLocation(m_sprProgram, 1, "vTexCoord");
        glBindFragDataLocation(m_sprProgram, 0, "fColor");

        uniforms.transform = glGetUniformLocation(m_sprProgram, "transform");
        uniforms.uv = glGetUniformLocation(m_sprProgram, "uv");
        uniforms.texture_sample = glGetUniformLocation(m_sprProgram, "texture_sample");
        uniforms.visibility = glGetUniformLocation(m_sprProgram, "visibility");

        glm::mat4 view(1.0f);
        transforms.proj = glm::ortho(0.0f, width, height, 0.0f);
        transforms.vp = transforms.proj * view;
    }

    void BeginRender()
    {
        glClearColor(m_backColor.r, m_backColor.g, m_backColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_sprProgram);
    }

    void CleanUp()
    {
        glDeleteProgram(m_sprProgram);
    }

    void SetBackColor(const glm::vec3& color) { m_backColor = color; }

    struct
    {
        GLint   transform,
                uv,
                visibility,
                texture_sample;
    } uniforms;
    struct
    {
        glm::mat4 vp, proj;
    } transforms;

private:
    glm::vec3 m_backColor;
    GLuint m_sprProgram;
};
