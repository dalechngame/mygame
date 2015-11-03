#include "SpriteWithHue.h"


void xRotateMat(float mat[3][3], float rs, float rc);
void yRotateMat(float mat[3][3], float rs, float rc);
void zRotateMat(float mat[3][3], float rs, float rc);
void matrixMult(float a[3][3], float b[3][3], float c[3][3]);
void hueMatrix(GLfloat mat[3][3], float angle);
void premultiplyAlpha(GLfloat mat[3][3], float alpha);

cocos2d::Map<std::string, GLProgram*>* SpriteWithHue::s_glProgramHueCache = nullptr;

SpriteWithHue::SpriteWithHue()
{
	m_hue = 0.0f;
}

SpriteWithHue::~SpriteWithHue()
{
    
}

SpriteWithHue* SpriteWithHue::create(const std::string& filename)
{
    SpriteWithHue *sprite = new (std::nothrow) SpriteWithHue();
    if (sprite && sprite->initWithFile(filename.c_str()))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool SpriteWithHue::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    if (!Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }

   // this->setupDefaultSettings();
	this->addShader();
    return true;
}

void SpriteWithHue::setupDefaultSettings()
{
    this->m_hue = 0.0;
}

void SpriteWithHue::initShader()
{
	GLProgram * p = new GLProgram();
	this->setGLProgram(p);
	p->release();
	p->initWithFilenames("shader/hueChange.vsh", "shader/hueChange.fsh");
	p->link();
	p->updateUniforms();
	this->getUniformLocations();
	this->updateColor();
}

GLProgram* SpriteWithHue::addShader(const char *shaderName)
{
	// 增加缓存机制
	if (!s_glProgramHueCache)
		s_glProgramHueCache = new cocos2d::Map<std::string, GLProgram*>();

	GLProgram* program = s_glProgramHueCache->at(shaderName);
	if (!program)
	{
		program = new GLProgram();
		program->autorelease();
		program->initWithFilenames("shader/hueChange.vsh", "shader/hueChange.fsh");
		program->link();
		program->updateUniforms();
		s_glProgramHueCache->insert(shaderName, program);
	}

	this->setGLProgram(program);
	this->getUniformLocations();
	this->updateColor();

	return program;
}

void SpriteWithHue::getUniformLocations()
{
    m_hueLocation = glGetUniformLocation(this->getGLProgram()->getProgram(), "u_hue");
    m_alphaLocation = glGetUniformLocation(this->getGLProgram()->getProgram(), "u_alpha");
}

void SpriteWithHue::updateColorMatrix()
{
    this->getGLProgram()->use();
    GLfloat mat[3][3];
    memset(mat, 0, sizeof(GLfloat)*9);
    hueMatrix(mat, m_hue);
    premultiplyAlpha(mat, this->alpha());
    glUniformMatrix3fv(m_hueLocation, 1, GL_FALSE, (GLfloat *)&mat);
}

void SpriteWithHue::updateAlpha()
{
    this->getGLProgram()->use();
    glUniform1f(m_alphaLocation, this->alpha());
}

GLfloat SpriteWithHue::alpha()
{
    
    return _displayedOpacity / 255.0f;
}

void SpriteWithHue::setHue(GLfloat _hue)
{
    m_hue = _hue;
    this->updateColorMatrix();
}

void SpriteWithHue::updateColor()
{
    Sprite::updateColor();
    this->updateColorMatrix();
    this->updateAlpha();
}

SpriteWithHue* SpriteWithHue::createWithTexture(Texture2D *texture, GLfloat hue)
{
	SpriteWithHue *pobSprite = new SpriteWithHue();
	pobSprite->m_hue = hue;
	Rect rect = Rect::ZERO;
	rect.size = texture->getContentSize();
	if (pobSprite && pobSprite->initWithTexture(texture, rect, false))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

void xRotateMat(float mat[3][3], float rs, float rc)
{
    mat[0][0] = 1.0;
    mat[0][1] = 0.0;
    mat[0][2] = 0.0;
    
    mat[1][0] = 0.0;
    mat[1][1] = rc;
    mat[1][2] = rs;
    
    mat[2][0] = 0.0;
    mat[2][1] = -rs;
    mat[2][2] = rc;
}

void yRotateMat(float mat[3][3], float rs, float rc)
{
    mat[0][0] = rc;
    mat[0][1] = 0.0;
    mat[0][2] = -rs;
    
    mat[1][0] = 0.0;
    mat[1][1] = 1.0;
    mat[1][2] = 0.0;
    
    mat[2][0] = rs;
    mat[2][1] = 0.0;
    mat[2][2] = rc;
}


void zRotateMat(float mat[3][3], float rs, float rc)
{
    mat[0][0] = rc;
    mat[0][1] = rs;
    mat[0][2] = 0.0;
    
    mat[1][0] = -rs;
    mat[1][1] = rc;
    mat[1][2] = 0.0;
    
    mat[2][0] = 0.0;
    mat[2][1] = 0.0;
    mat[2][2] = 1.0;
}

void matrixMult(float a[3][3], float b[3][3], float c[3][3])
{
    int x, y;
    float temp[3][3];
    
    for(y=0; y<3; y++) {
        for(x=0; x<3; x++) {
            temp[y][x] = b[y][0] * a[0][x] + b[y][1] * a[1][x] + b[y][2] * a[2][x];
        }
    }
    for(y=0; y<3; y++) {
        for(x=0; x<3; x++) {
            c[y][x] = temp[y][x];
        }
    }
}

void hueMatrix(GLfloat mat[3][3], float angle)
{
#define SQRT_2      sqrt(2.0)
#define SQRT_3      sqrt(3.0)
    
    float mag, rot[3][3];
    float xrs, xrc;
    float yrs, yrc;
    float zrs, zrc;
    
    // Rotate the grey vector into positive Z
    mag = SQRT_2;
    xrs = 1.0/mag;
    xrc = 1.0/mag;
    xRotateMat(mat, xrs, xrc);
    mag = SQRT_3;
    yrs = -1.0/mag;
    yrc = SQRT_2/mag;
    yRotateMat(rot, yrs, yrc);
    matrixMult(rot, mat, mat);
    
    // Rotate the hue
    zrs = sin(angle);
    zrc = cos(angle);
    zRotateMat(rot, zrs, zrc);
    matrixMult(rot, mat, mat);
    
    // Rotate the grey vector back into place
    yRotateMat(rot, -yrs, yrc);
    matrixMult(rot,  mat, mat);
    xRotateMat(rot, -xrs, xrc);
    matrixMult(rot,  mat, mat);
}

void premultiplyAlpha(GLfloat mat[3][3], float alpha)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat[i][j] *= alpha;
        }
    }
}
