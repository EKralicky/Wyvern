#include "wyvk_renderer.h"

namespace Wyvern {

WYVKRenderer::WYVKRenderer(Window& window)
    : m_window(window),
    m_instance(std::make_unique<WYVKInstance>()), 
    m_surface(std::make_unique<WYVKSurface>(*m_instance, window)),
    m_device(std::make_unique<WYVKDevice>(*m_instance, *m_surface))
{

}

void WYVKRenderer::destroy()
{
    m_device->destroy();
    m_surface->destroy();
    m_instance->destroy();
}

}

