/**
 @file    Window.cpp
 @author  Tyrone Davison
 @date    September 2014

 @note This implementation was originally written for GLFW 2 and has been
 adapted for GLFW3.  Some of the implementation may not be ideal for GLFW3.
 */

#include <tygra/Window.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tygra/WindowControlDelegate.hpp>
#include <tgl/tgl.h>
#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

namespace tygra
{

std::shared_ptr<Window> Window::main_window_;

Window::GamepadState Window::gamepad_state_[Window::MAX_GAMEPADS];

std::shared_ptr<Window> Window::
mainWindow()
{
    if (main_window_ == nullptr) {
        main_window_.reset(new Window());
    }
    return main_window_;
}

std::shared_ptr<WindowViewDelegate> Window::
view() const
{
    return view_;
}

void Window::
setView(std::shared_ptr<WindowViewDelegate> view)
{
	if (view == view_) {
		return;
    }
    if (isVisible()) {
	    if (view_ != nullptr) {
		    view_->windowViewDidStop(shared_from_this());
	    }
        view_ = view;
	    if (view_ != nullptr) {
    	    view_->windowViewWillStart(shared_from_this());
            fakeResizeCallback();
	    }
    } else {
        view_ = view;
    }
}

std::shared_ptr<WindowControlDelegate> Window::
controller() const
{
    return controller_;
}

void Window::
setController(std::shared_ptr<WindowControlDelegate> controller)
{
    if (controller == controller_) {
        return;
    }
    if (isVisible()) {
        if (controller_ != nullptr) {
            controller_->windowControlDidStop(shared_from_this());
        }
        controller_ = controller;
	    if (controller_ != nullptr) {
            controller_->windowControlWillStart(shared_from_this());
	    }
    } else {
        controller_ = controller;
    }
}

bool Window::
open(int width,
     int height,
     int sample_count,
     bool windowed,
	 int gl_major_version,
	 int gl_minor_version)
{
	const int MAX_MAJOR_VERSION = 4;
	if (gl_major_version > MAX_MAJOR_VERSION) {
		return false;
	}
	const int MAX_MINOR_VERSION[] = { 5, 1, 3, 5 };
	if (gl_minor_version > MAX_MINOR_VERSION[gl_major_version-1]) {
		return false;
	}
	int tgl_extension_index = 0;
	for (int i=0; i<(gl_major_version-1); ++i) {
		tgl_extension_index += MAX_MINOR_VERSION[i] + 1;
	}
	tgl_extension_index += gl_minor_version;
	TGLEXTENSION tgl_extension = (TGLEXTENSION)tgl_extension_index;

    if (glfwInit() != GL_TRUE) {
        return false;
    }

    glfwSetErrorCallback(onError);

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 0);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, sample_count);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor_version);
    if (gl_major_version >= 3 && gl_minor_version >= 3) {
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    GLFWmonitor* monitor = windowed ? NULL : glfwGetPrimaryMonitor();
    glfw_handle_ = glfwCreateWindow(width, height, "TyGrA", monitor, NULL);
    if (glfw_handle_ == nullptr) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(glfw_handle_);
    glfwSwapInterval(-1);
    glfwShowWindow(glfw_handle_);

    tglInit();
    if (tglIsAvailable(tgl_extension) != GL_TRUE) {
        glfwTerminate();
        return false;
    }

    if (view_ != nullptr) {
    	view_->windowViewWillStart(shared_from_this());
    }
    
    glfwSetFramebufferSizeCallback(glfw_handle_, onResize);
    glfwSetWindowCloseCallback(glfw_handle_, onClose);
    glfwSetCursorPosCallback(glfw_handle_, onMouseMove);
    glfwSetScrollCallback(glfw_handle_, onMouseWheel);
    glfwSetMouseButtonCallback(glfw_handle_, onMouseButton);
    glfwSetKeyCallback(glfw_handle_, onKeyEvent);

    fakeResizeCallback();

    if (controller_ != nullptr) {
        controller_->windowControlWillStart(shared_from_this());
    }

    return true;
}

bool Window::
isVisible() const
{
    return glfwGetWindowAttrib(glfw_handle_, GLFW_VISIBLE) != 0;
}

void Window::
update()
{
    if (isVisible() == false) {
        return;
    }
    if (view_ != nullptr) {
        if (controller_ != nullptr) {
            controller_->windowControlViewWillRender(shared_from_this());
        }
        view_->windowViewRender(shared_from_this());
    } else {
        glClearColor(0.25f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glfwSwapBuffers(glfw_handle_);
    glfwPollEvents();
    pollGamepads();
}

void Window::
close()
{
    glfwDestroyWindow(glfw_handle_);
    glfwTerminate();
    if (main_window_.get() == this) {
        main_window_.reset();
    }
}

void Window::
setTitle(std::string newTitle)
{
    glfwSetWindowTitle(glfw_handle_, newTitle.c_str());
}

void Window::
resize(int width,
       int height)
{
    glfwSetWindowSize(glfw_handle_, width, height);
}

void Window::
setFullscreen(bool yes)
{
    if (yes) {
        glfwRestoreWindow(glfw_handle_);
    } else {
        glfwIconifyWindow(glfw_handle_);
    }
}

void Window::
fakeResizeCallback()
{
    int width, height;
    Window* window = main_window_.get();
    glfwGetWindowSize(window->glfw_handle_, &width, &height);
    onResize(window->glfw_handle_, width, height);
}

void Window::
onError(int error_code,
        const char* description)
{
    tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, description);
}

void Window::
onResize(GLFWwindow* handle,
         int width,
         int height)
{
    Window* window = main_window_.get();
    if (window != nullptr && window->view_ != nullptr) {
        window->view_->windowViewDidReset(window->shared_from_this(),
                                          width, height);
    }
}

void Window::
onClose(GLFWwindow* handle)
{
    Window* window = main_window_.get();
    if (window != nullptr) {
        if (window->controller_ != nullptr) {
            window->controller_
              ->windowControlDidStop(window->shared_from_this());
        }
        if (window->view_ != nullptr) {
            window->view_->windowViewDidStop(window->shared_from_this());
        }
        glfwHideWindow(window->glfw_handle_);
    }
}

void Window::
onMouseMove(GLFWwindow* handle,
            double x,
            double y)
{
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_->windowControlMouseMoved(window->shared_from_this(),
                                                     (int)x,
                                                     (int)y);
    }
}

void Window::
onMouseWheel(GLFWwindow* handle,
             double x,
             double y)
{
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlMouseWheelMoved(window->shared_from_this(),
                                         (int)y);
    }
}

void Window::
onMouseButton(GLFWwindow* handle,
              int button,
              int action,
              int mods)
{
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlMouseButtonChanged(window->shared_from_this(),
                                            button,
                                            action == GLFW_PRESS);
    }
}

void Window::
onKeyEvent(GLFWwindow* handle,
           int key,
           int scancode,
           int action,
           int mods)
{
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlKeyboardChanged(window->shared_from_this(),
                                         key,
                                         action != GLFW_RELEASE);
    }
}

void Window::
pollGamepads()
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        for (int i=0; i<MAX_GAMEPADS; ++i) {
            // NB: data is copied into newState object to minimize changes when
            // porting from GLFW2 to GLFW3
            int num_glfw_axes = 0;
            const float* axes = glfwGetJoystickAxes(i, &num_glfw_axes);
            int num_axes = num_glfw_axes < MAX_GAMEPAD_AXES
                               ? num_glfw_axes : MAX_GAMEPAD_AXES;
            int num_glfw_buttons = 0;
            const unsigned char*
                buttons = glfwGetJoystickButtons(i, &num_glfw_buttons);
            int num_buttons = num_glfw_buttons < MAX_GAMEPAD_BUTTONS
                ? num_glfw_buttons : MAX_GAMEPAD_BUTTONS;
            GamepadState newState;
            newState.present = glfwJoystickPresent(i) == GL_TRUE;
            for (int j=0; j<num_axes; ++j) {
                newState.axis[j] = axes[j];
            }
            for (int j=0; j<num_buttons; ++j) {
                newState.button[j] = buttons[j];
            }
            if (newState.present != gamepad_state_[i].present) {
                window->controller_->
                    windowControlGamepadPresenceChanged(window->shared_from_this(),
                    i,
                    newState.present);
            }
            // a few hacks to standardize results from a Microsoft Gamepad
            newState.axis[1] = -newState.axis[1];
            newState.axis[3] = -newState.axis[3];
            newState.axis[7] = newState.axis[2] < 0.f ? -newState.axis[2] : 0.f;
            newState.axis[2] = newState.axis[2] > 0.f ? newState.axis[2] : 0.f;
            // TODO: dpad seems to be missing
            for (int j=0; j<num_axes; ++j) {
                if (newState.axis[j] != gamepad_state_[i].axis[j]) {
                    window->controller_->
                      windowControlGamepadAxisMoved(window->shared_from_this(),
                                                    i,
                                                    j,
                                                    newState.axis[j]);
                }
            }
            for (int j=0; j<num_buttons; ++j) {
                if (newState.button[j] != gamepad_state_[i].button[j]) {
                    window->controller_->windowControlGamepadButtonChanged(
                                            window->shared_from_this(),
                                            i,
                                            j,
                                            newState.button[j] == GLFW_PRESS);
                }
            }
            gamepad_state_[i] = newState;
        }
    }
}

Window::
Window()
{
    glfw_handle_ = nullptr;
}

Window::
~Window()
{
}

Window::GamepadState::
GamepadState()
{
    for (int i=0; i<MAX_GAMEPAD_AXES; ++i) {
        axis[i] = 0.f;
    }
    for (int i=0; i<MAX_GAMEPAD_BUTTONS; ++i) {
        button[i] = 0;
    }
}

} // end namespace tygra
