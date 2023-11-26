//#include "WYVK.h"
//#include "Wyvern/Application.h"
//#include <map>
//
//namespace Wyvern {
//
//void WYVK::createInstance(VkInstance& instance, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo) {
//    if (enableValidationLayers && !checkValidationLayerSupport(WYVKValidationLayers)) {
//        throw std::runtime_error("validation layers requested, but not available!");
//    }
//
//    VkApplicationInfo appInfo{};
//    createAppInfo(appInfo);
//
//    VkInstanceCreateInfo createInfo{};
//    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//    createInfo.pApplicationInfo = &appInfo;
//
//    // Apply validation layers if enabled
//    if (enableValidationLayers) {
//        createInfo.enabledLayerCount = static_cast<uint32_t>(WYVKValidationLayers.size());
//        createInfo.ppEnabledLayerNames = WYVKValidationLayers.data();
//        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
//    }
//    else {
//        createInfo.enabledLayerCount = 0;
//        createInfo.pNext = nullptr;
//    }
//
//    // We need extensions in Vulkan to interface with the window
//    getRequiredExtensions(WYVKEnabledExtensions); // Get GLFW required extensions
//
//    //========================
//    // ADD EXTENSIONS BELOW!!!
//    //========================
//
//    createInfo.enabledExtensionCount = static_cast<uint32_t>(WYVKEnabledExtensions.size());
//    createInfo.ppEnabledExtensionNames = WYVKEnabledExtensions.data();
//
//    WYVERN_LOG_INFO("Enabled Extensions:");
//    for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
//        WYVERN_LOG_INFO("\t{}", createInfo.ppEnabledExtensionNames[i]);
//    }
//
//    VK_CALL(vkCreateInstance(&createInfo, nullptr, &instance));
//}
//
//bool WYVK::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
//    uint32_t layerCount;
//    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//
//    std::vector<VkLayerProperties> availableLayers(layerCount);
//    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
//
//    for (const char* layerName : validationLayers) {
//        bool layerFound = false;
//
//        for (const auto& layerProperties : availableLayers) {
//            if (strcmp(layerName, layerProperties.layerName) == 0) {
//                layerFound = true;
//                break;
//            }
//        }
//
//        if (!layerFound) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//void WYVK::getRequiredExtensions(std::vector<const char*>& extensions) {
//    uint32_t glfwExtensionCount = 0;
//    const char** glfwExtensions;
//
//    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//    extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//    // Adds the debug extension for debug messagess.
//    // These messages are useful when we are using validation layers
//    if (enableValidationLayers) {
//        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//    }
//}
//
//void WYVK::initDebugMessenger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT& createInfo, VkDebugUtilsMessengerEXT& debugMessenger)
//{
//    if (!enableValidationLayers) return;
//    if (createDebugMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
//        throw std::runtime_error("failed to set up debug messenger!");
//    }
//}
//
//void WYVK::destroyDebugMessengerEXT(VkInstance& instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
//{
//    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//    if (func != nullptr) {
//        func(instance, debugMessenger, pAllocator);
//    }
//}
//
//VkResult WYVK::createDebugMessengerEXT(VkInstance& instance, 
//                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
//                                       const VkAllocationCallbacks* pAllocator, 
//                                       VkDebugUtilsMessengerEXT* pDebugMessenger)
//{
//    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//    if (func != nullptr) {
//        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
//    }
//    else {
//        return VK_ERROR_EXTENSION_NOT_PRESENT;
//    }
//}
//
//void WYVK::pickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& physicalDevice)
//{
//    std::vector<VkPhysicalDevice> devices = queryPhysicalDevices(instance);
//    std::multimap<int, VkPhysicalDevice> candidates;
//
//    for (const auto& device : devices) {
//        int score = rateDeviceSuitability(device);
//        candidates.insert(std::make_pair(score, device));
//    }
//    WYVERN_LOG_WARN("size {}", candidates.size());
//    // Check if the best candidate is suitable at all
//    if (candidates.rbegin()->first > 0) {
//        physicalDevice = candidates.rbegin()->second;
//    }
//    else {
//        throw std::runtime_error("failed to find a suitable GPU!");
//    }
//
//    VkPhysicalDeviceProperties deviceProperties;
//    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
//
//    WYVERN_ASSERT(isDeviceSuitable(physicalDevice), "The chosen physical device is not suitable for graphics rendering!!!");
//    WYVERN_LOG_INFO("Chosen device: {} {} {}", deviceProperties.deviceName, deviceProperties.deviceID, deviceProperties.driverVersion);
//}
//
//std::vector<VkPhysicalDevice> WYVK::queryPhysicalDevices(VkInstance& instance)
//{
//    uint32_t deviceCount = 0;
//    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//    if (deviceCount == 0) {
//        WYVERN_LOG_ERROR("No Vulkan compatible devices found!!");
//        throw std::runtime_error("No Vulkan compatible devices found!!");
//    }
//    std::vector<VkPhysicalDevice> devices(deviceCount);
//    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
//    return devices;
//}
//
//// Gives a score to each potential device. Higher scores are better GPU's and will be the primary choice
//bool WYVK::rateDeviceSuitability(const VkPhysicalDevice& device) {
//    int deviceScore = 0;
//    // Properties such as name, type, supported VK version
//    VkPhysicalDeviceProperties deviceProperties;
//    // Actual device features such as 64 bit floats, multi viewport rendering for VR
//    VkPhysicalDeviceFeatures deviceFeatures;
//
//    vkGetPhysicalDeviceProperties(device, &deviceProperties);
//    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//
//    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
//        deviceScore += 1000;
//    }
//    deviceScore += deviceProperties.limits.maxImageDimension3D;
//    return deviceScore;
//}
//
//bool WYVK::isDeviceSuitable(VkPhysicalDevice& device)
//{
//    QueueFamilyIndices indices = findQueueFamilies(device);
//    return indices.hasAllValidFamilies();
//}
//
//WYVK::QueueFamilyIndices WYVK::findQueueFamilies(VkPhysicalDevice& device)
//{
//    WYVK::QueueFamilyIndices indices;
//    uint32_t queueFamilyCount = 0;
//
//    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//    int i = 0;
//    for (const auto& queueFamily : queueFamilies) {
//        if (indices.hasAllValidFamilies()) {
//            break;
//        }
//        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//            indices.graphicsFamily = i;
//        }
//    }
//
//    return indices;
//}
//
//void WYVK::createLogicalDevice(VkInstance& instance, VkPhysicalDevice& physicalDevice, VkDevice& device) {
//    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
//    VkPhysicalDeviceFeatures deviceFeatures{};
//
//    float queuePriority = 1.0f;
//    VkDeviceCreateInfo deviceCreateInfo;
//    VkDeviceQueueCreateInfo deviceQueueInfo;
//    createDeviceQueueInfo(deviceQueueInfo, indices.graphicsFamily.value(), 1, &queuePriority);
//    createDeviceInfo(deviceCreateInfo, deviceQueueInfo, deviceFeatures);
//
//    VK_CALL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));
//}
//
////====================
//// INFO CREATION
////====================
//
//void WYVK::createDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& messengerCreateInfo) {
//    messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//    messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//    messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//    messengerCreateInfo.pfnUserCallback = debugCallback;
//    messengerCreateInfo.pUserData = nullptr; // Optional
//    messengerCreateInfo.pNext = nullptr;
//    messengerCreateInfo.flags = 0;
//}
//
//void WYVK::createAppInfo(VkApplicationInfo& appInfo) {
//    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//    appInfo.pApplicationName = "Wyvern";
//    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.pEngineName = "No Engine";
//    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.apiVersion = VK_API_VERSION_1_0;
//}
//
//void WYVK::createDeviceInfo(VkDeviceCreateInfo& deviceCreateInfo, VkDeviceQueueCreateInfo& queueCreateInfo, VkPhysicalDeviceFeatures& deviceFeatures) {
//    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
//    deviceCreateInfo.queueCreateInfoCount = 1;
//    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
//}
//
//void WYVK::createDeviceQueueInfo(VkDeviceQueueCreateInfo& queueCreateInfo, int queueFamilyIndex, int queueCount, float* queuePriority) {
//    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
//    queueCreateInfo.queueCount = queueCount;
//    queueCreateInfo.pQueuePriorities = queuePriority;
//}
//
//
//
//}
