#include <map>
#include <set>

#include "wyvk_device.h"

namespace Wyvern {

WYVKDevice::WYVKDevice(WYVKInstance& instance, WYVKSurface& surface)
    : m_instance(instance),
    m_surface(surface)
{
    createPhysicalDevice();
    createLogicalDevice();
}

/*
* Destroys the logical device. The physical device does not need to be destroyed as it is considered
* to be implicitly created and destroyed by the vkInstance
*/
void WYVKDevice::destroy()
{
    vkDestroyDevice(m_logicalDevice, nullptr);
}


void WYVKDevice::createPhysicalDevice() {
    // Retrieve valid physical devices from the system
    std::vector<VkPhysicalDevice> devices = queryPhysicalDevices(m_instance.getInstance());
    std::multimap<int, VkPhysicalDevice> candidates;

    // Choose best device based on rating
    for (const auto& device : devices) {
        int score = ratePhysicalDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }
    WYVERN_LOG_WARN("size {}", candidates.size());
    if (candidates.rbegin()->first > 0) {     // Check if the best candidate is suitable at all
        m_physicalDevice = candidates.rbegin()->second;
    }
    else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    // Get device properties, log, and check if the device is suitable at all. If not, exit the program
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
    WYVERN_LOG_INFO("Chosen device: {} {} {}", deviceProperties.deviceName, deviceProperties.deviceID, deviceProperties.driverVersion);
    WYVERN_ASSERT(isPhysicalDeviceSuitable(m_physicalDevice), "The chosen physical device is not suitable for graphics rendering!!!");
}
// We need to pass in validation layers because the logical device has separate validation layers than the instance
// We essentially need to sync them up so they are the same
void WYVKDevice::createLogicalDevice() {
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

    VkDeviceCreateInfo deviceCreateInfo{};
    VKInfo::createDeviceInfo(deviceCreateInfo, queueCreateInfos, deviceFeatures);
    VK_CALL(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice), "Unable to create Vulkan logical device!");

    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
}

//  Retrieves a list of physical device objects representing the physical devices installed in the system
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
bool WYVKDevice::ratePhysicalDeviceSuitability(VkPhysicalDevice device)
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

// Checks if a given device is suitable or not for our requirements. e.g. checks for extension support
bool WYVKDevice::isPhysicalDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkPhysicalDeviceExtensionSupport(device);

    return indices.hasAllValidFamilies() && extensionsSupported;
}

// Retrieves all available extensions from the device and checks the extensions against the m_deviceExtensions member variable to
// see if our required extensions exist
bool WYVKDevice::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
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
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface.getSurface(), &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }
    }

    return indices;
}

}


