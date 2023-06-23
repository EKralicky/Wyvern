#include "wyvk_shader.h"
#include <fstream>

Wyvern::WYVKShader::WYVKShader(WYVKDevice& device, std::filesystem::path& filePath, VkShaderStageFlagBits shaderStage)
    : m_filePath(filePath),
    m_device(device),
    m_shaderStage(shaderStage)
{
}

Wyvern::WYVKShader::~WYVKShader()
{
}

void Wyvern::WYVKShader::compile()
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

#ifdef NDEBUG
    options.SetOptimizationLevel(shaderc_optimization_level_size);
#else
    options.SetOptimizationLevel(shaderc_optimization_level_zero);
#endif

    std::ifstream file(m_filePath, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open GLSL file! " + m_filePath.string());
    }

    // Get contents of file in string format
    std::string source((std::istreambuf_iterator<char>(file)), 
                        std::istreambuf_iterator<char>()); 
    // Compile (source file contents, shader kind: vert or frag, file path, compiler options)
    shaderc_shader_kind kind = mapShaderStageToKind(m_shaderStage);
    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, m_filePath.string().c_str(), options);

    // Check if compilation succeeded
    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        WYVERN_LOG_ERROR(module.GetErrorMessage());
        throw std::runtime_error(module.GetErrorMessage());
    }

    m_binary = std::vector<uint32_t>(module.begin(), module.end());
}

VkShaderModule Wyvern::WYVKShader::createShaderModule(const std::vector<uint32_t>& code)
{
    VkShaderModule shaderModule;
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();

    VK_CALL(vkCreateShaderModule(m_device.getLogicalDevice(), &createInfo, nullptr, &shaderModule), "Failed to create shader module!");

    return shaderModule;
}

shaderc_shader_kind Wyvern::WYVKShader::mapShaderStageToKind(VkShaderStageFlagBits stage)
{
    switch (stage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
        return shaderc_vertex_shader;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
        return shaderc_fragment_shader;
    case VK_SHADER_STAGE_GEOMETRY_BIT:
        return shaderc_geometry_shader;
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        return shaderc_tess_control_shader;
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        return shaderc_tess_evaluation_shader;
    case VK_SHADER_STAGE_COMPUTE_BIT:
        return shaderc_compute_shader;
    case VK_SHADER_STAGE_RAYGEN_BIT_KHR:
        return shaderc_raygen_shader;
    case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:
        return shaderc_anyhit_shader;
    case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:
        return shaderc_closesthit_shader;
    case VK_SHADER_STAGE_MISS_BIT_KHR:
        return shaderc_miss_shader;
    case VK_SHADER_STAGE_INTERSECTION_BIT_KHR:
        return shaderc_intersection_shader;
    case VK_SHADER_STAGE_CALLABLE_BIT_KHR:
        return shaderc_callable_shader;
    default:
        throw std::runtime_error("Unsupported shader stage");
    }
}
