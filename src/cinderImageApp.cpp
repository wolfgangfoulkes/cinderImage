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
    
    Surface pixelKidSurface; //actually Surface8u(nsigned int) there's also a Surface32f(loat)
    //perform C++ operations on a surface. Texture is for GPU
    Surface pixelKidResize;
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
    if (pixelKidSurface.isPremultiplied())
    {
        cout << "Premultiplied by its alpha channel!!\n";
    }
    
    pixelKidResize = resizeCopy(pixelKidSurface, pixelKidSurface.getBounds(), Vec2i(app::getWindowWidth(), app::getWindowHeight()));
    int pkrw = pixelKidSurface.getWidth();
    int pkrh = pixelKidSurface.getHeight();
    cout << "width:" << pkrw << ", height:" << pkrh << "\n";
    
    Area pkrarea(0, 0, pkrw, pkrh);
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
    pixelKidTexture.setWrap(GL_REPEAT, GL_REPEAT);
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
