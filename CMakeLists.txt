cmake_minimum_required(VERSION 3.20)
project(BiohazardV120SplitScreenDecomp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(re5_split120 STATIC
    src/split_localcoop_120.cpp
    src/split_begin_add_player_120.cpp
    src/split_local_player_activation_120.cpp
    src/split_local_join_root_state_120.cpp
    src/split_root_flag_helpers_120.cpp
    src/split_agame_init_local_reset_120.cpp
    src/split_agame_state10_local_transition_120.cpp
    src/split_agame_dispatch_120.cpp
    src/split_story_local_join_driver_120.cpp
    src/split_story_local_visibility_120.cpp
    src/split_local_transition_helpers_120.cpp
    src/split_index1_flag_routes_120.cpp
    src/split_local_join_bit800_refresh_120.cpp
    src/split_player_input_routing_120.cpp
    src/split_input_owner_queries_120.cpp
    src/split_owner_physical_input_120.cpp
    src/split_selected_local_device_120.cpp
    src/split_gameplay_keyboard_owner_120.cpp
    src/split_gameplay_owner_completion_120.cpp
    src/split_input_availability_120.cpp
    src/split_input_command_mapping_120.cpp
    src/split_routed_prompt_120.cpp
    src/split_prompt_code_dispatch_120.cpp
    src/split_prompt_symbol_selection_120.cpp
    src/split_preferred_device_owner_binding_120.cpp
    src/split_keyboard_owner_writers_completion_120.cpp
    src/split_ui_owner_state_families_120.cpp
    src/split_ui_owner_nonpredicate_120.cpp
    src/split_ui_owner_route_sites_120.cpp
    src/split_ui_owner_consumer_completion_120.cpp
    src/split_ui_input_base_120.cpp
    src/split_state13_120.cpp
    src/split_story_pause_records_120.cpp
    src/split_story_pause_initializer_120.cpp
    src/split_story_pause_option05_120.cpp
    src/split_story_pause_option05_update_120.cpp
    src/split_story_pause_shared_owner_routes_120.cpp
    src/split_story_pause_state8_120.cpp
    src/split_story_pause_selector_120.cpp
    src/split_story_pause_dispatch_120.cpp
    src/split_story_pause_state9_120.cpp
    src/split_story_pause_state9_render_120.cpp
    src/split_keyboard_player_no_120.cpp
    src/split_trial_split_120.cpp
    src/split_trial_join_device_120.cpp
    src/split_session_slots_120.cpp
    src/split_session_native_core_120.cpp
    src/split_session_local_binding_reset_120.cpp
    src/split_pregame_two_slot_120.cpp
    src/split_mode_pregame_120.cpp
    src/split_mode_local_route_120.cpp
    src/split_global_local_mode_consumers_120.cpp
    src/split_frontend_mode_selection_120.cpp
    src/split_dlc_frontend_120.cpp
    src/split_dlc_row_semantics_120.cpp
    src/split_dlc_postconfirm_120.cpp
    src/split_session_mode_predicates_120.cpp
    src/split_dlc_local_activation_120.cpp
    src/split_dlc_join_input_120.cpp
    src/split_mercs_local_activation_120.cpp
    src/split_mercs_join_input_120.cpp
    src/split_mercs_join_reset_120.cpp
    src/split_mercs_join_state32_120.cpp
    src/split_mercs_postjoin_finalize_120.cpp
    src/split_mercs_player_device_owner_120.cpp
    src/split_mercs_frontend_player_init_120.cpp
    src/split_mercs_mode_consumers_120.cpp
    src/split_mercs_owner_gate_completion_120.cpp
    src/split_session_population_gate_120.cpp
    src/split_mercs_join_completion_gate_120.cpp
    src/split_mercs_state3_refresh_120.cpp
    src/split_frontend_transition_120.cpp
    src/split_mercs_frontend_completion_120.cpp
    src/split_ui_route_reset_120.cpp
    src/split_mercs_frontend_owner_120.cpp
    src/split_event_request_120.cpp
    src/split_render_120.cpp
    src/split_coordinate_helpers_120.cpp
    src/split_state_object_init_120.cpp
    src/split_viewport_state_update_120.cpp
    src/split_overlay_geometry_764ae0_120.cpp
    src/split_resource_120.cpp
    src/split_resource_geometry_120.cpp
    src/split_resource_geometry_blocks_120.cpp
    src/split_resource_geometry_more_120.cpp
    src/split_resource_geometry_completion_120.cpp
    src/split_resource728_geometry_120.cpp
    src/split_rebuild_120.cpp
    src/split_effective_full_extra_120.cpp
    src/split_a1be00_120.cpp
    src/split_a22c00_120.cpp
    src/split_cursor_120.cpp
    src/split_cursor_input_120.cpp
    src/split_widget_projection_120.cpp
    src/split_widget_screen_projection_remaining_120.cpp
    src/split_state_change_120.cpp
    src/split_aspect_correction_120.cpp
    src/split_aspect_input_120.cpp
    src/split_aspect_draw_wrapper_120.cpp
    src/split_j1_j2_anchor_geometry_120.cpp
    src/split_measured_x_delta_120.cpp
    src/split_remaining_direct_geometry_120.cpp
    src/split_remaining_scalar_and_composite_120.cpp
    src/split_fullscreen_option_120.cpp
    src/split_localjoin_mode_gate_120.cpp
    src/split_visibility_guard_120.cpp
    src/player_state_accessor_120.cpp
    src/split_table_owner_preguard_120.cpp
    src/split_table_fallback_120.cpp
    src/split_table_draw_120.cpp
    src/split_explicit_fallback_cases_120.cpp
    src/split_ui_dispatch_120.cpp
    src/split_transform_120.cpp
    src/split_hit_test_120.cpp
    src/split_panel_transform_120.cpp
    src/split_ui_metrics_120.cpp
    src/split_draw_dispatch_120.cpp
    src/split_centered_draw_120.cpp
    src/split_extent_shift_draw_120.cpp
    src/split_ui_primary_120.cpp
    src/split_ui_draw_120.cpp
    src/split_top22_120.cpp
    src/split_two_local_player_ui_120.cpp
    src/split_local_player_overlay_draw_120.cpp
    src/split_hud_geometry_completion_120.cpp
    src/split_inline_geometry_completion_120.cpp
    src/split_menu_layout_families_120.cpp
    src/split_prompt_and_remaining_geometry_120.cpp
)

target_include_directories(re5_split120 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(RE5_FREESTANDING_CROSS "Build Win32 COFF objects with Clang without a Windows SDK" OFF)
if(RE5_FREESTANDING_CROSS)
    target_compile_options(re5_split120 PRIVATE
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -nostdinc++
        -I${CMAKE_CURRENT_SOURCE_DIR}/cmake/freestanding
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(re5_split120 PRIVATE
        -Wall -Wextra -Wpedantic
        -include ${CMAKE_CURRENT_SOURCE_DIR}/include/re5/native_abi_audit_120.hpp
    )
endif()

set_target_properties(re5_split120 PROPERTIES
    OUTPUT_NAME "BiohazardV_1.2.0_splitscreen_decomp_win32"
)
