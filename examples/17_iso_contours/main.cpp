#include <cinolib/meshes/meshes.h>
#include <cinolib/geodesics.h>
#include <cinolib/profiler.h>
#include <cinolib/drawable_isocontour.h>
#include <cinolib/gl/glcanvas.h>

int main(int argc, char **argv)
{
    // The code will use the cinolib namespace
    using namespace cinolib;

    // Initialize the GL canvas (GUI) for rendering
    GLcanvas gui;

    // Load a mesh and compute its geodesic distances
    // Determine the path to the input file (mesh to load)
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/circle.obj";
    // Create a DrawableTrimesh object from the input file
    DrawableTrimesh<> m(s.c_str());
    // Compute the geodesic distances on the mesh from a single source vertex (vertex 100) 
    // using the cotangent weights for the Laplacian matrix
    compute_geodesics(m, {100}, COTANGENT).copy_to_mesh(m); // generate some scalar field
    // Show the mesh with the geodesic distances as a 1D texture
    m.show_texture1D(TEXTURE_1D_HSV_W_ISOLINES);
    // Show the mesh with the wireframe and transparency
    m.show_wireframe(true);
    m.show_wireframe_transparency(0.5f);
    // Push the mesh to the GL canvas
    gui.push(&m);

    // Create an iso-contour at a given value of the scalar field
    float iso_val = 0.5f;
    // Create a DrawableIsocontour object from the mesh and the iso value
    // Creates an isocontour on the mesh m at the value iso_val.
    DrawableIsocontour<> iso(m, iso_val);
    // Set the thickness of the isocontour
    iso.thickness = 3;
    // Adds the isocontour to the GUI for rendering
    gui.push(&iso, false);

    // Create a profiler to measure the time needed to tessellate the isocontour
    Profiler profiler;
    // Set the callback function for the GUI controls 
    gui.callback_app_controls = [&]()
    {
        // Creates a slider in the GUI for adjusting the isocontour value
        if(ImGui::SliderFloat("iso", &iso_val, 0.f, 1.f))
        {
            // Update the isocontour value
            iso = DrawableIsocontour<>(m,iso_val);
            // Resets the thickness of the isocontour.
            iso.thickness = 3;
        }
        // Creates a button in the GUI for tessellating the isocontour
        if(ImGui::SmallButton("Tessellate"))
        {
            // Starts a profiling segment for the tessellation process.
            profiler.push("tessellate iso-contour");
            // Tessellates the isocontour
            iso.tessellate(m);
            // Ends the profiling segment for the tessellation process.
            profiler.pop();
            // Prints the profiling results to the console.
            m.updateGL();
        }
    };
    // Launch the GUI
    return gui.launch();
}
