/**
 @file      WindowViewDelegate.hpp
 @author    Tyrone Davison
 @date      September 2012
 */

#pragma once
#ifndef __TYGRA_WINDOW_VIEW_DELEGATE__
#define __TYGRA_WINDOW_VIEW_DELEGATE__

#include <memory>

namespace tygra
{

class Window;

/**
 A window object requires a delegate object to implement this interface
 to define how the view (the window contents) should be rendered.
 */
class WindowViewDelegate
{
public:
    /**
     Tells the delegate that it will start to be used.
     Optional method.
     @param window  The window object using the delegate.
     */
    virtual void
    windowViewWillStart(std::shared_ptr<Window> window) {}

    /**
     Tells the delegate that the view has been reset, usually following
     a window resize event. Optional method.
     @param window  The window object using the delegate.
     @param width   The new width of the view in pixels.
     @param height  The new height of the view in pixels.
     */
	virtual void
    windowViewDidReset(std::shared_ptr<Window> window,
                       int width,
                       int height) {}

    /**
     Tells the delegate that it is no longer being used.
     Optional method.
     @param window  The window object using the delegate.
     */
    virtual void
    windowViewDidStop(std::shared_ptr<Window> window) {}

    /**
     Tells the delegate to render the view. Required method.
     @param window  The window object using the delegate.
     */
    virtual void
    windowViewRender(std::shared_ptr<Window> window) =0;
};

} // end namespace tygra

#endif
