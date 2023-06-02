#include <map>
#include <set>

#include "wyvk_device.h"

namespace Wyvern {

void WYVKDevice::initialize(VkInstance instance, std::vector<const char*>& validationLayers, VkSurfaceKHR surface)
{
    m_surface = surface;
    std::vector<VkPhysicalDevice> devices = queryPhysicalDevices(instance);
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }
    WYVERN_LOG_WARN("size {}", candidates.size());
    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        m_physicalDevice = candidates.rbegin()->second;
    }
    else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

    WYVERN_ASSERT(isDeviceSuitable(m_physicalDevice), "The chosen physical device is not suitable for graphics rendering!!!");
    WYVERN_LOG_INFO("Chosen device: {} {} {}", deviceProperties.deviceName, deviceProperties.deviceID, deviceProperties.driverVersion);
    createLogicalDevice(instance, validationLayers);
}

/*
* Destroys the logical device. The physical device does not need to be destroyed as it is considered
* to be implicitly created and destroyed by the vkInstance
*/
void WYVKDevice::destroy()
{
    vkDestroyDevice(m_device, nullptr);
}


std::vector<VkPhysicalDevice> WYVKDevice::queryPhysicalDevices(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        WYVERN_LOG_ERROR("No Vulkan compatible devices found!!");
        throw std::runtime_error("No Vulkan compatible devices found!!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    return devices;
}

// Gives a score to each potential device. Higher scores are better GPU's and will be the primary choice
bool WYVKDevice::rateDeviceSuitability(const VkPhysicalDevice device)
{
    int deviceScore = 0;
    // Properties such as name, type, supported VK version
    VkPhysicalDeviceProperties deviceProperties;
    // Actual device features such as 64 bit floats, multi viewport rendering for VR
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        deviceScore += 1000;
    }
    deviceScore += deviceProperties.limits.maxImageDimension3D;
    return deviceScore;
}

bool WYVKDevice::isDeviceSuitable(const VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.hasAllValidFamilies();
}

WYVKDevice::QueueFamilyIndices WYVKDevice::findQueueFamilies(VkPhysicalDevice device)
{
    WYVKDevice::QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (indices.hasAllValidFamilies()) {
            break;
        }
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        // Check for present support (capability for a device to present to the surface)
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }
    }

    return indices;
}

// We need to pass in validation layers because the logical device has separate validation layers than the instance
// We essentially need to sync them up so they are the same
void WYVKDevice::createLogicalDevice(VkInstance instance, std::vector<const char*>& validationLayers) {
    WYVKDevice::QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
    VkPhysicalDeviceFeatures deviceFeatures{};


    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    // Create queue info's for all queue families in uniqueQueueFamilies
    // Some queue families may have the same index which is ok.
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo deviceQueueInfo{}; 
        VKInfo::createDeviceQueueInfo(deviceQueueInfo, queueFamily, 1, &queuePriority);
        queueCreateInfos.push_back(deviceQueueInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo {};
    VKInfo::createDeviceInfo(deviceCreateInfo, queueCreateInfos, deviceFeatures, validationLayers);

    VK_CALL(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device), "Unable to create Vulkan device!");
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
}

}


