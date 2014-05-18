#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ip/Resize.h"

using namespace ci;
using namespace ci::app;
using namespace ci::ip;
using namespace std;

class cinderImageApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    void drawRepeatingTexture( const gl::Texture& tex, const Rectf& destRect, const Vec2f& textureBounds );
    
    Surface pixelKidSurface; //actually Surface8u(nsigned int) there's also a Surface32f(loat)
    //perform C++ operations on a surface. Texture is for GPU
    Surface pixelKidResize;
    gl::Texture pixelKidTexture;
    Vec2f texBounds;
};

void cinderImageApp::setup()
{
    pixelKidSurface = *new Surface(loadImage(loadResource("pixelkid.jpg")));
    //App::getResourcePath() will give you a full path.
    int pkw = pixelKidSurface.getWidth();
    int pkh = pixelKidSurface.getHeight();
    cout << "width:" << pkw << ", height:" << pkh << "\n";
    if (!pixelKidSurface.hasAlpha())
    {
        cout << "no Alpha Channel!\n";
        //can't force format. don't fucking ask me why.
    }
    if (pixelKidSurface.isPremultiplied())
    {
        cout << "Premultiplied by its alpha channel!!\n";
    }
    
    pixelKidResize = resizeCopy(pixelKidSurface, pixelKidSurface.getBounds(), Vec2i(app::getWindowWidth()/4, app::getWindowHeight()/4));
    texBounds = Vec2f(pixelKidResize.getWidth(), pixelKidResize.getHeight());
    cout << "width:" << texBounds.x << ", height:" << texBounds.y << "\n";
    cout << "window bounds:" << getWindowBounds() << "\n";
    
    Area pkrarea(0, 0, texBounds.x, texBounds.y);
    Surface::Iter iter = pixelKidResize.getIter( pkrarea );
    while (iter.line())
    {
        while (iter.pixel())
        {
            iter.r() = 255 - iter.r(); //iter.r returns a reference so we can use it.
            iter.g() = 255 - iter.g();
            iter.b() = 255 - iter.b();
        }
    }

    
    pixelKidTexture = gl::Texture(pixelKidResize);
    pixelKidTexture.setWrap(GL_REPEAT, GL_REPEAT); //only repeats with some texture coordinate mapping.
    //pixelKidTexture.enableMipmapping
}

void cinderImageApp::update()
{
}

void cinderImageApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::draw(pixelKidTexture, getWindowBounds(), getWindowBounds()); //weird eh?
}

void cinderImageApp::drawRepeatingTexture( const gl::Texture& tex, const Rectf& destRect, const Vec2f& textureBounds ) //doesn't work right.
{
    tex.enableAndBind();
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    GLfloat verts[8];
    GLfloat	texCoords[8];
    glVertexPointer( 2, GL_FLOAT, 0, verts );
    glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
    
    verts[0*2+0] = destRect.getX2(); verts[0*2+1] = destRect.getY1();
	verts[1*2+0] = destRect.getX1(); verts[1*2+1] = destRect.getY1();
	verts[2*2+0] = destRect.getX2(); verts[2*2+1] = destRect.getY2();
	verts[3*2+0] = destRect.getX1(); verts[3*2+1] = destRect.getY2();
    
    texCoords[0*2+0] = textureBounds.x; texCoords[0*2+1] = 0;
	texCoords[1*2+0] = 0; 				texCoords[1*2+1] = 0;
	texCoords[2*2+0] = textureBounds.x; texCoords[2*2+1] = textureBounds.y;
	texCoords[3*2+0] = 0; 				texCoords[3*2+1] = textureBounds.y;
    
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

CINDER_APP_NATIVE( cinderImageApp, RendererGl )


