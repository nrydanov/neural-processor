cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")

project(AUDIO_PLUGIN_EXAMPLE VERSION 0.0.1)

set(RTNEURAL_STL ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE
add_subdirectory(RTNeural)
include_directories(RTNeural)

juce_add_plugin(NeuralProcessor
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    COPY_PLUGIN_AFTER_BUILD TRUE/FALSE          # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "NeuralProcessor")        # The name of the final executable, which can differ from the target name

target_link_libraries(NeuralProcessor LINK_PUBLIC RTNeural)

juce_generate_juce_header(NeuralProcessor)

target_sources(NeuralProcessor
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

target_compile_definitions(NeuralProcessor
    PUBLIC
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(NeuralProcessor
    PRIVATE
        juce::juce_audio_utils
        RTNeural
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
