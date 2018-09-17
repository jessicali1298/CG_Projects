/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#include <core/core.h>
#include <core/platform.h>
#include <core/renderer.h>
#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

/**
 * Load TOML scene file and create scene objects.
 */
bool loadTOML(TinyRender::Config& config, const std::string& inputFile) {
    // Scene and Wavefront OBJ files
    const auto data = cpptoml::parse_file(inputFile);
    config.tomlFile = inputFile;
    const auto input = data->get_table("input");
    config.objFile = *input->get_as<std::string>("objfile");

    // Camera settings
    const auto camera = data->get_table("camera");
    config.camera.fov = camera->get_as<double>("fov").value_or(30.);
    auto eye = camera->get_array_of<double>("eye").value_or({1., 1., 0.});
    config.camera.o = v3f(eye[0], eye[1], eye[2]);
    auto at = camera->get_array_of<double>("at").value_or({0., 0., 0.});
    config.camera.at = v3f(at[0], at[1], at[2]);
    auto up = camera->get_array_of<double>("up").value_or({0., 1., 0.});
    config.camera.up = v3f(up[0], up[1], up[2]);

    // Film settings
    const auto film = data->get_table("film");
    config.width = film->get_as<int>("width").value_or(768);
    config.height = film->get_as<int>("height").value_or(576);

    // Renderer settings
    const auto renderer = data->get_table("renderer");
    auto realTime = renderer->get_as<bool>("realtime").value_or(false);
    auto type = renderer->get_as<std::string>("type").value_or("normal");

    // Real-time renderpass
    if (realTime) {
        if (type == "normal") {
            config.renderpass = TinyRender::ENormalRenderPass;
        } else {
            throw std::runtime_error("Invalid renderpass type");
        }
    }

    // Offline integrator
    else {
        if (type == "normal") {
            config.integrator = TinyRender::ENormalIntegrator;
        } else {
            throw std::runtime_error("Invalid integrator type");
        }

        config.spp = renderer->get_as<int>("spp").value_or(1);
    }

    return realTime;
}

/**
 * Launch rendering job.
 */
void run(std::string& inputTOMLFile, bool nogui) {
    TinyRender::Config config;
    bool isRealTime;

    try {
        isRealTime = loadTOML(config, inputTOMLFile);
    } catch (std::exception const& e) {
        std::cerr << "Error while parsing scene file: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    TinyRender::Renderer renderer(config);
    renderer.init(isRealTime, nogui);
    renderer.render();
    renderer.cleanUp();
}

/**
 * Main TinyRender program.
 */
int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        cerr << "Syntax: " << argv[0] << " <scene.toml>" << endl;
        exit(EXIT_FAILURE);
    }

    bool nogui = false;
    if (argc == 3) {
        if (std::string(argv[2]) == "nogui") {
            nogui = true;
        }
    }

    auto inputTOMLFile = std::string(argv[1]);
    run(inputTOMLFile, nogui);

#ifdef _WIN32
    if(!nogui) system("pause");
#endif

    return EXIT_SUCCESS;
}
