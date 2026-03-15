#pragma once

#include <vulkan/vulkan_core.h>

class VulkanContext {
  public:
    VulkanContext();

    ~VulkanContext();

    void queryInfo() const;

  private:
    void createInstance();

    void createLogicalDevice();

    void createDescriptors();

  private:
    VkInstance _instance;
    VkDevice   _device;
    VkQueue    _computeQueue;
};
