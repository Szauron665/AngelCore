# AngelScript SDK CMakeLists.txt
# Builds only the essential addons needed for TrinityCore integration

# Core AngelScript library source files
set(ANGELSCRIPT_CORE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_atomic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_builder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_bytecode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_compiler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_configgroup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_datatype.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_gc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_generic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_globalproperty.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_module.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_objecttype.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_outputbuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_restore.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_scriptcode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_scriptengine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_scriptfunction.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_scriptnode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_scriptobject.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_string.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_string_util.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_thread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_tokenizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_typeinfo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_variablescope.cpp
)

# Platform-specific callfunc implementations
if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit Windows
    list(APPEND ANGELSCRIPT_CORE_SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x64_msvc.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x64_msvc_asm.asm
    )
    set_source_files_properties(
      ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x64_msvc_asm.asm
      PROPERTIES LANGUAGE ASM_MASM
    )
  else()
    # 32-bit Windows
    list(APPEND ANGELSCRIPT_CORE_SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x86.cpp
    )
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
      # 64-bit x86 Linux/macOS
      list(APPEND ANGELSCRIPT_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x64_gcc.cpp
      )
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
      # ARM64 Linux
      list(APPEND ANGELSCRIPT_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_arm64.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_arm64_gcc.S
      )
      set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_arm64_gcc.S
        PROPERTIES LANGUAGE C
      )
    endif()
  else()
    # 32-bit Linux
    list(APPEND ANGELSCRIPT_CORE_SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/source/as_callfunc_x86.cpp
    )
  endif()
endif()

# Essential addon sources
set(ANGELSCRIPT_ADDON_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptstdstring/scriptstdstring.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptstdstring/scriptstdstring_utils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptarray/scriptarray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scripthelper/scripthelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptmath/scriptmath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptbuilder/scriptbuilder.cpp
)

# Create static library
add_library(angelscript STATIC
  ${ANGELSCRIPT_CORE_SOURCES}
  ${ANGELSCRIPT_ADDON_SOURCES}
)

# Include directories
target_include_directories(angelscript
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/angelscript/include
    ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptstdstring
    ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptarray
    ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scripthelper
    ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptmath
    ${CMAKE_CURRENT_SOURCE_DIR}/add_on/scriptbuilder
)

# Compile options
target_compile_options(angelscript
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
)

# Disable exceptions if needed for performance (optional)
# target_compile_definitions(angelscript PRIVATE AS_NO_EXCEPTIONS)

set_target_properties(angelscript
  PROPERTIES
    FOLDER "AngelScript"
)
