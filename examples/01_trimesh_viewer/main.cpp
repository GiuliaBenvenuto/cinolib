#include <cinolib/meshes/meshes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>

int main(int argc, char **argv)
{
    // The code will use the cinolib namespace
    using namespace cinolib;

    // Define a string that hold the path of the mesh file
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/bunny.obj";
    
    // Create a DrawableTrimesh object that is an instance of the template classe of cinolib
    // It is initialized with the path of the mesh file
    DrawableTrimesh<> m(s.c_str());

    // Create a GLcanvas object responsible for managing the OpenGL context and the rendering window
    GLcanvas gui;
    // Create a SurfaceMeshControls object that provides a user interface for interacting with the mesh
    SurfaceMeshControls<DrawableTrimesh<>> menu(&m, &gui);
    // Adds the DrawableTrimesh object to the GLcanvas object
    gui.push(&m);
    // Adds the SurfaceMeshControls object to the GLcanvas object
    gui.push(&menu);
    // Launch the rendering window GUI with the mesh and the panel for interacting 
    return gui.launch();
}
