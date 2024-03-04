#include <cinolib/meshes/meshes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>
#include <cinolib/sphere_coverage.h>
#include <cinolib/profiler.h>

int main(int, char **)
{
    using namespace cinolib;

    // Init number of points
    int n_points = 1000;
    // Create a vector to store the 3D points (samples) on the sphere
    std::vector<vec3d> samples;
    // Call the sphere_coverage function to generate the samples (1000 samples)
    sphere_coverage(1000, samples);
    // Create a vector to store the IDs of the polygons on the mesh (empty in this case)
    std::vector<uint> no_polys = {};
    // Create a DrawableTrimesh object to store the samples and the polygons (no polygons in this case)
    DrawableTrimesh<> m(samples, no_polys);
    // Set the color of the vertices to black
    m.vert_set_color(Color::BLACK());
    // Show the wireframe of the mesh with a width of 3
    m.show_wireframe_width(3);
    // Show the mesh points
    m.show_mesh_points();

    // Create a GLCanvas object for the GUI
    GLcanvas gui;
    // Add the mesh to the GUI window to render it
    gui.push(&m);

    // Create a Profiler object to measure performance
    Profiler profiler;
    // Lambda function that is called when the app controls are updated
    gui.callback_app_controls = [&]()
    {
        // Show the GUI window
        // In the GUI there is a text label "Num Points" 
        ImGui::Text("Num Points:");
        // In the GUI there is a slider to chenage the number of points on the sphere
        if(ImGui::SliderInt("##Num points",&n_points,100,10000))
        {
            // Start a new profiling section named "Sphere coverage"
            profiler.push("Sphere coverage");
            // Updates the distribution of points on the sphere based on the slider's value.
            sphere_coverage((uint)n_points, samples);
            // End the profiling section
            profiler.pop();
            // Update the mesh with the new samples
            m = DrawableTrimesh<>(samples, no_polys);
            // Set the color of the vertices to black
            m.vert_set_color(Color::BLACK());
            // Show the wireframe of the mesh with a width of 3
            m.show_wireframe_width(3);
            // Show the mesh points
            m.show_mesh_points();
        }
    };

    // Launch the GUI
    return gui.launch();
}
