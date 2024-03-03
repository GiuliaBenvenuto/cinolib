#include <cinolib/meshes/meshes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>
#include <cinolib/profiler.h>

int main(int argc, char **argv)
{
    using namespace cinolib;

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/maxFace.obj";

    DrawableTrimesh<> m(s.c_str());
    m.show_vert_color();

    GLcanvas gui;
    SurfaceMeshControls<DrawableTrimesh<>> menu(&m, &gui);
    gui.push(&m);
    gui.push(&menu);

    // associates to the mouse single click event a function that
    // unprojects the 2D click point and looks for the mesh vertex
    // closest to it.
    // NOTE: the complexity here is linear w.r.t. the number of verts.
    // In my personal experience this has never been a bottleneck, not
    // even on big meshes. Should this be the case, accelleration data
    // structures (e.g. octrees) could be used

    // Instance of a Profiler class
    Profiler profiler;
    // Lambda function that is called when the mouse left button is clicked
    // The int modifiers parameter allows the function to check if any modifier keys (like shift or control) were pressed when the mouse was clicked.
    gui.callback_mouse_left_click = [&](int modifiers) -> bool
    {
        // Check if the shift key (GLFW_MOD_SHIFT) is pressed during the mouse click
        if(modifiers & GLFW_MOD_SHIFT)
        {
            // If the shift key is pressed, the function will print the time it takes to pick the vertex
            // Declare a 3D vector to store the unprojected point
            vec3d p;
            // Gets the current cursor position on the screen (a 2D point) and stores it in the "click" variable
            vec2d click = gui.cursor_pos();
            // Try to unproject the 2D cursor position (click) to a 3D point in the scene, storing the result in p
            if(gui.unproject(click, p)) // transform click in a 3d point
            {   // If the unprojection is successfu
                // Start the profiler to measure performance of the pick_vert function
                profiler.push("Vertex pick");

                // Call to method pick_vert on an object m (mesh), passing the unprojected point p
                // This function is used to find the vertex closest to the point p
                uint vid = m.pick_vert(p);

                // Ends the profiling of the pick_vert function
                profiler.pop();

                // Prints the ID of the picked vertex to the console
                std::cout << "ID " << vid << std::endl;

                // Set the color of the picked vertex to red
                m.vert_data(vid).color = Color::RED();

                // Refresh the OpenGL buffer with the changed vertex data so the color change
                m.updateGL();
            }
        }
        return false;
    };

    return gui.launch();
}
