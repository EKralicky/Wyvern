#pragma once

extern Wyvern::Application* Wyvern::createApplication();

int main() {
    auto app = Wyvern::createApplication();

    try {
        app->run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}