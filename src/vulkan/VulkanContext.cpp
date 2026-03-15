#include "VulkanContext.hpp"

#include "log.hpp"

#include <cstdint>
#include <format>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

VulkanContext::VulkanContext()
{
    // Mandatory construction
    createInstance();
    createLogicalDevice();

    // Will require parameters
    createDescriptors();
    createPipeline();
}

void VulkanContext::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType      = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // maybe only debug?
    const char*          validationLayer = "VK_LAYER_KHRONOS_validation";
    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType               = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo    = &appInfo;
    instanceInfo.enabledLayerCount   = 1;
    instanceInfo.ppEnabledLayerNames = &validationLayer;

    vkCreateInstance(&instanceInfo, nullptr, &_instance);
}

void VulkanContext::createLogicalDevice()
{

    // for now we just use a single device, but in future we can have it look for
    // certain presets and the 1 device is the last preset if it can't find
    // anything else

    // --- Find physical device ---
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("No Vulkan compatible physical device found!");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    VkResult foundDevice = vkEnumeratePhysicalDevices(_instance, &deviceCount, physicalDevices.data());

    VkPhysicalDevice physicalDevice = physicalDevices[0];

    // --- Find compute queue family ---
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t computeQueueFamily = -1;
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            computeQueueFamily = i;
            break;
        }
    }
    if (computeQueueFamily == -1)
        throw std::runtime_error("No compute queue found");

    // --- Logical device ---
    float                   queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // what are these types for?
    queueInfo.queueFamilyIndex = computeQueueFamily;
    queueInfo.queueCount       = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos    = &queueInfo;

    vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &_device);

    vkGetDeviceQueue(_device, computeQueueFamily, 0, &_computeQueue);
}

void VulkanContext::createDescriptors()
{
    // hard coded single DescriptorSet with 3 SSBO bindings

    // --- Descriptor set layout ---
    VkDescriptorSetLayoutBinding bindings[3]{};

    // 3 storage buffers
    for (int i = 0; i < 3; i++)
    {
        bindings[i].binding         = i;
        bindings[i].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[i].descriptorCount = 1;
        bindings[i].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 3;
    layoutInfo.pBindings    = bindings;

    vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout);

    // --- Descriptor pool ---
    VkDescriptorPoolSize poolSize{};
    poolSize.type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSize.descriptorCount = 3;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets       = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &poolSize;

    vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);

    // VkBuffer     buffers[3] = {gridBuffer, accumBuffer, photonBuffer};
    // VkDeviceSize sizes[3]   = {gridSize, accumSize, photonBatchSize};

    // for (int i = 0; i < 3; i++)
    // {
    //     VkDescriptorBufferInfo bufInfo{};
    //     bufInfo.buffer = buffers[i];
    //     bufInfo.offset = 0;
    //     bufInfo.range  = sizes[i];

    //     VkWriteDescriptorSet write{};
    //     write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    //     write.dstSet          = descriptorSet;
    //     write.dstBinding      = i;
    //     write.descriptorCount = 1;
    //     write.descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    //     write.pBufferInfo     = &bufInfo;

    //     vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
    // }
}

void VulkanContext::createPipeline()
{
    // --- Pipeline layout ---
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts    = &_descriptorSetLayout;

    vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_pipelineLayout);

    // Pipeline might change dynamically (different shaders)
}

void VulkanContext::queryInfo() const
{

    // get highest available version
    uint32_t availVersion;
    vkEnumerateInstanceVersion(&availVersion);
    Log::info(
        "Available Vulkan instance version: {}.{}.{}",
        VK_API_VERSION_MAJOR(availVersion),
        VK_API_VERSION_MINOR(availVersion),
        VK_API_VERSION_PATCH(availVersion)
    );

    // get physical device count
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

    // get physical devices
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, physicalDevices.data());

    // --- Physical Devices ---
    VkPhysicalDeviceProperties deviceProperties;
    Log::info("Vulkan compatible devices:");
    for (uint32_t d = 0; d < deviceCount; d++)
    {
        const auto physicalDevice = physicalDevices[d];

        // get device properties
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        Log::info_t("  {}{}", Log::Color::Bold, deviceProperties.deviceName);

        // --- Queue Families ---
        // get queue family count
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        // get queue all families
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
        for (uint32_t f = 0; f < queueFamilyCount; f++)
        {
            auto queueFamily = queueFamilies[f];

            // get queue count
            uint32_t queueCount = queueFamily.queueCount;

            // get queue flags
            std::string flagStr;
            auto        flags = queueFamilies[f].queueFlags;

            if (flags & VK_QUEUE_GRAPHICS_BIT)
                flagStr += "GRAPHICS | ";
            if (flags & VK_QUEUE_COMPUTE_BIT)
                flagStr += "COMPUTE | ";
            if (flags & VK_QUEUE_TRANSFER_BIT)
                flagStr += "TRANSFER | ";
            if (flags & VK_QUEUE_SPARSE_BINDING_BIT)
                flagStr += "SPARSE_BINDING | ";
            if (flags & VK_QUEUE_PROTECTED_BIT)
                flagStr += "PROTECTED | ";
            if (flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR)
                flagStr += "VIDEO_DECODE | ";
            if (flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
                flagStr += "VIDEO_ENCODE | ";
            if (flags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
                flagStr += "OPTICAL_FLOW | ";

            // trim trailing " | "
            if (!flagStr.empty())
                flagStr.erase(flagStr.size() - 3);

            Log::info_t("    family {}:", f);
            Log::info_t("      {} queue(s): {}", queueCount, flagStr);
        }

        // --- Memory Properties ---
        VkPhysicalDeviceMemoryProperties memProps{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

        Log::info_t("    memory properties:");
        for (uint32_t i = 0; i < memProps.memoryHeapCount; i++)
        {
            auto& heap = memProps.memoryHeaps[i];

            std::string flags;
            if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                flags += "DEVICE_LOCAL ";
            if (heap.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
                flags += "MULTI_INSTANCE ";
            Log::info_t("      heap {}: {} MB {}", i, heap.size / 1024 / 1024, flags);

            for (uint32_t j = 0; j < memProps.memoryTypeCount; j++)
            {
                auto& type = memProps.memoryTypes[j];
                if (type.heapIndex != i)
                    continue;

                std::string flags;
                if (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                    flags += "DEVICE_LOCAL ";
                if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                    flags += "HOST_VISIBLE ";
                if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
                    flags += "HOST_COHERENT ";
                if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
                    flags += "HOST_CACHED ";
                Log::info_t("        type {}: {}", j, flags);
            }
        }

        Log::info_t("");
    }
}

VulkanContext::~VulkanContext()
{
    vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
    vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
    vkDestroyDevice(_device, nullptr);
    vkDestroyInstance(_instance, nullptr);
}