#include <cinolib/meshes/meshes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>

int main(int argc, char **argv)
{
    // The code will use the cinolib namespace
    using namespace cinolib;

    // Define a string that hold the path of the mesh file
    std::string filename = (argc==3) ? std::string(argv[1]) : std::string(DATA_PATH) + "/blub_triangulated.obj";
    // Define a string that hold the path of the texture file
    std::string texture  = (argc==3) ? std::string(argv[2]) : std::string(DATA_PATH) + "/blub_texture.png";

    // Init two GLcanvas objects, used to create two separate graphical windows
    GLcanvas gui_xyz, gui_uvw;

    // PRIMITIVE: triangle
    // Creates a DrawableTrimesh object from the specified file 
    DrawableTrimesh<> m_xyz(filename.c_str());
    // Apply a 2D texture to the m_xyz mesh. The texture is loaded from the texture file path determined earlier.
    m_xyz.show_texture2D(TEXTURE_2D_BITMAP, 1.f, texture.c_str());
    // Mesh without wireframe
    m_xyz.show_wireframe(false);
    // Add the mesh to the gui_xyz canvas for rendering
    gui_xyz.push(&m_xyz);

    // PRIMITIVE: triangle
    // Create a copy of the m_xyz mesh named m_uvw
    DrawableTrimesh<> m_uvw = m_xyz;
    // Transform the UVW texture coordinates to XYZ coordinates on the m_uvw mesh.
    m_uvw.copy_uvw_to_xyz(UVW_param);
    // Update the normals of the m_uvw mesh.
    m_uvw.update_normals();
    // Apply a 2D texture to the m_uvw mesh. The texture is loaded from the texture file path determined earlier.
    m_uvw.show_texture2D(TEXTURE_2D_BITMAP, 1.f, texture.c_str());
    // Mesh without wireframe
    m_uvw.show_wireframe(false);
    // Add the mesh to the gui_uvw canvas for rendering
    gui_uvw.push(&m_uvw);

    // only the main window can host visual controls, so all menus are pushed in gui_xyz
    SurfaceMeshControls<DrawableTrimesh<>> menu_xyz(&m_xyz, &gui_xyz, "object space");
    SurfaceMeshControls<DrawableTrimesh<>> menu_uvw(&m_uvw, &gui_xyz, "texture space");
    gui_xyz.push(&menu_xyz);
    gui_xyz.push(&menu_uvw);

    // the main window launches the event loop for itself and for all other windows in the app
    return gui_xyz.launch({&gui_uvw});
}
