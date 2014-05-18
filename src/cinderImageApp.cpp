#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cinderImageApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void cinderImageApp::setup()
{
}

void cinderImageApp::mouseDown( MouseEvent event )
{
}

void cinderImageApp::update()
{
}

void cinderImageApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( cinderImageApp, RendererGl )
