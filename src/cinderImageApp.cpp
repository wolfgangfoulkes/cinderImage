#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cinderImageApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    Surface pixelKidSurface; //actually Surface8u(nsigned int) there's also a Surface32f(loat)
    //perform C++ operations on a surface. Texture is for GPU
    gl::Texture pixelKidTexture;
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
    
    
    Area pkarea(0, 0, pkw, pkh);
    Surface::Iter iter = pixelKidSurface.getIter( pkarea );
    while (iter.line())
    {
        while (iter.pixel())
        {
            iter.r() = iter.r(); //iter.r returns a reference so we can use it.
            iter.g() = iter.g();
            iter.b() = iter.b();
        }
    }

    

    pixelKidTexture = gl::Texture(pixelKidSurface);
    pixelKidTexture.setWrap(GL_REPEAT, GL_REPEAT);
    //pixelKidTexture.setWrap(GL_REPEAT, GL_REPEAT); //x, y
    //pixelKidTexture.enableMipmapping
    
    
    
}

void cinderImageApp::update()
{
}

void cinderImageApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::draw(pixelKidTexture, getWindowBounds());
}

CINDER_APP_NATIVE( cinderImageApp, RendererGl )
