#include <map>
#include <set>

#include "wyvk_device.h"

namespace Wyvern {

WYVKDevice::WYVKDevice(WYVKInstance& instance)
    : m_instance(instance)
{
    createPhysicalDevice();
    createLogicalDevice();
}

/*
* Destroys the logical device. The physical device does not need to be destroyed as it is considered
* to be implicitly created and destroyed by the vkInstance
*/
WYVKDevice::~WYVKDevice()
{
    WYVERN_LOG_INFO("Destroying Logical Device...");
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

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
    WYVERN_LOG_INFO("Chosen device: {} {} {}", deviceProperties.deviceName, deviceProperties.deviceID, deviceProperties.driverVersion);
    WYVERN_ASSERT(isPhysicalDeviceSuitable(m_physicalDevice), "The chosen physical device is not suitable for graphics rendering!!!");
}

// We need to pass in validation layers because the logical device has separate validation layers than the instance
// We essentially need to sync them up so they are the same
void WYVKDevice::createLogicalDevice() {
    m_queueFamilyIndices = findQueueFamilies(m_physicalDevice);
    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { 
        m_queueFamilyIndices.graphicsFamily.value(), 
        m_queueFamilyIndices.presentFamily.value(),
        m_queueFamilyIndices.computeFamily.value()
    };

    // Create queue info's for all queue families in uniqueQueueFamilies
    // Some queue families may have the same index which is ok.
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo deviceQueueInfo{};
        deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo.queueFamilyIndex = queueFamily;
        deviceQueueInfo.queueCount = 1;
        deviceQueueInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(deviceQueueInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    if (ENABLE_VALIDATION_LAYERS) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instance.getValidationLayers().size());
        deviceCreateInfo.ppEnabledLayerNames = m_instance.getValidationLayers().data();
    }
    else {
        deviceCreateInfo.enabledLayerCount = 0;
    }
    VK_CALL(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice), "Unable to create Vulkan logical device!");

    vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
    vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndices.computeFamily.value(), 0, &m_graphicsQueue);
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
    VkPhysicalDeviceProperties deviceProperties; // Properties such as name, type, supported VK version
    VkPhysicalDeviceFeatures deviceFeatures;     // Actual device features such as 64 bit floats, multi viewport rendering for VR

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

        if (indices.hasAllValidFamilies()) { // Breaks out of loop if all families are valid
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.computeFamily = i;
        }

        if (queueFamily.queueCount > 0) {
            indices.presentFamily = i;
        }
    }

    return indices;
}

}


