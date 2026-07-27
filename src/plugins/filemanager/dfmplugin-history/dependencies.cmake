# dependencies.cmake - Dependencies configuration for dfmplugin-history
# This file defines the specific dependencies and configuration for the history plugin

cmake_minimum_required(VERSION 3.10)

# Include DFM plugin configuration module
include(DFMPluginConfig)

# Function to setup history plugin dependencies
function(dfm_setup_history_dependencies target_name)
    message(STATUS "DFM: Setting up history plugin dependencies for: ${target_name}")

    # Apply default plugin configuration (links DFM6::base and DFM6::framework)
    dfm_apply_default_plugin_config(${target_name})

    message(STATUS "DFM: History plugin dependencies configured successfully")
endfunction()

message(STATUS "DFM: History plugin dependencies configuration loaded") 
